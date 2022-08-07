use nom::bytes::complete::{tag, take_till1};
use nom::IResult;

use crate::ast::Expr;

pub fn _num_hex(input: &str) -> IResult<&str, Expr> {
    let (input, _) = tag("0x")(input)?;
    let (input, num) = take_till1(|c: char| !c.is_ascii_hexdigit())(input)?;

    Ok((input, Expr::Num(i32::from_str_radix(num, 16).unwrap())))
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_num_hex() {
        assert_eq!(_num_hex("0x123"), Ok(("", Expr::Num(0x123))));
    }
}
