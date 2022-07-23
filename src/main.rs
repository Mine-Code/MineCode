extern crate nom;

mod basic;
mod expr;
mod preprocess;
mod stmt;

use stmt::Stmt;

use nom::{multi, IResult};

fn program(input: &str) -> IResult<&str, Vec<Stmt>> {
    multi::many0(Stmt::read)(input)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let prog = std::fs::read_to_string("./demo/mc2.mc2")?;
    let prog = preprocess::preprocess(prog);

    let prog = program(&prog).unwrap();

    println!("{:?}", prog.0);

    Ok(())
}
