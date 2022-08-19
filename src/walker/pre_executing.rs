use std::collections::HashMap;

use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

#[derive(Debug, PartialEq)]
struct Storage {
    index: usize,
}
impl Storage {
    fn identifier(&self) -> String {
        format!("s{}", self.index)
    }
}

#[derive(Debug)]
struct CachedVariable {
    storage: Option<Storage>,
    value: Expr,
}

impl CachedVariable {
    fn get_assign_expr(&self) -> Expr {
        match &self.storage {
            Some(storage) => Expr::ApplyOperator(
                BinaryOp::Assignment,
                Box::new(Expr::Ident(format!("s{}", storage.index))),
                Box::new(self.value.clone()), // TODO: Fix this for performance
            ),
            None => panic!("Cached variable has no storage"),
        }
    }
}

pub struct PreExecutingWalker {
    virtual_variables: HashMap<String, CachedVariable>,
    last_used_storage_index: usize,
    stmts: Vec<Stmt>,
}

impl PreExecutingWalker {
    pub fn new() -> PreExecutingWalker {
        PreExecutingWalker {
            virtual_variables: HashMap::new(),
            last_used_storage_index: 0,
            stmts: Vec::new(),
        }
    }
}

impl PreExecutingWalker {
    fn expr_const_evaluative(&self, expr: &Expr) -> Option<bool> {
        match expr {
            Expr::ApplyOperator(_op, l, r) => {
                let l_const_evaluative = self.expr_const_evaluative(&**l);
                let r_const_evaluative = self.expr_const_evaluative(&**r);
                match (l_const_evaluative, r_const_evaluative) {
                    (Some(l_const_evaluative), Some(r_const_evaluative)) => {
                        Some(l_const_evaluative && r_const_evaluative)
                    }
                    _ => None,
                }
            }
            Expr::Num(_) => Some(true),
            Expr::Ident(name) => self
                .virtual_variables
                .get(name)
                .map(|v| self.expr_const_evaluative(&v.value))
                .unwrap_or(Some(false)),
            Expr::String(_) => Some(true),
            Expr::Attribute(b, _) => self.expr_const_evaluative(&**b),
            Expr::FuncCall(_b, _args) => None, // TODO: Impl this
            Expr::Ranged(s, e) => {
                let s_const_evaluative = self.expr_const_evaluative(&**s);
                let e_const_evaluative = self.expr_const_evaluative(&**e);
                match (s_const_evaluative, e_const_evaluative) {
                    (Some(s_const_evaluative), Some(e_const_evaluative)) => {
                        Some(s_const_evaluative && e_const_evaluative)
                    }
                    _ => None,
                }
            }
            Expr::Pointer(p) => self.expr_const_evaluative(&**p),
            Expr::CompileTime(_s) => Some(true),
            Expr::LogicalNot(e) => self.expr_const_evaluative(&**e),
            Expr::BitwiseNot(e) => self.expr_const_evaluative(&**e),
            Expr::Negative(e) => self.expr_const_evaluative(&**e),
            Expr::Subscript(a, i) => {
                let a_const_evaluative = self.expr_const_evaluative(&**a);
                let i_const_evaluative = self.expr_const_evaluative(&**i);
                match (a_const_evaluative, i_const_evaluative) {
                    (Some(a_const_evaluative), Some(i_const_evaluative)) => {
                        Some(a_const_evaluative && i_const_evaluative)
                    }
                    _ => None,
                }
            }
            Expr::If { branches, fallback } => {
                for branch in branches {
                    let branch_const_evaluative = self.expr_const_evaluative(&branch.0);
                    if branch_const_evaluative != Some(true) {
                        return None;
                    }

                    let value_const_evaluative = self.expr_const_evaluative(&branch.1);
                    if value_const_evaluative != Some(true) {
                        return None;
                    }
                }

                let fallback_const_evaluative = self.expr_const_evaluative(&fallback);
                if fallback_const_evaluative != Some(true) {
                    return None;
                }

                Some(true)
            }
            Expr::For {
                name: _n,
                iter,
                body,
                value,
            } => {
                let iter_const_evaluative = self.expr_const_evaluative(&**iter);
                if iter_const_evaluative != Some(true) {
                    return None;
                }

                let body_const_evaluative = self.expr_const_evaluative(&**body);
                if body_const_evaluative != Some(true) {
                    return None;
                }
                let value_const_evaluative = self.expr_const_evaluative(&*value);
                if value_const_evaluative != Some(true) {
                    return None;
                }
                Some(true)
            }

            Expr::Exprs(exprs) => {
                for expr in exprs {
                    let expr_const_evaluative = self.expr_const_evaluative(expr);
                    if expr_const_evaluative != Some(true) {
                        return None;
                    }
                }
                Some(true)
            }

            Expr::Nil => Some(true),
        }
    }

