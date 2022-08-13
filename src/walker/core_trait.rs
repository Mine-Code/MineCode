use crate::ast::{BinaryOp, Expr, Stmt};

pub trait Walker {
    type StmtT;
    type ExprT;

    fn walk_stmt(stmt: Stmt) -> Self::StmtT;

    fn walk_load_module(module_name: String) -> Self::StmtT;
    fn walk_expr(expr: Expr) -> Self::StmtT;
    fn walk_func_def(name: String, args: Vec<String>, body: Expr) -> Self::StmtT;

    fn walk_num(num: i32) -> Self::ExprT;
    fn walk_ident(ident: String) -> Self::ExprT;
    fn walk_string(string: String) -> Self::ExprT;
    fn walk_func_call(func_name: Expr, args: Vec<Expr>) -> Self::ExprT;
    fn walk_ranged(start: Expr, end: Expr) -> Self::ExprT;
    fn walk_pointer(expr: Expr) -> Self::ExprT;
    fn walk_compile_time(expr: Expr) -> Self::ExprT;
    fn walk_apply_operator(op: BinaryOp, left: Expr, right: Expr) -> Self::ExprT;
    fn walk_logical_not(expr: Expr) -> Self::ExprT;
    fn walk_bitwise_not(expr: Expr) -> Self::ExprT;
    fn walk_negative(expr: Expr) -> Self::ExprT;
    fn walk_subscript(expr: Expr, index: Expr) -> Self::ExprT;
    fn walk_attribute(expr: Expr, attr: String) -> Self::ExprT;
    fn walk_if(branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Self::ExprT;
    fn walk_for(name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Self::ExprT;
    fn walk_exprs(exprs: Vec<Expr>) -> Self::ExprT;
}
