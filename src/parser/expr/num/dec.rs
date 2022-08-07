use nom::{bytes::complete::take_till1, IResult};

use crate::ast::Expr;

pub fn _num_dec(input: &str) -> IResult<&str, Expr> {
    let (input, num) = take_till1(|c: char| !c.is_ascii_digit())(input)?;
    Ok((input, Expr::Num(i32::from_str_radix(num, 10).unwrap())))
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_num_dec() {
        assert_eq!(_num_dec("123"), Ok(("", Expr::Num(123))));
    }
}
