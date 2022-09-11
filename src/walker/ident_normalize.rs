use std::collections::HashMap;

use crate::ast::{BinaryOp, Expr, Stmt};

use super::Walker;
pub struct IdentNormalizeWalker {
    stmts: Vec<Stmt>,
    variable_replacement: HashMap<String, usize>,
    last_used_index: usize,
}

impl IdentNormalizeWalker {
    pub fn new() -> IdentNormalizeWalker {
        IdentNormalizeWalker {
            stmts: Vec::new(),
            variable_replacement: HashMap::new(),
            last_used_index: 0,
        }
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
        self.add_stmt(Stmt::LoadModule {
            module: module_name,
        });
    }
    fn walk_stmt_expr(&mut self, expr: &Expr) {
        let expr = self.walk_expr(expr);
        self.add_stmt(Stmt::Expression(expr));
    }
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: &Expr) {
        let body = self.walk_expr(body);

        self.add_stmt(Stmt::FuncDef { name, args, body });
    }
    fn walk_direct_func_call(&mut self, addr: u64, args: &[Expr]) -> Self::ExprT {
        let args = args.iter().map(|x| self.walk_expr(x)).collect();
        Expr::DirectFuncCall(addr, args)
    }

    fn walk_num(&mut self, num: i32) -> Self::ExprT {
        Expr::Num(num)
    }
    fn walk_ident(&mut self, ident: String) -> Self::ExprT {
        if !self.variable_replacement.contains_key(&ident) {
            self.last_used_index += 1;
            self.variable_replacement
                .insert(ident.clone(), self.last_used_index);
        }
        Expr::Storage(self.variable_replacement[&ident])
    }
    fn walk_storage(&mut self, index: usize) -> Self::ExprT {
        Expr::Storage(index)
    }
    fn walk_string(&mut self, string: String) -> Self::ExprT {
        Expr::String(string)
    }
    fn walk_func_call(&mut self, func_name: &Expr, args: &[Expr]) -> Self::ExprT {
        if let Expr::Subscript(array, index) = func_name {
            if **array == Expr::Ident("func".to_string()) {
                if let Expr::Num(addr) = **index {
                    return self.walk_direct_func_call(addr as u64, args);
                }
            }
        }

        let func_name = self.walk_expr(func_name);
        let args = args.iter().map(|x| self.walk_expr(x)).collect();
        Expr::FuncCall(Box::new(func_name), args)
    }
    fn walk_ranged(&mut self, start: &Expr, end: &Expr) -> Self::ExprT {
        let start = self.walk_expr(start);
        let end = self.walk_expr(end);
        Expr::Ranged(Box::new(start), Box::new(end))
    }
    fn walk_reference(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::Reference(Box::new(expr))
    }
    fn walk_dereference(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::DeReference(Box::new(expr))
    }
    fn walk_compile_time(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::CompileTime(Box::new(expr))
    }
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) -> Self::ExprT {
        let left = self.walk_expr(left);
        let right = self.walk_expr(right);
        Expr::ApplyOperator(op, Box::new(left), Box::new(right))
    }
    fn walk_logical_not(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::LogicalNot(Box::new(expr))
    }
    fn walk_bitwise_not(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::BitwiseNot(Box::new(expr))
    }
    fn walk_negative(&mut self, expr: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::Negative(Box::new(expr))
    }
    fn walk_subscript(&mut self, expr: &Expr, index: &Expr) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        let index = self.walk_expr(index);
        Expr::Subscript(Box::new(expr), Box::new(index))
    }
    fn walk_attribute(&mut self, expr: &Expr, attr: String) -> Self::ExprT {
        let expr = self.walk_expr(expr);
        Expr::Attribute(Box::new(expr), attr)
    }
    fn walk_if(&mut self, branches: &[(Expr, Expr)], fallback: &Expr) -> Self::ExprT {
        let branches = branches
            .iter()
            .map(|(cond, body)| (self.walk_expr(cond), self.walk_expr(body)))
            .collect();
        let fallback = self.walk_expr(fallback);
        Expr::If {
            branches,
            fallback: Box::new(fallback),
        }
    }
    fn walk_for(&mut self, name: String, iter: &Expr, body: &Expr, value: &Expr) -> Self::ExprT {
        let iter = self.walk_expr(iter);
        let body = self.walk_expr(body);
        let value = self.walk_expr(value);
        Expr::For {
            name,
            iter: Box::new(iter),
            body: Box::new(body),
            value: Box::new(value),
        }
    }
    fn walk_exprs(&mut self, exprs: &[Expr]) -> Self::ExprT {
        let exprs = exprs.iter().map(|x| self.walk_expr(x)).collect();
        Expr::Exprs(exprs)
    }
    fn walk_nil(&mut self) -> Self::ExprT {
        Expr::Nil
    }

    fn walk_as(&mut self, v: &Expr, t: &Expr) -> Self::ExprT {
        let v = self.walk_expr(v);
        let t = self.walk_expr(t);
        Expr::As(Box::new(v), Box::new(t))
    }

    fn walk_any_type(&mut self) -> Self::ExprT {
        Expr::AnyType
    }

    fn walk_assignment(&mut self, a: &Expr, b: &Expr) -> Self::ExprT {
        // TODO: make type hint for assignment

        let a = self.walk_expr(a);
        let b = self.walk_expr(b);
        Expr::Assignment(Box::new(a), Box::new(b))
    }
}
