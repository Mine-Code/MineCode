extern crate nom;

mod ast;
mod optimizer;
mod parser;
mod preprocess;
mod walker;

use std::collections::HashMap;

use ast::Stmt;
use nom::{multi::many0, IResult};

use crate::walker::Walker;

enum Expr {
    Num,
}

struct Scope {
    variables: HashMap<String, Expr>,
}

struct MineCodeRuntime {
    root_scope: Scope,
}

struct MineCodeInterpreter;

fn program(input: &str) -> IResult<&str, Vec<Stmt>> {
    many0(parser::stmt)(input)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let prog = std::fs::read_to_string("./demo/mc2.mc2")?;
    let prog = preprocess::preprocess(prog);

    let (remained_src, stmts) = program(&prog).unwrap();

    let mut stmts = stmts;
    for stmt in stmts.iter_mut() {
        stmt.optimize();
    }

    println!("{:?}", stmts);

    let runtime = MineCodeRuntime {
        root_scope: Scope {
            variables: {
                let mut map = HashMap::new();
                map.insert("a".to_string(), Expr::Num);
                map
            },
        },
    };

    let (variable_replacement, stmts) = {
        let mut walker = walker::IdentNormalizeWalker::new();
        (
            walker.variable_replacement.clone(),
            walker.walk(stmts).clone(),
        )
    };

    // let mut walker = walker::PreExecutingWalker::new();
    // let stmts = walker.walk_ref(stmts);

    // let mut byte_code_walker = ByteCodeWalker::new();
    // let stmts = byte_code_walker.walk_ref(stmts).iter().map(|x| {
    //     x.iter()
    //         .map(|x| format!("{:02x}", x))
    //         .collect::<Vec<_>>()
    //         .join(" ")
    // });

    println!("{:?}", remained_src);
    println!("Stmts: ");
    println!(
        "{}",
        stmts
            .iter()
            .map(|x| x.to_string() + "\n")
            .fold("".to_string(), |a, c| a + &c)
    );

    Ok(())
}
