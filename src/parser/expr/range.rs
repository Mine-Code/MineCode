use nom::{
    branch::permutation, bytes::complete::tag, combinator::opt, sequence::preceded, IResult, Parser,
};

use crate::ast::Expr;

use super::primary::_primary;

pub fn range(input: &str) -> IResult<&str, Expr> {
    permutation((_primary, opt(preceded(tag("..."), _primary))))
        .map(|(begin, end)| {
            if let Some(x) = end {
                Expr::Ranged(Box::new(begin), Box::new(x))
            } else {
                begin
            }
        })
        .parse(input)
}
