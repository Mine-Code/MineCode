use nom::{bytes::complete::take_till1, IResult};

use crate::ast::Expr;
use nom::bytes::complete::tag;

pub fn _num_oct(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("0o")(input)?;
    let (input, num) = take_till1(|c: char| !c.is_digit(8))(input)?;

    Ok((input, Expr::Num(i32::from_str_radix(num, 8).unwrap())))
}
