use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

pub struct ByteCodeWalker {}

impl ByteCodeWalker {
    pub fn new() -> ByteCodeWalker {
        ByteCodeWalker {}
    }
}

impl Walker for ByteCodeWalker {
    type StmtT = Vec<u8>;
    type ExprT = Vec<u8>;

    fn walk_stmt(stmt: Stmt) -> Self::StmtT {
        match stmt {
            Stmt::Expression(expr) => Self::walk_expr(expr),
            Stmt::LoadModule { module } => Self::walk_load_module(module),
            Stmt::FuncDef { name, args, body } => Self::walk_func_def(name, args, body),
        }
    }

    fn walk_load_module(module_name: String) -> Vec<u8> {
        let mut ret = vec![0u8];
        ret.extend(module_name.as_bytes().to_vec());
        ret
    }
    fn walk_expr(expr: Expr) -> Vec<u8> {
        match expr {
            Expr::Num(x) => Self::walk_num(x),
            Expr::Ident(x) => Self::walk_ident(x),
            Expr::String(x) => Self::walk_string(x),
            Expr::FuncCall(x, y) => Self::walk_func_call(*x, y),

            Expr::Ranged(x, y) => Self::walk_ranged(*x, *y),
            Expr::Pointer(x) => Self::walk_pointer(*x),
            Expr::CompileTime(x) => Self::walk_compile_time(*x),
            Expr::ApplyOperator(x, y, z) => Self::walk_apply_operator(x, *y, *z),
            Expr::LogicalNot(x) => Self::walk_logical_not(*x),

            Expr::BitwiseNot(x) => Self::walk_bitwise_not(*x),
            Expr::Negative(x) => Self::walk_negative(*x),
            Expr::Subscript(x, y) => Self::walk_subscript(*x, *y),
            Expr::Attribute(x, y) => Self::walk_attribute(*x, y),
            Expr::If { branches, fallback } => Self::walk_if(branches, fallback.map(|x| *x)),
            Expr::For {
                name,
                iter,
                body,
                value,
            } => Self::walk_for(name, *iter, *body, value.map(|x| *x)),
            Expr::Exprs(x) => Self::walk_exprs(x),
        }
    }

    fn walk_num(num: i32) -> Vec<u8> {
        let mut ret = vec![];
        if num < 0x100 {
            ret.push(0x90u8);
            ret.push(num as u8);
        } else if num < 0x10000 {
            ret.push(0x91u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);
        } else
        /* if num < 0x100000000 */
        {
            ret.push(0x92u8);
            ret.push((num >> 24) as u8);
            ret.push((num >> 16) as u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);
        }
        /*  else  if num < 0x10000000000000000{
            ret.push(0x93u8);
            ret.push((num >> 56) as u8);
            ret.push((num >> 48) as u8);
            ret.push((num >> 40) as u8);
            ret.push((num >> 32) as u8);
            ret.push((num >> 24) as u8);
            ret.push((num >> 16) as u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);

        } */
        ret
    }
    fn walk_ident(ident: String) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_string(string: String) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_func_call(func_name: Expr, args: Vec<Expr>) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_ranged(start: Expr, end: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_pointer(expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_compile_time(expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_apply_operator(op: BinaryOp, left: Expr, right: Expr) -> Vec<u8> {
        let mut ret = vec![op.into()];
        ret.extend(Self::walk_expr(left));
        ret.extend(Self::walk_expr(right));
        ret
    }
    fn walk_logical_not(expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_bitwise_not(expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_negative(expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_subscript(expr: Expr, index: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_attribute(expr: Expr, attr: String) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_if(branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_for(name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_exprs(exprs: Vec<Expr>) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_func_def(name: String, args: Vec<String>, body: Expr) -> Vec<u8> {
        unimplemented!()
    }
}
