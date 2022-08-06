mod bin;
mod dec;
mod hex;
mod oct;

use bin::_num_bin;
use dec::_num_dec;
use hex::_num_hex;
use nom::{branch::alt, character::complete::multispace0, sequence::delimited, IResult};
use oct::_num_oct;

use crate::ast::Expr;

pub fn _num(input: &str) -> IResult<&str, Expr> {
    delimited(
        multispace0,
        alt((_num_hex, _num_oct, _num_dec, _num_bin)),
        multispace0,
    )(input)
}
