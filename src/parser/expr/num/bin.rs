use nom::{
    bytes::{complete::tag, complete::take_till1},
    IResult,
};

use crate::ast::Expr;

pub fn _num_bin(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("0b")(input)?;
    let (input, num) = take_till1(|c: char| !c.is_digit(2))(input)?;

    Ok((input, Expr::Num(i32::from_str_radix(num, 2).unwrap())))
}
