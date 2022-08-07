use nom::{character::complete::char, multi::separated_list1, IResult, Parser};

use crate::{ast::Expr, parser::basic::ident};

pub fn _ident(input: &str) -> IResult<&str, Expr> {
    separated_list1(char('.'), ident)
        .map(|x| x.join("."))
        .map(Expr::Ident)
        .parse(input)
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_ident() {
        assert_eq!(_ident("a"), Ok(("", Expr::Ident("a".to_string()))));
    }

    #[test]
    fn test_ident_with_underscore() {
        assert_eq!(_ident("a_b"), Ok(("", Expr::Ident("a_b".to_string()))));
    }

    #[test]
    fn test_ident_with_first_underscore() {
        assert_eq!(_ident("_a_b").is_err(), true);
    }

    #[test]
    fn test_ident_with_single_dot() {
        assert_eq!(_ident("a.b"), Ok(("", Expr::Ident("a.b".to_string()))));
    }

    #[test]
    fn test_ident_with_double_dot() {
        assert_eq!(_ident("a.b.c"), Ok(("", Expr::Ident("a.b.c".to_string()))));
    }
    #[test]
    fn test_ident_with_triple_dot() {
        assert_eq!(
            _ident("a.b.c.d"),
            Ok(("", Expr::Ident("a.b.c.d".to_string())))
        );
    }
}
