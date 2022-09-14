use crate::ast::{keyword::Keyword, BinaryOp};

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Num(i32),
    SizedNum(i32, u32),
    Ident(String),
    Storage(usize),
    String(String),
    FuncCall(Box<Expr>, Vec<Expr>),
    DirectFuncCall(u64, Vec<Expr>),

    Attribute(Box<Expr>, String),

    Ranged(Box<Expr>, Box<Expr>),
    CompileTime(Box<Expr>),

    As(Box<Expr>, Box<Expr>),
    Reference(Box<Expr>),
    DeReference(Box<Expr>),

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
    TypeHolder(usize), // Unknown type

    Assignment(Box<Expr>, Box<Expr>),
    Keyword(Keyword),
}
