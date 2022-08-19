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
        .map(|(_, a)| a))
        .map(|x| x.unwrap_or_else(|| Box::new(Expr::Nil))),
    ))
    .map(|(_, branches, fallback)| Expr::If { branches, fallback })
    .parse(input)
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_if() {
        assert_eq!(
            _if("if a => d, b => e, c => f"),
            Ok((
                "",
                Expr::If {
                    branches: vec![
                        (Expr::Ident("a".to_string()), Expr::Ident("d".to_string())),
                        (Expr::Ident("b".to_string()), Expr::Ident("e".to_string())),
                        (Expr::Ident("c".to_string()), Expr::Ident("f".to_string())),
                    ],
                    fallback: Box::new(Expr::Nil),
                }
            ))
        );
    }
    #[test]
    fn test_if_with_fallback() {
        assert_eq!(
            _if("if a => d, b => e, c => f, _ => g"),
            Ok((
                "",
                Expr::If {
                    branches: vec![
                        (Expr::Ident("a".to_string()), Expr::Ident("d".to_string())),
                        (Expr::Ident("b".to_string()), Expr::Ident("e".to_string())),
                        (Expr::Ident("c".to_string()), Expr::Ident("f".to_string())),
                    ],
                    fallback: Box::new(Expr::Ident("g".to_string())),
                }
            ))
        );
    }
}
