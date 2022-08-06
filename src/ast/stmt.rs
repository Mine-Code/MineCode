use super::Expr;

#[derive(Debug, Clone)]
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
