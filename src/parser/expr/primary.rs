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
