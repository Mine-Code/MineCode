use nom::{
    character::complete::{anychar, multispace0},
    combinator::verify,
    multi::many0,
    sequence::delimited,
    IResult, Parser,
};

pub fn ident(input: &str) -> IResult<&str, String> {
    let (input, _) = multispace0(input)?;
    let (input, a) = verify(anychar, |x: &char| x.is_alphanumeric() || x == &'@')(input)?;
    let (input, b) = many0(verify(anychar, |x: &char| {
        x.is_alphanumeric() || x == &'@' || x == &'_'
    }))
    .map(|x| x.iter().fold("".to_string(), |a, c| a + &c.to_string()))
    .parse(input)?;
    let (input, _) = multispace0(input)?;

    let ret = a.to_string() + &b;
    Ok((input, ret))
}

pub fn symbol(ch: char) -> impl Fn(&str) -> IResult<&str, char> {
    move |input| delimited(multispace0, nom::character::complete::char(ch), multispace0)(input)
}
