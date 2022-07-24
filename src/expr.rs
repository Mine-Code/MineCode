use std::str::FromStr;

use strum_macros::EnumString;

use nom::branch::{alt, permutation};
use nom::bytes::complete::{tag, take_until};
use nom::character::complete::{digit1, multispace0, one_of};
use nom::combinator::opt;
use nom::multi::{many1, separated_list0};
use nom::sequence::delimited;
use nom::{IResult, Parser};

use crate::basic::{self, ident, symbol};

#[derive(Debug, EnumString, strum_macros::Display)]
pub enum BinaryOp {
    #[strum(serialize = "+")]
    Add,
    #[strum(serialize = "-")]
    Sub,
    #[strum(serialize = "*")]
    Mul,
    #[strum(serialize = "/")]
    Div,
    #[strum(serialize = "%")]
    Mod,
    #[strum(serialize = "**")]
    Power,
    #[strum(serialize = "||")]
    LogicalOr,
    #[strum(serialize = "&&")]
    LogicalAnd,
    #[strum(serialize = "|")]
    BitwiseOr,
    #[strum(serialize = "&")]
    BitwiseAnd,
    #[strum(serialize = "^")]
    BitwiseXor,
    #[strum(serialize = "<<")]
    ShiftLeft,
    #[strum(serialize = ">>")]
    ShiftRight,
    #[strum(serialize = "==")]
    Equal,
    #[strum(serialize = "!=")]
    NotEqual,
    #[strum(serialize = "<")]
    LessThan,
    #[strum(serialize = "<=")]
    LessThanOrEqual,
    #[strum(serialize = ">")]
    GreaterThan,
    #[strum(serialize = ">=")]
    GreaterThanOrEqual,
}

#[derive(Debug)]
pub enum Primary {
    Num(i32),
    Ident(String),
    String(String),
    FuncCall(String, Vec<Primary>),

    Ranged(Box<Primary>, Box<Primary>),
    Pointer(Box<Primary>),
    CompileTime(Box<Primary>),

    ApplyOperator(BinaryOp, Box<Primary>, Box<Primary>),

    Assign(Box<Primary>, Box<Primary>),
    Subscript(Box<Primary>, Box<Primary>),
}

fn apply_operator(operators: &[BinaryOp]) -> impl Fn(&str) -> IResult<&str, Primary> {
    let operators = operators
        .iter()
        .map(|x| x.to_string())
        .collect::<Vec<_>>()
        .join("");
    move |input| {
        let (input, root) = Primary::primary(input)?;

        let (input, parts) = many1(permutation((
            delimited(multispace0, one_of(operators.as_str()), multispace0),
            Primary::read,
        )))(input)?;

        let mut expr = root;

        for (op, right) in parts {
            expr = Primary::ApplyOperator(
                BinaryOp::from_str(op.to_string().as_str()).unwrap(),
                Box::new(expr),
                Box::new(right),
            );
        }
        Ok((input, expr))
    }
}

impl Primary {
    pub fn num_hex(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0x")(input)?;
        let (input, num) = many1(one_of("0123456789abcdefABCDEFxX"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 16).unwrap())))
    }
    pub fn num_oct(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0o")(input)?;
        let (input, num) = many1(one_of("01234567"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 8).unwrap())))
    }
    pub fn num_dec(input: &str) -> IResult<&str, Self> {
        let (input, num) = many1(digit1)(input)?;

        let num = num.join("");
        Ok((input, Primary::Num(i32::from_str(&num).unwrap())))
    }

    pub fn num_bin(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0b")(input)?;
        let (input, num) = many1(one_of("01"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 2).unwrap())))
    }

    pub fn num(input: &str) -> IResult<&str, Self> {
        alt((Self::num_hex, Self::num_oct, Self::num_dec, Self::num_bin))(input)
    }

    pub fn ident(input: &str) -> IResult<&str, Self> {
        let (mut input, mut num) = ident(input)?;

        loop {
            let ret = symbol('.')(input);
            if ret.is_err() {
                break;
            }

            let tmp = ret.unwrap();
            input = tmp.0;
            num.push_str(".");

            let ret = ident(input);
            if ret.is_err() {
                break;
            }
            let tmp = ret.unwrap();
            input = tmp.0;
            num.push_str(&tmp.1);
        }

        Ok((input, Self::Ident(num)))
    }

    pub fn call_function(input: &str) -> IResult<&str, Self> {
        let (input, f) = permutation((
            ident,
            delimited(
                basic::symbol('('),
                separated_list0(basic::symbol(','), Self::read),
                basic::symbol(')'),
            ),
        ))(input)?;

        Ok((input, Self::FuncCall(f.0, f.1)))
    }

    pub fn string(input: &str) -> IResult<&str, Self> {
        let (input, s) =
            delimited(basic::symbol('"'), take_until("\""), basic::symbol('"'))(input)?;

        Ok((input, Self::String(s.to_string())))
    }

    fn primary(input: &str) -> IResult<&str, Self> {
        let (input, comptime_flag) = opt(basic::symbol('@'))(input)?;

        let (input, mut expr) = alt((
            // []
            delimited(
                basic::symbol('['),
                Self::read.map(Box::new),
                basic::symbol(']'),
            )
            .map(|e| Primary::Pointer(e)),
            //
            Self::num,
            Self::string,
            Self::call_function,
            Self::ident,
        ))(input)?;

        if comptime_flag.is_some() {
            expr = Self::CompileTime(Box::new(expr));
        }

        let a = basic::symbol('{')(input);

        let (input, expr) = if a.is_ok() {
            let (input, expr2) =
                delimited(basic::symbol('{'), Self::read, basic::symbol('}'))(input)?;
            (input, Primary::Subscript(Box::new(expr), Box::new(expr2)))
        } else {
            (input, expr)
        };

        let (input, expr) = if input.starts_with("...") {
            let (input, _) = tag("...")(input)?;
            let (input, expr2) = Self::read(input)?;
            (input, Self::Ranged(Box::new(expr), Box::new(expr2)))
        } else {
            (input, expr)
        };
        Ok((input, expr))
    }

    fn _read(input: &str) -> IResult<&str, Self> {
        let a = alt((
            apply_operator(&[BinaryOp::ShiftLeft, BinaryOp::ShiftRight]),
            apply_operator(&[
                BinaryOp::BitwiseAnd,
                BinaryOp::BitwiseOr,
                BinaryOp::BitwiseXor,
            ]),
            apply_operator(&[BinaryOp::Power]),
            apply_operator(&[BinaryOp::Mul, BinaryOp::Div, BinaryOp::Mod]),
            apply_operator(&[BinaryOp::Add, BinaryOp::Sub]),
            apply_operator(&[
                BinaryOp::LessThan,
                BinaryOp::LessThanOrEqual,
                BinaryOp::GreaterThan,
                BinaryOp::GreaterThanOrEqual,
            ]),
            apply_operator(&[BinaryOp::Equal, BinaryOp::NotEqual]),
            apply_operator(&[BinaryOp::LogicalAnd]),
            apply_operator(&[BinaryOp::LogicalOr]),
            Self::primary,
        ))(input);

        a
    }

    // Parses _read = _read, _read
    pub fn read(input: &str) -> IResult<&str, Self> {
        let a = alt((
            permutation((Self::_read, basic::symbol('='), Self::_read))
                .map(|(l, _, r)| Self::Assign(Box::new(l), Box::new(r))),
            Self::_read,
        ))(input);

        a
    }
}
