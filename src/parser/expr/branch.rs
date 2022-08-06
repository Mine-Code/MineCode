use nom::{
    branch::permutation, bytes::complete::tag, character::complete::multispace0, combinator::opt,
    multi::separated_list1, IResult, Parser,
};

use super::parser::expr;
use crate::{ast::Expr, parser::basic::symbol};

pub fn _if(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("if")(input)?;

    permutation((
        multispace0,
        separated_list1(
            tag(","),
            permutation((expr, multispace0, tag("=>"), multispace0, expr))
                .map(|(a, _, _, _, c)| (a, c)),
        ),
        opt(permutation((
            permutation((
                symbol(','),
                symbol('_'),
                multispace0,
                tag("=>"),
                multispace0,
            )),
            expr.map(Box::new),
        ))
        .map(|(_, a)| a)),
    ))
    .map(|(_, branches, fallback)| Expr::If { branches, fallback })
    .parse(input)
}
