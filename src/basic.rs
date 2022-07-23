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

pub fn symbol(ch: char) -> impl Fn(&str) -> IResult<&str, char> {
    move |input| {
        let (input, _) =
            delimited(multispace0, nom::character::complete::char(ch), multispace0)(input)?;
        Ok((input, ch))
    }
}
