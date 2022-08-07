use crate::ast::BinaryOp;

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Num(i32),
    // Make Attribute(Expr, String)
    Ident(String),
    String(String),
    FuncCall(Box<Expr>, Vec<Expr>),

    Ranged(Box<Expr>, Box<Expr>),
    Pointer(Box<Expr>),
    CompileTime(Box<Expr>),

    ApplyOperator(BinaryOp, Box<Expr>, Box<Expr>),

    // TODO: Remove this
    SubExpr(Box<Expr>),

    LogicalNot(Box<Expr>),
    BitwiseNot(Box<Expr>),
    Negative(Box<Expr>),

    Subscript(Box<Expr>, Box<Expr>),

    If {
        branches: Vec<(Expr, Expr)>,
        fallback: Option<Box<Expr>>,
    },
    For {
        // TODO: Change this type to String
        name: Box<Expr>,
        iter: Box<Expr>,
        body: Box<Expr>,
        value: Option<Box<Expr>>,
    },

    Exprs(Vec<Expr>),
}
