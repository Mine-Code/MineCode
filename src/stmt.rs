use crate::basic;

use crate::expr::Primary;

use nom::branch::permutation;
use nom::bytes::complete::tag;
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::multi::{many0, separated_list0};
use nom::sequence::delimited;
use nom::{IResult, Parser};

#[derive(Debug)]
pub enum Stmt {
    LoadModule {
        module: String,
    },
    Expression(Primary),
    FuncDef {
        name: String,
        args: Vec<String>,
        body: Box<Stmt>,
    },
    For {
        name: Primary,
        iter: Primary,
        body: Box<Stmt>,
    },
    Stmts(Vec<Stmt>),
}

impl std::fmt::Display for Stmt {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::LoadModule { module } => write!(f, "Load {}", module),
            Self::Expression(expr) => write!(f, "{}", expr),
            Self::FuncDef { name, args, body } => {
                write!(f, "def {}({}) {}", name, args.join(","), body)
            }
            Self::For { name, iter, body } => {
                write!(f, "for {} in {} {}", name, iter, body)
            }
            Self::Stmts(body) => write!(
                f,
                "{{{}}}",
                body.iter()
                    .map(|x| format!("{}; ", x))
                    .fold("".to_string(), |a, c| a + &c)
            ),
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
            "for" => stmt_for(sub_input),
            _ => {
                if input.trim().starts_with("{") {
                    stmt_stmts(input)
                } else {
                    stmt_expr(input)
                }
            }
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
    let (input, expr) = Primary::read(input)?;

    Ok((input, Stmt::Expression(expr)))
}
pub fn stmt_stmts(input: &str) -> IResult<&str, Stmt> {
    delimited(basic::symbol('{'), many0(Stmt::read), basic::symbol('}'))
        .map(|x| Stmt::Stmts(x))
        .parse(input)
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

fn stmt_for(input: &str) -> IResult<&str, Stmt> {
    permutation((
        Primary::read,
        multispace0,
        tag("in"),
        multispace0,
        Primary::read,
        Stmt::read.map(Box::new),
    ))
    .map(|(name, _, _, _, iter, body)| Stmt::For { name, iter, body })
    .parse(input)
}
