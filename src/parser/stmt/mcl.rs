use crate::{ast::Stmt, parser::basic::ident};
use nom::bytes::complete::tag;
use nom::Parser;
use nom::{branch::permutation, character::complete::multispace0, IResult};

pub fn mcl(input: &str) -> IResult<&str, Stmt> {
    permutation((multispace0, tag("mcl"), multispace0, ident))
        .map(|(_, _, _, name)| Stmt::LoadModule { module: name })
        .parse(input)
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test() {
        assert_eq!(
            mcl("mcl test"),
            Ok((
                "",
                Stmt::LoadModule {
                    module: "test".to_string()
                }
            ))
        );
    }
}
