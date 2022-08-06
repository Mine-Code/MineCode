use nom::{bytes::complete::take_until, sequence::delimited, IResult};

use crate::{ast::Expr, parser::basic::symbol};

pub fn _string(input: &str) -> IResult<&str, Expr> {
    let (input, s) = delimited(symbol('"'), take_until("\""), symbol('"'))(input)?;

    Ok((input, Expr::String(s.to_string())))
}
