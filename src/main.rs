extern crate nom;

mod ast;
mod optimizer;
mod parser;
mod preprocess;
mod walker;

use ast::Stmt;
use nom::{multi::many0, IResult};

use crate::walker::ByteCodeWalker;
use crate::walker::Walker;

fn program(input: &str) -> IResult<&str, Vec<Stmt>> {
    many0(parser::stmt)(input)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let prog = std::fs::read_to_string("./demo/mc2.mc2")?;
    let prog = preprocess::preprocess(prog);

    let (remained_src, stmts) = program(&prog).unwrap();

    println!("{:?}", remained_src);
    println!(
        "{}",
        stmts
            .iter()
            .cloned()
            .map(|x| x.optimize())
            .map(|x| ByteCodeWalker::walk_stmt(x))
            .map(|x| format!("{:?}\n", x))
            .fold("".to_string(), |a, c| a + &c)
    );

    Ok(())
}
