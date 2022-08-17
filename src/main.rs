extern crate nom;

mod ast;
mod optimizer;
mod parser;
mod preprocess;
mod walker;

use ast::Stmt;
use nom::{multi::many0, IResult};

use crate::walker::ByteCodeWalker;
use crate::walker::PreExecutingWalker;
use crate::walker::Walker;

fn program(input: &str) -> IResult<&str, Vec<Stmt>> {
    many0(parser::stmt)(input)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let prog = std::fs::read_to_string("./demo/mc2.mc2")?;
    let prog = preprocess::preprocess(prog);

    let (remained_src, stmts) = program(&prog).unwrap();

    let mut pre_executing_walker = PreExecutingWalker::new();
    let mut byte_code_walker = ByteCodeWalker::new();

    let stmts = stmts.iter().cloned();
    let stmts = stmts.map(|x| x.optimize());
    let stmts = stmts.map(|x| pre_executing_walker.walk_stmt(x));
    let stmts = stmts.map(|x| byte_code_walker.walk_stmt(x)).map(|x| {
        x.iter()
            .map(|x| format!("{:02x}", x))
            .collect::<Vec<_>>()
            .join(" ")
    });

    println!("{:?}", remained_src);
    println!(
        "{}",
        stmts
            .map(|x| x.to_string() + "\n")
            .fold("".to_string(), |a, c| a + &c)
    );

    Ok(())
}
