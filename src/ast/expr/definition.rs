use crate::ast::BinaryOp;

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Num(i32),

    Attribute(Box<Expr>, String),

    Ident(String),
    Storage(usize),
    String(String),

    FuncCall(Box<Expr>, Vec<Expr>),

    Ranged(Box<Expr>, Box<Expr>),
    Pointer(Box<Expr>),
    CompileTime(Box<Expr>),

    ApplyOperator(BinaryOp, Box<Expr>, Box<Expr>),

    LogicalNot(Box<Expr>),
    BitwiseNot(Box<Expr>),
    Negative(Box<Expr>),

    Subscript(Box<Expr>, Box<Expr>),

    If {
        branches: Vec<(Expr, Expr)>,
        fallback: Box<Expr>,
    },
    For {
        name: String,
        iter: Box<Expr>,
        body: Box<Expr>,
        value: Box<Expr>,
    },

    Exprs(Vec<Expr>),
    Nil,
}
