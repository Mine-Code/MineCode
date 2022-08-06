use super::expression;
use super::func_def;
use super::mcl;
use crate::ast::Stmt;
use nom::branch::alt;
use nom::bytes::complete::tag;
use nom::combinator::opt;
use nom::sequence::terminated;
use nom::IResult;
use nom::Parser;

pub fn stmt(input: &str) -> IResult<&str, Stmt> {
    terminated(alt((func_def, mcl, expression)), opt(tag(";"))).parse(input)
}
