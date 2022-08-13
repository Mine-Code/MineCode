use crate::ast::{BinaryOp, Expr};

pub trait Walker<StmtT, ExprT> {
    fn walk_load_module(module_name: String) -> StmtT;
    fn walk_expr(expr: Expr) -> StmtT;
    fn walk_func_def(name: String, args: Vec<String>, body: Expr) -> StmtT;

    fn walk_num(num: i32) -> ExprT;
    fn walk_ident(ident: String) -> ExprT;
    fn walk_string(string: String) -> ExprT;
    fn walk_func_call(func_name: Expr, args: Vec<Expr>) -> ExprT;
    fn walk_ranged(start: Expr, end: Expr) -> ExprT;
    fn walk_pointer(expr: Expr) -> ExprT;
    fn walk_compile_time(expr: Expr) -> ExprT;
    fn walk_apply_operator(op: BinaryOp, left: Expr, right: Expr) -> ExprT;
    fn walk_logical_not(expr: Expr) -> ExprT;
    fn walk_bitwise_not(expr: Expr) -> ExprT;
    fn walk_negative(expr: Expr) -> ExprT;
    fn walk_subscript(expr: Expr, index: Expr) -> ExprT;
    fn walk_attribute(expr: Expr, attr: String) -> ExprT;
    fn walk_if(branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> ExprT;
    fn walk_for(name: String, iter: Expr, body: Expr, value: Option<Expr>) -> ExprT;
    fn walk_exprs(exprs: Vec<Expr>) -> ExprT;
}
