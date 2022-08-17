use crate::ast::{BinaryOp, Expr, Stmt};

pub trait Walker {
    type StmtT;
    type ExprT;

    fn walk_stmt(&mut self, stmt: Stmt) -> Self::StmtT;

    fn walk_load_module(&mut self, module_name: String) -> Self::StmtT;
    fn walk_expr(&mut self, expr: Expr) -> Self::StmtT;
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: Expr) -> Self::StmtT;

    fn walk_num(&mut self, num: i32) -> Self::ExprT;
    fn walk_ident(&mut self, ident: String) -> Self::ExprT;
    fn walk_string(&mut self, string: String) -> Self::ExprT;
    fn walk_func_call(&mut self, func_name: Expr, args: Vec<Expr>) -> Self::ExprT;
    fn walk_ranged(&mut self, start: Expr, end: Expr) -> Self::ExprT;
    fn walk_pointer(&mut self, expr: Expr) -> Self::ExprT;
    fn walk_compile_time(&mut self, expr: Expr) -> Self::ExprT;
    fn walk_apply_operator(&mut self, op: BinaryOp, left: Expr, right: Expr) -> Self::ExprT;
    fn walk_logical_not(&mut self, expr: Expr) -> Self::ExprT;
    fn walk_bitwise_not(&mut self, expr: Expr) -> Self::ExprT;
    fn walk_negative(&mut self, expr: Expr) -> Self::ExprT;
    fn walk_subscript(&mut self, expr: Expr, index: Expr) -> Self::ExprT;
    fn walk_attribute(&mut self, expr: Expr, attr: String) -> Self::ExprT;
    fn walk_if(&mut self, branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Self::ExprT;
    fn walk_for(
        &mut self,
        name: String,
        iter: Expr,
        body: Expr,
        value: Option<Expr>,
    ) -> Self::ExprT;
    fn walk_exprs(&mut self, exprs: Vec<Expr>) -> Self::ExprT;
}
