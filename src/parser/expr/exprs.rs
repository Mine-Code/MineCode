use nom::{multi::many0, sequence::delimited, IResult, Parser};

use crate::{ast::Expr, parser::basic::symbol};

use super::parser::expr;

pub fn _exprs(input: &str) -> IResult<&str, Expr> {
    delimited(symbol('{'), many0(expr), symbol('}'))
        .map(Expr::Exprs)
        .parse(input)
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_exprs() {
        assert_eq!(
            _exprs("{a; b; c}"),
            Ok((
                "",
                Expr::Exprs(vec![
                    Expr::Ident("a".to_string()),
                    Expr::Ident("b".to_string()),
                    Expr::Ident("c".to_string()),
                ])
            ))
        );
    }
}
