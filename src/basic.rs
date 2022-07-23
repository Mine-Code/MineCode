use nom::{character::complete::multispace0, multi, sequence::delimited, IResult};

pub fn ident(input: &str) -> IResult<&str, String> {
    let (input, module) = delimited(
        multispace0,
        multi::many1(nom::character::complete::one_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789@",
        )),
        multispace0,
    )(input)?;

    Ok((input, module.into_iter().collect()))
}
