use nom::{IResult, Parser};

use crate::{ast::Stmt, parser::expr::expr};

pub fn expression(input: &str) -> IResult<&str, Stmt> {
    expr.map(Stmt::Expression).parse(input)
}
#[cfg(test)]
mod test {
    use crate::ast::Expr;

    use super::*;

    #[test]
    fn test() {
        assert_eq!(expression("1"), Ok(("", Stmt::Expression(Expr::Num(1)))));
    }
}
