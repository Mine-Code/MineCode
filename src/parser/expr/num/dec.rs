use nom::{bytes::complete::take_till1, IResult};

use crate::ast::Expr;

pub fn _num_dec(input: &str) -> IResult<&str, Expr> {
    let (input, num) = take_till1(|c: char| !c.is_ascii_digit())(input)?;
    Ok((input, Expr::Num(num.parse::<i32>().unwrap())))
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_num_dec() {
        assert_eq!(_num_dec("123"), Ok(("", Expr::Num(123))));
    }
    #[test]
    fn test_a() {
        assert_eq!(_num_dec("0 as u32"), Ok((" as u32", Expr::Num(0))));
    }
}
