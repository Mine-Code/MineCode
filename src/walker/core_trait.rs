use crate::ast::{BinaryOp, Expr, Stmt};

pub trait Walker {
    type StmtT;
    type ExprT;

    fn walk_stmt(&self, stmt: Stmt) -> Self::StmtT;

    fn walk_load_module(&self, module_name: String) -> Self::StmtT;
    fn walk_expr(&self, expr: Expr) -> Self::StmtT;
    fn walk_func_def(&self, name: String, args: Vec<String>, body: Expr) -> Self::StmtT;

    fn walk_num(&self, num: i32) -> Self::ExprT;
    fn walk_ident(&self, ident: String) -> Self::ExprT;
    fn walk_string(&self, string: String) -> Self::ExprT;
    fn walk_func_call(&self, func_name: Expr, args: Vec<Expr>) -> Self::ExprT;
    fn walk_ranged(&self, start: Expr, end: Expr) -> Self::ExprT;
    fn walk_pointer(&self, expr: Expr) -> Self::ExprT;
    fn walk_compile_time(&self, expr: Expr) -> Self::ExprT;
    fn walk_apply_operator(&self, op: BinaryOp, left: Expr, right: Expr) -> Self::ExprT;
    fn walk_logical_not(&self, expr: Expr) -> Self::ExprT;
    fn walk_bitwise_not(&self, expr: Expr) -> Self::ExprT;
    fn walk_negative(&self, expr: Expr) -> Self::ExprT;
    fn walk_subscript(&self, expr: Expr, index: Expr) -> Self::ExprT;
    fn walk_attribute(&self, expr: Expr, attr: String) -> Self::ExprT;
    fn walk_if(&self, branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Self::ExprT;
    fn walk_for(&self, name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Self::ExprT;
    fn walk_exprs(&self, exprs: Vec<Expr>) -> Self::ExprT;
}
