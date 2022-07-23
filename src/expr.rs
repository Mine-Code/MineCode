use std::fmt::Write;
use std::str::FromStr;

use nom::bytes::complete::take_until;
use nom::character::complete;
use nom::combinator::opt;
use nom::sequence::delimited;
use nom::{branch, IResult, Parser};

use crate::basic::{self, ident, symbol};

#[derive(Debug)]
pub enum UnaryOp {
    Minus,
    BooleanNot,
    LogicalNot,
}

#[derive(Debug)]
pub enum BinaryOp {
    Add,                // +
    Sub,                // -
    Mul,                // *
    Div,                // /
    Mod,                // %
    LogicalOr,          // ||
    LogicalAnd,         // &&
    BitwiseOr,          // |
    BitwiseAnd,         // &
    BitwiseXor,         // ^
    ShiftLeft,          // <<
    ShiftRight,         // >>
    Equal,              // ==
    NotEqual,           // !=
    LessThan,           // <
    LessThanOrEqual,    // <=
    GreaterThan,        // >
    GreaterThanOrEqual, // >=
}

impl std::fmt::Display for BinaryOp {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Add => write!(f, "+"),
            Self::Sub => write!(f, "-"),
            Self::Mul => write!(f, "*"),
            Self::Div => write!(f, "/"),
            Self::Mod => write!(f, "%"),
            Self::LogicalOr => write!(f, "||"),
            Self::LogicalAnd => write!(f, "&&"),
            Self::BitwiseOr => write!(f, "|"),
            Self::BitwiseAnd => write!(f, "&"),
            Self::BitwiseXor => write!(f, "^"),
            Self::ShiftLeft => write!(f, "<<"),
            Self::ShiftRight => write!(f, ">>"),
            Self::Equal => write!(f, "=="),
            Self::NotEqual => write!(f, "!="),
            Self::LessThan => write!(f, "<"),
            Self::LessThanOrEqual => write!(f, "<="),
            Self::GreaterThan => write!(f, ">"),
            Self::GreaterThanOrEqual => write!(f, ">="),
        }
    }
}

impl FromStr for BinaryOp {
    fn from_str(s: &str) -> Result<Self, Self::Err> {}
}

#[derive(Debug)]
pub enum Primary {
    Num(i32),
    Ident(String),
    String(String),
    FuncCall(String, Vec<Box<Primary>>),

    Ranged(Box<Primary>, Box<Primary>),
    Pointer(Box<Primary>),
    CompileTime(Box<Primary>),

    Unary(UnaryOp, Box<Primary>),
    Binary(BinaryOp, Box<Primary>, Box<Primary>),
}

impl Primary {
    pub fn num_expr(input: &str) -> IResult<&str, Self> {
        let (input, num) = complete::digit1(input)?;
        Ok((input, Self::Num(num.parse().unwrap())))
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
        let (input, f) = nom::branch::permutation((
            ident,
            nom::sequence::delimited(
                basic::symbol('('),
                nom::multi::separated_list0(basic::symbol(','), Self::read.map(Box::new)),
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

    pub fn read(input: &str) -> IResult<&str, Self> {
        println!("[EXPR] Reading: {:?}", input);

        let (input, comptime_flag) = opt(basic::symbol('@'))(input)?;

        let (input, mut expr) = branch::alt((
            // []
            nom::sequence::delimited(
                basic::symbol('['),
                Self::read.map(Box::new),
                basic::symbol(']'),
            )
            .map(|e| Primary::Pointer(e)),
            //
            Self::num_expr,
            Self::string,
            Self::call_function,
            Self::ident,
        ))(input)?;

        if comptime_flag.is_some() {
            expr = Self::CompileTime(Box::new(expr));
        }

        if input.starts_with("...") {
            let (input, _) = nom::bytes::complete::tag("...")(input)?;
            let (input, expr2) = Self::read(input)?;
            return Ok((input, Self::Ranged(Box::new(expr), Box::new(expr2))));
        }

        Ok((input, expr))
    }
}
