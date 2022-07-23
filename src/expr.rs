use nom::IResult;

#[derive(Debug)]
pub enum Expr {
    Num(i32),
}

impl Expr {
    pub fn read(input: &str) -> IResult<&str, Expr> {
        println!("Reading exp");
        Ok((input, Expr::Num(0)))
    }
}
