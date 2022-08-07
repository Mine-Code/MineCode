use crate::ast::Expr;

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    LoadModule {
        module: String,
    },
    Expression(Expr),
    FuncDef {
        name: String,
        args: Vec<String>,
        // Change this type to Expr
        body: Box<Stmt>,
    },
}