    fn resolve_identifier_from_storage(&self, storage_name: String) -> Option<String> {
        if storage_name.len() < 2 {
            return None;
        }
        let index = storage_name[1..].parse::<usize>();
        if index.is_err() {
            return None;
        }
        let index = index.unwrap();

        for (k, v) in &self.virtual_variables {
            if v.storage == Some(Storage { index: index }) {
                return Some(k.clone());
            }
        }
        None
    }

    fn resolve_identifier(&self, ident: String) -> Option<String> {
        if self.virtual_variables.contains_key(&ident) {
            return Some(ident);
        }
        self.resolve_identifier_from_storage(ident)
    }

    fn resolve_variable(&mut self, ident: String) -> Option<&mut CachedVariable> {
        let ident = self.resolve_identifier(ident);
        if ident.is_none() {
            return None;
        }
        self.virtual_variables.get_mut(&ident.unwrap())
    }
    fn get_new_storage(&mut self) -> Storage {
        let index = self.last_used_storage_index;
        self.last_used_storage_index += 1;
        Storage { index: index }
    }
}
impl Walker for PreExecutingWalker {
    type StmtT = Option<Stmt>;
    type ExprT = Option<Expr>;

    fn add_stmt(&mut self, stmt: crate::ast::Stmt) {
        self.stmts.push(stmt);
    }
    fn get_stmts(&self) -> &Vec<Stmt> {
        &self.stmts
    }

    fn walk_load_module(&mut self, _module_name: String) -> Option<Stmt> {
        unimplemented!()
    }

    fn walk_stmt_expr(&mut self, expr: &Expr) -> Option<Stmt> {
        let expr = self.walk_expr(&expr);
        if expr.is_none() {
            return None;
        }
        Some(Stmt::Expression(expr.unwrap()))
    }

    fn walk_num(&mut self, num: i32) -> Self::ExprT {
        Some(Expr::Num(num))
    }
    fn walk_ident(&mut self, ident: String) -> Self::ExprT {
        let variable = self.resolve_variable(ident);
        if variable.is_none() {
            // TODO: Error
            panic!("Undefined variable")
        }

        let variable = variable.unwrap();
        if variable.storage == None {
            let storage = self.get_new_storage();
            variable.storage = Some(storage);
            let assigner = Stmt::Expression(variable.get_assign_expr());
            self.add_stmt(assigner);
        }
        return Some(Expr::Ident(variable.storage.unwrap().identifier()));
    }
    fn walk_string(&mut self, _string: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_func_call(&mut self, _func_name: &Expr, _args: &Vec<Expr>) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_ranged(&mut self, _start: &Expr, _end: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_pointer(&mut self, _expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_compile_time(&mut self, _expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) -> Self::ExprT {
        if op == BinaryOp::Assignment {
            if let Expr::Ident(x) = left {
                let a = self.walk_expr(&right);

                self.virtual_variables.insert(
                    x.clone(),
                    CachedVariable {
                        storage: None,
                        value: if a.is_none() {
                            // TODO: Fix this for performance
                            right.clone()
                        } else {
                            a.unwrap()
                        },
                    },
                );
                return None;
            } else {
                println!("Assignment operator can only be used with identifiers");
                println!("{:?}", left);
                unimplemented!()
            }
        } else if op == BinaryOp::Add {
            let l = self.walk_expr(left);
            let r = self.walk_expr(right);
            println!("{:?}", self.virtual_variables);
            println!("+ {:?} {:?}", l, r);
            unimplemented!()
        } else if op == BinaryOp::Sub {
            let l = self.walk_expr(left);
            let r = self.walk_expr(right);
            println!("+ {:?} {:?}", l, r);
            unimplemented!()
        }
        println!(
            "{:?} {:?}[{:?}] {:?}[{:?}]",
            op,
            left,
            self.expr_const_evaluative(left),
            right,
            self.expr_const_evaluative(right)
        );
        unimplemented!()
    }
    fn walk_logical_not(&mut self, _expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_bitwise_not(&mut self, _expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_negative(&mut self, _expr: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_subscript(&mut self, _expr: &Expr, _index: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_attribute(&mut self, _expr: &Expr, _attr: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_if(&mut self, _branches: &Vec<(Expr, Expr)>, _fallback: &Expr) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_for(
        &mut self,
        _name: String,
        _iter: &Expr,
        _body: &Expr,
        _value: &Expr,
    ) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_exprs(&mut self, _exprs: &Vec<Expr>) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_func_def(&mut self, _name: String, _args: Vec<String>, _body: &Expr) -> Option<Stmt> {
        unimplemented!()
    }
    fn walk_nil(&mut self) -> Self::ExprT {
        unimplemented!()
    }
}
