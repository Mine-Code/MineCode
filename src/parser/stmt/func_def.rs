use crate::{
    ast::Stmt,
    parser::{
        basic::{ident, symbol},
        expr::expr,
    },
};
use nom::bytes::complete::tag;
use nom::{branch::permutation, multi::separated_list0, sequence::delimited, IResult};
use nom::{character::complete::multispace0, Parser};

pub fn func_def(input: &str) -> IResult<&str, Stmt> {
    let (input, _) = permutation((multispace0, tag("fn"), multispace0))(input)?;
    permutation((
        ident,
        delimited(
            symbol('('),
            separated_list0(symbol(','), ident),
            symbol(')'),
        ),
        expr,
    ))
    .map(|(name, args, body)| Stmt::FuncDef { name, args, body })
    .parse(input)
}

#[cfg(test)]
mod test {

    use crate::ast::Expr;

    use super::*;

    #[test]
    fn test() {
        assert_eq!(
            func_def("fn test() { a; b; c }"),
            Ok((
                "",
                Stmt::FuncDef {
                    name: "test".to_string(),
                    args: vec![],
                    body: Expr::Exprs(vec![
                        Expr::Ident("a".to_string()),
                        Expr::Ident("b".to_string()),
                        Expr::Ident("c".to_string()),
                    ])
                }
            ))
        );
    }
}
