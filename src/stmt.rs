use crate::basic::ident;

use crate::expr::Expr;

use nom::IResult;

#[derive(Debug)]
pub enum Stmt {
    LoadModule { module: String },
    Expression(Expr),
}

impl Stmt {
    pub fn read(input: &str) -> IResult<&str, Stmt> {
        let (input, t) = ident(input)?;

        let a = match t.as_str() {
            "mcl" => stmt_mcl(input),
            _ => stmt_expr(input),
        };

        let a = a.unwrap();
        println!("{:?}", a.1);

        return Ok(a);
    }
}

fn stmt_mcl(input: &str) -> IResult<&str, Stmt> {
    let (input, name) = ident(input)?;
    Ok((input, Stmt::LoadModule { module: name }))
}

fn stmt_expr(input: &str) -> IResult<&str, Stmt> {
    let (input, expr) = Expr::read(input)?;
    Ok((input, Stmt::Expression(expr)))
}
