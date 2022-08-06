use crate::{ast::Stmt, parser::basic::ident};
use nom::bytes::complete::tag;
use nom::Parser;
use nom::{branch::permutation, character::complete::multispace0, IResult};

pub fn mcl(input: &str) -> IResult<&str, Stmt> {
    permutation((multispace0, tag("mcl"), multispace0, ident))
        .map(|(_, _, _, name)| Stmt::LoadModule { module: name })
        .parse(input)
}
