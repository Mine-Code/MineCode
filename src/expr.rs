use nom::bytes::complete::take_until;
use nom::character::complete;
use nom::combinator::opt;
use nom::sequence::delimited;
use nom::{branch, multi, IResult};

use crate::basic::{self, ident, symbol};

#[derive(Debug)]
pub enum ExprData {
    Num(i32),
    Ident(String),
    String(String),
    FuncCall(String, Vec<SingleExpr>),
}

impl ExprData {
    pub fn num_expr(input: &str) -> IResult<&str, Self> {
        let (input, num) = complete::digit1(input)?;
        Ok((input, ExprData::Num(num.parse().unwrap())))
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
                nom::multi::separated_list0(basic::symbol(','), SingleExpr::read),
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
        let (input, expr) = branch::alt((
            Self::num_expr,
            Self::string,
            Self::call_function,
            Self::ident,
        ))(input)?;

        Ok((input, expr))
    }
}

#[derive(Debug)]
pub enum SingleExpr {
    CompileTime(ExprData),
    RunTime(ExprData),
}

impl SingleExpr {
    pub fn read(input: &str) -> IResult<&str, Self> {
        let (input, a) = opt(basic::symbol('@'))(input)?;

        let (input, expr) = ExprData::read(input)?;

        if a.is_none() {
            Ok((input, Self::RunTime(expr)))
        } else {
            Ok((input, Self::CompileTime(expr)))
        }
    }
}

#[derive(Debug)]
pub enum Expr {
    Single(SingleExpr),
    Ranged(SingleExpr, SingleExpr),
}

impl Expr {
    pub fn read_ranged(input: &str) -> IResult<&str, Self> {
        let (input, lhs) = SingleExpr::read(input)?;
        let (input, _) = nom::bytes::complete::tag("...")(input)?;
        let (input, rhs) = SingleExpr::read(input)?;
        Ok((input, Self::Ranged(lhs, rhs)))
    }

    pub fn read_single(input: &str) -> IResult<&str, Self> {
        let (input, expr) = SingleExpr::read(input)?;
        Ok((input, Self::Single(expr)))
    }

    pub fn read(input: &str) -> IResult<&str, Self> {
        branch::alt((Self::read_ranged, Self::read_single))(input)
    }
}
