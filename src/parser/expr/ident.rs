use nom::IResult;

use crate::{
    ast::Expr,
    parser::basic::{ident, symbol},
};

pub fn _ident(input: &str) -> IResult<&str, Expr> {
    let (mut input, mut num) = ident(input)?;

    loop {
        let ret = symbol('.')(input);
        if ret.is_err() {
            break;
        }

        let tmp = ret.unwrap();
        input = tmp.0;
        num.push('.');

        let ret = ident(input);
        if ret.is_err() {
            break;
        }
        let tmp = ret.unwrap();
        input = tmp.0;
        num.push_str(&tmp.1);
    }

    Ok((input, Expr::Ident(num)))
}
