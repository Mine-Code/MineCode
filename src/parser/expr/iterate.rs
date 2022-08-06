use crate::ast::Expr;
use nom::branch::permutation;
use nom::bytes::complete::tag;
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::{IResult, Parser};

use super::parser::expr;

pub fn _for(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("for")(input)?;

    permutation((
        expr.map(Box::new),
        multispace0,
        tag("in"),
        multispace0,
        expr.map(Box::new),
        multispace0,
        expr.map(Box::new),
        opt(
            permutation((multispace0, tag("=>"), multispace0, expr.map(Box::new)))
                .map(|(_, _, _, a)| a),
        ),
    ))
    .map(|(name, _, _, _, iter, _, body, value)| Expr::For {
        name,
        iter,
        body,
        value,
    })
    .parse(input)
}
