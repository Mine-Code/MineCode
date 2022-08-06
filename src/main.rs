extern crate nom;

mod ast;
// mod optimizer;
mod parser;
mod preprocess;

use ast::Stmt;
use nom::{multi::many0, IResult};

fn program(input: &str) -> IResult<&str, Vec<Stmt>> {
    many0(Stmt::read)(input)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let prog = std::fs::read_to_string("./demo/mc2.mc2")?;
    let prog = preprocess::preprocess(prog);

    let prog = program(&prog).unwrap();

    println!("{:?}", prog.0);
    println!(
        "{}",
        prog.1
            .iter()
            .map(|x| format!("{}\n", x))
            .fold("".to_string(), |a, c| a + &c)
    );

    Ok(())
}
