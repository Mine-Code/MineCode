use std::str::FromStr;

use nom::{
    branch::alt, bytes::complete::tag, character::complete::multispace0, combinator::opt,
    multi::many0, sequence::delimited, IResult,
};

use crate::ast::{BinaryOp, Expr};

use super::range;

fn _binary_op<'a>(
    i: &'a str,
    joiner: impl FnMut(&'a str) -> IResult<&'a str, &'a str>,
    mut parser: impl FnMut(&'a str) -> IResult<&'a str, Expr> + Copy,
) -> IResult<&'a str, Expr> {
    let mut joiner = delimited(multispace0, joiner, multispace0);
    let part = move |i: &'a str| {
        let (i, op) = joiner(i)?;
        let (i, expr) = parser(i)?;
        Ok((i, (op, expr)))
    };

    let (i, mut r) = parser(i)?;
    let (i, parts) = many0(part)(i)?;

    for (op, x) in parts {
        if op == "=" {
            r = Expr::Assignment(Box::new(r), Box::new(x));
        } else {
            r = Expr::ApplyOperator(BinaryOp::from_str(op).unwrap(), Box::new(r), Box::new(x));
        }
    }
    Ok((i, r))
}

fn _pow(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, tag("**"), range)
}
fn _level_shift(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, alt((tag("<<"), tag(">>"))), _pow)
}
fn _bitwise(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, alt((tag("|"), tag("&"), tag("^"))), _level_shift)
}
fn _term(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, alt((tag("*"), tag("/"), tag("%"))), _bitwise)
}
fn _math_expr(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, alt((tag("+"), tag("-"))), _term)
}
fn _size_comp(input: &str) -> IResult<&str, Expr> {
    _binary_op(
        input,
        alt((tag("<="), tag(">="), tag("<"), tag(">"))),
        _math_expr,
    )
}
fn _equ_neq(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, alt((tag("!="), tag("=="))), _size_comp)
}
fn _logical_and(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, tag("&&"), _equ_neq)
}
fn _logical_or(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, tag("||"), _logical_and)
}
fn _assignment(input: &str) -> IResult<&str, Expr> {
    _binary_op(input, tag("="), _logical_or)
}
pub fn expr(input: &str) -> IResult<&str, Expr> {
    let (i, a) = _assignment(input)?;
    let (i, _) = opt(tag(";"))(i)?;
    Ok((i, a))
}
