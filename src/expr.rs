use std::str::FromStr;

use strum_macros::EnumString;

use nom::branch::{alt, permutation};
use nom::bytes::complete::{tag, take_until};
use nom::character::complete::{digit1, multispace0, one_of};
use nom::combinator::opt;
use nom::multi::{many0, many1, separated_list0};
use nom::sequence::{delimited, preceded};
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
    #[strum(serialize = "=")]
    Assignment,
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

    LogicalNot(Box<Primary>),
    BitwiseNot(Box<Primary>),
    Negative(Box<Primary>),

    Subscript(Box<Primary>, Box<Primary>),
}

impl Primary {
    fn _num_hex(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0x")(input)?;
        let (input, num) = many1(one_of("0123456789abcdefABCDEFxX"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 16).unwrap())))
    }
    fn _num_oct(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0o")(input)?;
        let (input, num) = many1(one_of("01234567"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 8).unwrap())))
    }
    fn _num_dec(input: &str) -> IResult<&str, Self> {
        let (input, num) = many1(digit1)(input)?;

        let num = num.join("");
        Ok((input, Primary::Num(i32::from_str(&num).unwrap())))
    }
    fn _num_bin(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0b")(input)?;
        let (input, num) = many1(one_of("01"))(input)?;

        let num = num.iter().collect::<String>();
        Ok((input, Primary::Num(i32::from_str_radix(&num, 2).unwrap())))
    }
    fn _num(input: &str) -> IResult<&str, Self> {
        alt((
            Self::_num_hex,
            Self::_num_oct,
            Self::_num_dec,
            Self::_num_bin,
        ))(input)
    }
    fn _ident(input: &str) -> IResult<&str, Self> {
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
    fn _call_function(input: &str) -> IResult<&str, Self> {
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
    fn _string(input: &str) -> IResult<&str, Self> {
        let (input, s) =
            delimited(basic::symbol('"'), take_until("\""), basic::symbol('"'))(input)?;

        Ok((input, Self::String(s.to_string())))
    }
}

impl Primary {
    fn _binary_op<'a>(
        i: &'a str,
        joiner: impl FnMut(&'a str) -> IResult<&'a str, &'a str>,
        parser: impl FnMut(&'a str) -> IResult<&'a str, Primary> + Copy,
    ) -> IResult<&'a str, Primary> {
        permutation((
            parser.clone(),
            many0(permutation((
                delimited(multispace0, joiner, multispace0),
                parser,
            ))),
        ))
        .map(|(mut r, parts)| {
            for (op, x) in parts {
                r = Self::ApplyOperator(BinaryOp::from_str(op).unwrap(), Box::new(r), Box::new(x));
            }
            r
        })
        .parse(i)
    }

    fn _primary(input: &str) -> IResult<&str, Self> {
        let parser = alt((
            preceded(basic::symbol('-'), Self::read.map(Box::new)).map(|e| Self::Negative(e)),
            preceded(basic::symbol('~'), Self::read.map(Box::new)).map(|e| Self::BitwiseNot(e)),
            preceded(basic::symbol('!'), Self::read.map(Box::new)).map(|e| Self::LogicalNot(e)),
            // CompileTime
            preceded(basic::symbol('@'), Self::read.map(Box::new)).map(|e| Self::CompileTime(e)),
            // []
            delimited(
                basic::symbol('['),
                Self::read.map(Box::new),
                basic::symbol(']'),
            )
            .map(|e| Self::Pointer(e)),
            //
            Self::_num,
            Self::_string,
            Self::_call_function,
            Self::_ident,
        ));
        let parser = permutation((parser, opt(delimited(symbol('{'), Self::read, symbol('}')))))
            .map(|(array, index)| {
                if index.is_none() {
                    array
                } else {
                    Self::Subscript(Box::new(array), Box::new(index.unwrap()))
                }
            });

        let mut parser =
            permutation((parser, opt(preceded(tag("..."), Self::_primary)))).map(|(begin, end)| {
                if end.is_some() {
                    Self::Ranged(Box::new(begin), Box::new(end.unwrap()))
                } else {
                    begin
                }
            });

        let (input, expr) = parser.parse(input)?;

        let (input, expr) = if input.starts_with("...") {
            let (input, _) = tag("...")(input)?;
            let (input, expr2) = Self::read(input)?;
            (input, Self::Ranged(Box::new(expr), Box::new(expr2)))
        } else {
            (input, expr)
        };
        Ok((input, expr))
    }

    fn _pow(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("**"), Self::_primary)
    }
    fn _level_shift(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("<<"), tag(">>"))), Self::_pow)
    }
    fn _bitwise(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(
            input,
            alt((tag("|"), tag("&"), tag("^"))),
            Self::_level_shift,
        )
    }
    fn _term(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("*"), tag("/"), tag("%"))), Self::_bitwise)
    }
    fn _math_expr(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("+"), tag("-"))), Self::_term)
    }
    fn _size_comp(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(
            input,
            alt((tag("<"), tag(">"), tag("<="), tag(">="))),
            Self::_math_expr,
        )
    }
    fn _equ_neq(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("!="), tag("=="))), Self::_size_comp)
    }
    fn _logical_and(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("&&"), Self::_equ_neq)
    }
    fn _logical_or(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("||"), Self::_logical_and)
    }
    fn _assignment(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("="), Self::_logical_or)
    }
    pub fn read(input: &str) -> IResult<&str, Self> {
        Self::_assignment(input)
    }
}
