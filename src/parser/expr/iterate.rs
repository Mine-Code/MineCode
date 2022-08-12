use crate::ast::Expr;
use crate::parser::basic::ident;
use nom::branch::permutation;
use nom::bytes::complete::tag;
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::{IResult, Parser};

use super::parser::expr;

pub fn _for(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("for")(input)?;

    permutation((
        ident,
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

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_for() {
        assert_eq!(
            _for("for a in b c"),
            Ok((
                "",
                Expr::For {
                    name: Box::new(Expr::Ident("a".to_string())),
                    iter: Box::new(Expr::Ident("b".to_string())),
                    body: Box::new(Expr::Ident("c".to_string())),
                    value: None,
                }
            ))
        );
    }
    #[test]
    fn test_for_with_value() {
        assert_eq!(
            _for("for a in b c => d"),
            Ok((
                "",
                Expr::For {
                    name: Box::new(Expr::Ident("a".to_string())),
                    iter: Box::new(Expr::Ident("b".to_string())),
                    body: Box::new(Expr::Ident("c".to_string())),
                    value: Some(Box::new(Expr::Ident("d".to_string()))),
                }
            ))
        );
    }
}
