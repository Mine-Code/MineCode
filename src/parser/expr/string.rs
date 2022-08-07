use nom::{bytes::complete::take_until, sequence::delimited, IResult};

use crate::{ast::Expr, parser::basic::symbol};

pub fn _string(input: &str) -> IResult<&str, Expr> {
    let (input, s) = delimited(symbol('"'), take_until("\""), symbol('"'))(input)?;

    Ok((input, Expr::String(s.to_string())))
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_expr_string() {
        assert_eq!(_string("\"a\""), Ok(("", Expr::String("a".to_string()))));
    }
}
