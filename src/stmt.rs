use crate::basic;

use crate::expr::Primary;

use nom::character::complete::multispace0;
use nom::IResult;

#[derive(Debug)]
pub enum Stmt {
    LoadModule {
        module: String,
    },
    Expression(Primary),
    FuncDef {
        name: String,
        args: Vec<String>,
        body: Vec<Stmt>,
    },
    For {
        name: Primary,
        iter: Primary,
        body: Vec<Stmt>,
    },
}

impl Stmt {
    pub fn read(input: &str) -> IResult<&str, Stmt> {
        let tmp = basic::ident(input);

        let (input, t) = if tmp.is_err() {
            (input, "".to_string())
        } else {
            tmp.unwrap()
        };

        let a = match t.as_str() {
            "mcl" => stmt_mcl(input),
            "func" => stmt_func(input),
            "for" => stmt_for(input),
            _ => stmt_expr(input),
        };

        a
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

fn stmt_func(input: &str) -> IResult<&str, Stmt> {
    let (input, (name, args, body)) = nom::branch::permutation((
        basic::ident,
        nom::sequence::delimited(
            basic::symbol('('),
            nom::multi::separated_list0(basic::symbol(','), basic::ident),
            basic::symbol(')'),
        ),
        nom::sequence::delimited(
            basic::symbol('{'),
            nom::multi::many0(Stmt::read),
            basic::symbol('}'),
        ),
    ))(input)?;

    Ok((input, Stmt::FuncDef { name, args, body }))
}

fn stmt_for(input: &str) -> IResult<&str, Stmt> {
    let (input, (name, _, _, _, iter, body)) = nom::branch::permutation((
        Primary::read,
        multispace0,
        nom::bytes::complete::tag_no_case("in"),
        multispace0,
        Primary::read,
        nom::sequence::delimited(
            basic::symbol('{'),
            nom::multi::many0(Stmt::read),
            basic::symbol('}'),
        ),
    ))(input)?;

    Ok((input, Stmt::For { name, iter, body }))
}
