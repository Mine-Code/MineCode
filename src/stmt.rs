use crate::basic;

use crate::expr::Expr;

use nom::branch::permutation;
use nom::bytes::complete::tag;

use nom::combinator::opt;
use nom::multi::separated_list0;
use nom::sequence::delimited;
use nom::{IResult, Parser};

#[derive(Debug)]
pub enum Stmt {
    LoadModule {
        module: String,
    },
    Expression(Expr),
    FuncDef {
        name: String,
        args: Vec<String>,
        body: Box<Stmt>,
    },
}

impl std::fmt::Display for Stmt {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::LoadModule { module } => write!(f, "Load {}", module),
            Self::Expression(expr) => write!(f, "{}", expr),
            Self::FuncDef { name, args, body } => {
                write!(f, "def {}({}) {}", name, args.join(","), body)
            }
        }
    }
}

impl Stmt {
    pub fn read(input: &str) -> IResult<&str, Stmt> {
        let tmp = basic::ident(input);

        let (sub_input, t) = if tmp.is_err() {
            (input, "".to_string())
        } else {
            tmp?
        };

        let stmt = match t.as_str() {
            "mcl" => stmt_mcl(sub_input),
            "fn" => stmt_func(sub_input),
            _ => stmt_expr(input),
        };
        let (i, stmt) = stmt?;
        let (i, _) = opt(tag(";"))(i)?;
        Ok((i, stmt))
    }
}

fn stmt_mcl(input: &str) -> IResult<&str, Stmt> {
    let (input, name) = basic::ident(input)?;
    Ok((input, Stmt::LoadModule { module: name }))
}

fn stmt_expr(input: &str) -> IResult<&str, Stmt> {
    let (input, expr) = Expr::read(input)?;

    Ok((input, Stmt::Expression(expr)))
}

fn stmt_func(input: &str) -> IResult<&str, Stmt> {
    permutation((
        basic::ident,
        delimited(
            basic::symbol('('),
            separated_list0(basic::symbol(','), basic::ident),
            basic::symbol(')'),
        ),
        Stmt::read.map(Box::new),
    ))
    .map(|(name, args, body)| Stmt::FuncDef { name, args, body })
    .parse(input)
}
