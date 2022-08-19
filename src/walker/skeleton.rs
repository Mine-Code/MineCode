use crate::ast::{BinaryOp, Expr, Stmt};

use super::Walker;
pub struct IdentNormalizeWalker {
    stmts: Vec<Stmt>,
}

impl IdentNormalizeWalker {
    pub fn new() -> IdentNormalizeWalker {
        IdentNormalizeWalker { stmts: Vec::new() }
    }
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

    fn walk_load_module(&mut self, module_name: String) {
        unimplemented!()
    }
    fn walk_stmt_expr(&mut self, expr: &Expr) {
        unimplemented!()
    }
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: &Expr) {
        unimplemented!()
    }

    fn walk_num(&mut self, num: i32) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_ident(&mut self, ident: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_string(&mut self, string: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_func_call(&mut self, func_name: &Expr, args: &Vec<Expr>) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_ranged(&mut self, start: &Expr, end: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_pointer(&mut self, expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_compile_time(&mut self, expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_logical_not(&mut self, expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_bitwise_not(&mut self, expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_negative(&mut self, expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_subscript(&mut self, expr: &Expr, index: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_attribute(&mut self, expr: &Expr, attr: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_if(&mut self, branches: &Vec<(Expr, Expr)>, fallback: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_for(&mut self, name: String, iter: &Expr, body: &Expr, value: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_exprs(&mut self, exprs: &Vec<Expr>) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_nil(&mut self) -> Self::ExprT {
        unimplemented!()
    }
}
