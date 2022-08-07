use nom::branch::{alt, permutation};
use nom::bytes::complete::tag;
use nom::combinator::opt;
use nom::multi::separated_list0;
use nom::sequence::{delimited, preceded};
use nom::{IResult, Parser};

use crate::ast::Expr;
use crate::parser::basic::symbol;

use super::parser::expr;

pub fn _primary(input: &str) -> IResult<&str, Expr> {
    if input.trim().is_empty() {
        return Err(nom::Err::Error(nom::error::Error::new(
            input,
            nom::error::ErrorKind::Eof,
        )));
    }
    let parser = alt((
        preceded(symbol('-'), expr.map(Box::new)).map(Expr::Negative),
        preceded(symbol('~'), expr.map(Box::new)).map(Expr::BitwiseNot),
        preceded(symbol('!'), expr.map(Box::new)).map(Expr::LogicalNot),
        // CompileTime
        preceded(symbol('@'), expr.map(Box::new)).map(Expr::CompileTime),
        // []
        delimited(symbol('['), expr.map(Box::new), symbol(']')).map(Expr::Pointer),
        // SubExpr
        delimited(symbol('('), expr.map(Box::new), symbol(')')).map(Expr::SubExpr),
        //
        super::_num,
        super::_for,
        super::_if,
        super::_string,
        super::_ident,
        super::_exprs,
    ));

    let parser = permutation((parser, opt(delimited(symbol('['), expr, symbol(']'))))).map(
        |(array, index)| {
            if let Some(x) = index {
                Expr::Subscript(Box::new(array), Box::new(x))
            } else {
                array
            }
        },
    );

    let parser = permutation((parser, opt(preceded(tag("..."), _primary)))).map(|(begin, end)| {
        if let Some(x) = end {
            Expr::Ranged(Box::new(begin), Box::new(x))
        } else {
            begin
        }
    });
    let mut parser = permutation((
        parser,
        opt(delimited(
            symbol('('),
            separated_list0(symbol(','), expr),
            symbol(')'),
        )),
    ))
    .map(|(p, a)| {
        if let Some(x) = a {
            Expr::FuncCall(Box::new(p), x)
        } else {
            p
        }
    });

    parser.parse(input)
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_primary_plain() {
        assert_eq!(_primary("a"), Ok(("", Expr::Ident("a".to_string()))));
    }

    #[test]
    fn test_primary_negative() {
        assert_eq!(
            _primary("-a"),
            Ok(("", Expr::Negative(Box::new(Expr::Ident("a".to_string())))))
        );
    }

    #[test]
    fn test_primary_bitwise_not() {
        assert_eq!(
            _primary("~a"),
            Ok(("", Expr::BitwiseNot(Box::new(Expr::Ident("a".to_string())))))
        );
    }

    #[test]
    fn test_primary_logical_not() {
        assert_eq!(
            _primary("!a"),
            Ok(("", Expr::LogicalNot(Box::new(Expr::Ident("a".to_string())))))
        );
    }

    #[test]
    fn test_primary_compile_time() {
        assert_eq!(
            _primary("@a"),
            Ok((
                "",
                Expr::CompileTime(Box::new(Expr::Ident("a".to_string())))
            ))
        );
    }

    #[test]
    fn test_primary_pointer() {
        assert_eq!(
            _primary("[a]"),
            Ok(("", Expr::Pointer(Box::new(Expr::Ident("a".to_string())))))
        );
    }

    #[test]
    fn test_primary_sub_expr() {
        assert_eq!(
            _primary("(a)"),
            Ok(("", Expr::SubExpr(Box::new(Expr::Ident("a".to_string())))))
        );
    }

    #[test]
    fn test_primary_func_call() {
        assert_eq!(
            _primary("a(b, c)"),
            Ok((
                "",
                Expr::FuncCall(
                    Box::new(Expr::Ident("a".to_string())),
                    vec![Expr::Ident("b".to_string()), Expr::Ident("c".to_string())],
                )
            ))
        );
    }

    #[test]
    fn test_primary_subscript() {
        assert_eq!(
            _primary("a[b]"),
            Ok((
                "",
                Expr::Subscript(
                    Box::new(Expr::Ident("a".to_string())),
                    Box::new(Expr::Ident("b".to_string())),
                )
            ))
        );
    }

    #[test]
    fn test_primary_ranged() {
        assert_eq!(
            _primary("a...b"),
            Ok((
                "",
                Expr::Ranged(
                    Box::new(Expr::Ident("a".to_string())),
                    Box::new(Expr::Ident("b".to_string())),
                )
            ))
        );
    }
}
