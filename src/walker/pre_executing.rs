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

    fn walk_stmt(&self, stmt: Stmt) -> Self::StmtT {
        match stmt {
            Stmt::Expression(expr) => self.walk_expr(expr),
            Stmt::LoadModule { module } => self.walk_load_module(module),
            Stmt::FuncDef { name, args, body } => self.walk_func_def(name, args, body),
        }
    }

    fn walk_load_module(&self, module_name: String) -> Stmt {
        unimplemented!()
    }
    fn walk_expr(&self, expr: Expr) -> Stmt {
        let expr = match expr {
            Expr::Num(x) => self.walk_num(x),
            Expr::Ident(x) => self.walk_ident(x),
            Expr::String(x) => self.walk_string(x),
            Expr::FuncCall(x, y) => self.walk_func_call(*x, y),

            Expr::Ranged(x, y) => self.walk_ranged(*x, *y),
            Expr::Pointer(x) => self.walk_pointer(*x),
            Expr::CompileTime(x) => self.walk_compile_time(*x),
            Expr::ApplyOperator(x, y, z) => self.walk_apply_operator(x, *y, *z),
            Expr::LogicalNot(x) => self.walk_logical_not(*x),

            Expr::BitwiseNot(x) => self.walk_bitwise_not(*x),
            Expr::Negative(x) => self.walk_negative(*x),
            Expr::Subscript(x, y) => self.walk_subscript(*x, *y),
            Expr::Attribute(x, y) => self.walk_attribute(*x, y),
            Expr::If { branches, fallback } => self.walk_if(branches, fallback.map(|x| *x)),
            Expr::For {
                name,
                iter,
                body,
                value,
            } => self.walk_for(name, *iter, *body, value.map(|x| *x)),
            Expr::Exprs(x) => self.walk_exprs(x),
        };

        Stmt::Expression(expr)
    }

    fn walk_num(&self, num: i32) -> Expr {
        unimplemented!()
    }
    fn walk_ident(&self, ident: String) -> Expr {
        unimplemented!()
    }
    fn walk_string(&self, string: String) -> Expr {
        unimplemented!()
    }
    fn walk_func_call(&self, func_name: Expr, args: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_ranged(&self, start: Expr, end: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_pointer(&self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_compile_time(&self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_apply_operator(&self, op: BinaryOp, left: Expr, right: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_logical_not(&self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_bitwise_not(&self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_negative(&self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_subscript(&self, expr: Expr, index: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_attribute(&self, expr: Expr, attr: String) -> Expr {
        unimplemented!()
    }
    fn walk_if(&self, branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_for(&self, name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_exprs(&self, exprs: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_func_def(&self, name: String, args: Vec<String>, body: Expr) -> Stmt {
        unimplemented!()
    }
}
