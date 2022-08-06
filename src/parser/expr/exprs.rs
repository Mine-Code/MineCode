use nom::{multi::many0, sequence::delimited, IResult, Parser};

use crate::{ast::Expr, parser::basic::symbol};

use super::parser::expr;

pub fn _exprs(input: &str) -> IResult<&str, Expr> {
    delimited(symbol('{'), many0(expr), symbol('}'))
        .map(Expr::Exprs)
        .parse(input)
}
