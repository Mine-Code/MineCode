use crate::ast::{Expr, UnaryOp};
use crate::parser::basic::{ident, symbol};
use nom::branch::alt;
use nom::bytes::complete::tag;
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::multi::separated_list0;
use nom::sequence::{delimited, preceded};
use nom::{IResult, Parser};

use super::parser::expr;

pub fn _primary(input: &str) -> IResult<&str, Expr> {
    if input.trim().is_empty() {
        return Err(nom::Err::Error(nom::error::Error::new(
            input,
            nom::error::ErrorKind::Eof,
        )));
    }

    let (mut input, mut ret) = alt((
        preceded(symbol('-'), _primary.map(Box::new)).map(|x| Expr::UnaryOp(UnaryOp::Negative, x)),
        preceded(symbol('~'), _primary.map(Box::new))
            .map(|x| Expr::UnaryOp(UnaryOp::BitwiseNot, x)),
        preceded(symbol('!'), _primary.map(Box::new))
            .map(|x| Expr::UnaryOp(UnaryOp::LogicalNot, x)),
        // CompileTime
        preceded(symbol('@'), _primary.map(Box::new)).map(Expr::CompileTime),
        // []
        delimited(symbol('['), expr.map(Box::new), symbol(']')).map(|x| {
            Expr::UnaryOp(
                UnaryOp::DeReference,
                Box::new(Expr::As(
                    x,
                    Box::new(Expr::UnaryOp(
                        UnaryOp::Reference,
                        Box::new(Expr::Keyword(crate::ast::Keyword::AnyType)),
                    )),
                )),
            )
        }),
        // SubExpr
        delimited(symbol('('), expr, symbol(')')),
        //
        super::_num,
        super::_for,
        super::_if,
        super::_string,
        super::_ident,
        super::_exprs,
    ))
    .parse(input)?;

    loop {
        let a: Option<Expr>;
        (input, a) = opt(delimited(symbol('['), expr, symbol(']')))(input)?;
        if let Some(index) = a {
            ret = Expr::Subscript(Box::new(ret), Box::new(index));
            continue;
        }

        let a: Option<String>;
        (input, a) = opt(preceded(symbol('.'), ident))(input)?;
        if let Some(name) = a {
            ret = Expr::Attribute(Box::new(ret), name);
            continue;
        }

        let a: Option<Expr>;
        (input, a) = opt(preceded(
            delimited(multispace0, tag("as"), multispace0),
            expr,
        ))(input)?;
        if let Some(t) = a {
            ret = Expr::As(Box::new(ret), Box::new(t));
            continue;
        }

        let a: Option<Vec<Expr>>;
        (input, a) = opt(delimited(
            symbol('('),
            separated_list0(symbol(','), expr),
            symbol(')'),
        ))(input)?;

        if let Some(args) = a {
            ret = Expr::FuncCall(Box::new(ret), args);
            continue;
        }
        break;
    }

    let (input, range_end) = opt(preceded(tag("..."), _primary))(input)?;
    if let Some(end) = range_end {
        ret = Expr::Ranged(Box::new(ret), Box::new(end));
    }

    Ok((input, ret))
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
            Ok((
                "",
                Expr::UnaryOp(UnaryOp::Negative, Box::new(Expr::Ident("a".to_string())))
            ))
        );
    }

    #[test]
    fn test_primary_bitwise_not() {
        assert_eq!(
            _primary("~a"),
            Ok((
                "",
                Expr::UnaryOp(UnaryOp::BitwiseNot, Box::new(Expr::Ident("a".to_string())))
            ))
        );
    }

    #[test]
    fn test_primary_logical_not() {
        assert_eq!(
            _primary("!a"),
            Ok((
                "",
                Expr::UnaryOp(UnaryOp::LogicalNot, Box::new(Expr::Ident("a".to_string())))
            ))
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
            Ok((
                "",
                Expr::UnaryOp(
                    UnaryOp::DeReference,
                    Box::new(Expr::As(
                        Box::new(Expr::Ident("a".to_string())),
                        Box::new(Expr::UnaryOp(
                            UnaryOp::Reference,
                            Box::new(Expr::Keyword(crate::ast::Keyword::Nil))
                        )),
                    ))
                )
            ))
        );
    }

    #[test]
    fn test_primary_sub_expr() {
        assert_eq!(_primary("(a)"), Ok(("", Expr::Ident("a".to_string()))));
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
