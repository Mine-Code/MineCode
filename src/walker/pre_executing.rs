use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

pub struct PreExecutingWalker {}

impl PreExecutingWalker {
    pub fn new() -> PreExecutingWalker {
        PreExecutingWalker {}
    }
}

impl Walker for PreExecutingWalker {
    type StmtT = Stmt;
    type ExprT = Expr;

    fn walk_stmt(stmt: Stmt) -> Self::StmtT {
        match stmt {
            Stmt::Expression(expr) => Self::walk_expr(expr),
            Stmt::LoadModule { module } => Self::walk_load_module(module),
            Stmt::FuncDef { name, args, body } => Self::walk_func_def(name, args, body),
        }
    }

    fn walk_load_module(module_name: String) -> Stmt {
        unimplemented!()
    }
    fn walk_expr(expr: Expr) -> Stmt {
        let expr = match expr {
            Expr::Num(x) => Self::walk_num(x),
            Expr::Ident(x) => Self::walk_ident(x),
            Expr::String(x) => Self::walk_string(x),
            Expr::FuncCall(x, y) => Self::walk_func_call(*x, y),

            Expr::Ranged(x, y) => Self::walk_ranged(*x, *y),
            Expr::Pointer(x) => Self::walk_pointer(*x),
            Expr::CompileTime(x) => Self::walk_compile_time(*x),
            Expr::ApplyOperator(x, y, z) => Self::walk_apply_operator(x, *y, *z),
            Expr::LogicalNot(x) => Self::walk_logical_not(*x),

            Expr::BitwiseNot(x) => Self::walk_bitwise_not(*x),
            Expr::Negative(x) => Self::walk_negative(*x),
            Expr::Subscript(x, y) => Self::walk_subscript(*x, *y),
            Expr::Attribute(x, y) => Self::walk_attribute(*x, y),
            Expr::If { branches, fallback } => Self::walk_if(branches, fallback.map(|x| *x)),
            Expr::For {
                name,
                iter,
                body,
                value,
            } => Self::walk_for(name, *iter, *body, value.map(|x| *x)),
            Expr::Exprs(x) => Self::walk_exprs(x),
        };

        Stmt::Expression(expr)
    }

    fn walk_num(num: i32) -> Expr {
        unimplemented!()
    }
    fn walk_ident(ident: String) -> Expr {
        unimplemented!()
    }
    fn walk_string(string: String) -> Expr {
        unimplemented!()
    }
    fn walk_func_call(func_name: Expr, args: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_ranged(start: Expr, end: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_pointer(expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_compile_time(expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_apply_operator(op: BinaryOp, left: Expr, right: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_logical_not(expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_bitwise_not(expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_negative(expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_subscript(expr: Expr, index: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_attribute(expr: Expr, attr: String) -> Expr {
        unimplemented!()
    }
    fn walk_if(branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_for(name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_exprs(exprs: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_func_def(name: String, args: Vec<String>, body: Expr) -> Stmt {
        unimplemented!()
    }
}
