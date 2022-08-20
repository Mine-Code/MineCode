use std::collections::HashMap;

use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

pub struct PreExecutingWalker {
    virtual_variables: HashMap<usize, Expr>,
    stmts: Vec<Stmt>,
}

impl PreExecutingWalker {
    pub fn new() -> PreExecutingWalker {
        PreExecutingWalker {
            virtual_variables: HashMap::new(),
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
            Expr::Ident(name) => panic!("{} is not a constant expression", name),
            Expr::Storage(a) => self.expr_const_evaluative(&self.virtual_variables[a]),
            Expr::Attribute(b, _) => self.expr_const_evaluative(&**b),
            Expr::FuncCall(_b, _args) => None,  // TODO: Impl this
            Expr::DirectFuncCall(_, _) => None, // TODO: Impl this
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

            Expr::Nil | Expr::Num(_) | Expr::String(_) => Some(true),
        }
    }
}
impl Walker for PreExecutingWalker {
    type StmtT = Stmt;
    type ExprT = Expr;

    fn add_stmt(&mut self, stmt: Stmt) {
        if let Stmt::Expression(ref e) = stmt {
            if let Expr::Num(_) = e {
                return;
            }
        }
        self.stmts.push(stmt);
    }
    fn get_stmts(&self) -> &Vec<Stmt> {
        &self.stmts
    }

    fn walk_load_module(&mut self, _module_name: String) {
        unimplemented!()
    }
    fn walk_func_def(&mut self, _name: String, _args: Vec<String>, _body: &Expr) {
        unimplemented!()
    }

    fn walk_stmt_expr(&mut self, expr: &Expr) {
        let expr = self.walk_expr(&expr);

        self.add_stmt(Stmt::Expression(expr));
    }

    fn walk_num(&mut self, num: i32) -> Self::ExprT {
        Expr::Num(num)
    }
    fn walk_ident(&mut self, _ident: String) -> Self::ExprT {
        panic!("walk_ident is not allowed in PreExecutingWalker");
    }
    fn walk_storage(&mut self, index: usize) -> Self::ExprT {
        Expr::Storage(index)
    }
    fn walk_string(&mut self, _string: String) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_func_call(&mut self, _func_name: &Expr, _args: &[Expr]) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_direct_func_call(&mut self, addr: u64, args: &[Expr]) -> Self::ExprT {
        Expr::DirectFuncCall(addr, args.to_vec())
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
            if let Expr::Storage(storage_index) = left {
                let a = self.walk_expr(&right);

                self.virtual_variables.insert(*storage_index, a.clone());

                if self.expr_const_evaluative(&a) == Some(true) {
                    return a;
                } else {
                    self.add_stmt(Stmt::Expression(Expr::ApplyOperator(
                        BinaryOp::Assignment,
                        Box::new(left.clone()),
                        Box::new(a),
                    )));
                    return left.clone();
                }
            } else {
                println!("Assignment operator can only be used with identifiers");
                println!("{:?}", left);
                unimplemented!()
            }
        }
        let l = self.walk_expr(left);
        let l = if let Expr::Storage(x) = l {
            self.virtual_variables.get(&x).unwrap().clone()
        } else {
            l.clone()
        };

        let r = self.walk_expr(right);

        if let (Expr::Num(l), Expr::Num(r)) = (&l, &r) {
            return Expr::Num(if op == BinaryOp::Add {
                *l + *r
            } else if op == BinaryOp::Sub {
                *l - *r
            } else if op == BinaryOp::Mul {
                *l * *r
            } else if op == BinaryOp::Div {
                *l / *r
            } else if op == BinaryOp::Mod {
                *l % *r
            } else if op == BinaryOp::BitwiseOr {
                *l | *r
            } else if op == BinaryOp::BitwiseAnd {
                *l & *r
            } else if op == BinaryOp::BitwiseXor {
                *l ^ *r
            } else if op == BinaryOp::ShiftLeft {
                *l << *r
            } else if op == BinaryOp::ShiftRight {
                *l >> *r
            } else if op == BinaryOp::Power {
                if *r < 0 {
                    println!("Power operator can only be used with positive numbers");
                    println!("{:?}", right);
                    unimplemented!()
                }
                l.pow(*(r) as u32)
            } else if op == BinaryOp::LessThan {
                if *l < *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::LessThanOrEqual {
                if *l <= *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::GreaterThan {
                if *l > *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::GreaterThanOrEqual {
                if *l >= *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::Equal {
                if *l == *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::NotEqual {
                if *l != *r {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::LogicalOr {
                if *l != 0 || *r != 0 {
                    1
                } else {
                    0
                }
            } else if op == BinaryOp::LogicalAnd {
                if *l != 0 && *r != 0 {
                    1
                } else {
                    0
                }
            } else {
                println!("{:?}", self.virtual_variables);
                println!(
                    "{:?} {:?}[{:?}] {:?}[{:?}]",
                    op,
                    l,
                    self.expr_const_evaluative(left),
                    r,
                    self.expr_const_evaluative(right)
                );
                unimplemented!();
            });
        }

        println!(
            "{:?} {:?}[{:?}] {:?}[{:?}]",
            op,
            left,
            self.expr_const_evaluative(left),
            right,
            self.expr_const_evaluative(right)
        );
        unimplemented!();
        return Expr::ApplyOperator(op, Box::new(l), Box::new(r));
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
    fn walk_if(&mut self, _branches: &[(Expr, Expr)], _fallback: &Expr) -> Self::ExprT {
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
    fn walk_exprs(&mut self, _exprs: &[Expr]) -> Self::ExprT {
        unimplemented!()
    }
    fn walk_nil(&mut self) -> Self::ExprT {
        unimplemented!()
    }
}
