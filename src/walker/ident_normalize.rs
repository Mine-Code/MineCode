use crate::ast::{BinaryOp, Expr, Stmt};

use super::Walker;
pub struct IdentNormalizeWalker {
    stmts: Vec<Stmt>,
}

impl Walker for IdentNormalizeWalker {
    type StmtT = Stmt;
    type ExprT = Expr;

    fn add_stmt(&mut self, stmt: Stmt) {
        self.stmts.push(stmt);
    }

    fn get_stmts(&self) -> &Vec<Stmt> {
        &self.stmts
    }

    fn walk_load_module(&mut self, module_name: String) {}
    fn walk_stmt_expr(&mut self, expr: &Expr) {}
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: &Expr) {}

    fn walk_num(&mut self, num: i32) {}
    fn walk_ident(&mut self, ident: String) {}
    fn walk_string(&mut self, string: String) {}
    fn walk_func_call(&mut self, func_name: &Expr, args: &Vec<Expr>) {}
    fn walk_ranged(&mut self, start: &Expr, end: &Expr) {}
    fn walk_pointer(&mut self, expr: &Expr) {}
    fn walk_compile_time(&mut self, expr: &Expr) {}
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) {}
    fn walk_logical_not(&mut self, expr: &Expr) {}
    fn walk_bitwise_not(&mut self, expr: &Expr) {}
    fn walk_negative(&mut self, expr: &Expr) {}
    fn walk_subscript(&mut self, expr: &Expr, index: &Expr) {}
    fn walk_attribute(&mut self, expr: &Expr, attr: String) {}
    fn walk_if(&mut self, branches: &Vec<(Expr, Expr)>, fallback: &Expr) {}
    fn walk_for(&mut self, name: String, iter: &Expr, body: &Expr, value: &Expr) {}
    fn walk_exprs(&mut self, exprs: &Vec<Expr>) {}
    fn walk_nil(&mut self) {}
}
