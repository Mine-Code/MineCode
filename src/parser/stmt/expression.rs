use nom::{IResult, Parser};

use crate::{ast::Stmt, parser::expr::expr};

pub fn expression(input: &str) -> IResult<&str, Stmt> {
    expr.map(Stmt::Expression).parse(input)
}
