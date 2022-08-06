use super::stmt;
use crate::{
    ast::Stmt,
    parser::basic::{ident, symbol},
};
use nom::bytes::complete::tag;
use nom::{branch::permutation, multi::separated_list0, sequence::delimited, IResult};
use nom::{character::complete::multispace0, Parser};

pub fn func_def(input: &str) -> IResult<&str, Stmt> {
    let (input, _) = permutation((multispace0, tag("fn"), multispace0))(input)?;
    permutation((
        ident,
        delimited(
            symbol('('),
            separated_list0(symbol(','), ident),
            symbol(')'),
        ),
        stmt.map(Box::new),
    ))
    .map(|(name, args, body)| Stmt::FuncDef { name, args, body })
    .parse(input)
}
