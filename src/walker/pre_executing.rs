use std::collections::HashMap;

use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

struct CachedVariable {
    flushed: bool,
    value: Expr,
}

pub struct PreExecutingWalker {
    virtual_variables: HashMap<String, CachedVariable>,
}

impl PreExecutingWalker {
    pub fn new() -> PreExecutingWalker {
        PreExecutingWalker {
            virtual_variables: HashMap::new(),
        }
    }
}

impl Walker for PreExecutingWalker {
    type StmtT = Stmt;
    type ExprT = Expr;

    fn walk_stmt(&mut self, stmt: Stmt) -> Self::StmtT {
        match stmt {
            Stmt::Expression(expr) => self.walk_expr(expr),
            Stmt::LoadModule { module } => self.walk_load_module(module),
            Stmt::FuncDef { name, args, body } => self.walk_func_def(name, args, body),
        }
    }

    fn walk_load_module(&mut self, module_name: String) -> Stmt {
        unimplemented!()
    }
    fn walk_expr(&mut self, expr: Expr) -> Stmt {
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

    fn walk_num(&mut self, num: i32) -> Expr {
        unimplemented!()
    }
    fn walk_ident(&mut self, ident: String) -> Expr {
        unimplemented!()
    }
    fn walk_string(&mut self, string: String) -> Expr {
        unimplemented!()
    }
    fn walk_func_call(&mut self, func_name: Expr, args: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_ranged(&mut self, start: Expr, end: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_pointer(&mut self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_compile_time(&mut self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_apply_operator(&mut self, op: BinaryOp, left: Expr, right: Expr) -> Expr {
        if op == BinaryOp::Assignment {
            if let Expr::Ident(x) = left {
                self.virtual_variables.insert(
                    x,
                    CachedVariable {
                        flushed: false,
                        value: right,
                    },
                );
            } else {
                unimplemented!()
            }
        }
        unimplemented!()
    }
    fn walk_logical_not(&mut self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_bitwise_not(&mut self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_negative(&mut self, expr: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_subscript(&mut self, expr: Expr, index: Expr) -> Expr {
        unimplemented!()
    }
    fn walk_attribute(&mut self, expr: Expr, attr: String) -> Expr {
        unimplemented!()
    }
    fn walk_if(&mut self, branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_for(&mut self, name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_exprs(&mut self, exprs: Vec<Expr>) -> Expr {
        unimplemented!()
    }
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: Expr) -> Stmt {
        unimplemented!()
    }
}
