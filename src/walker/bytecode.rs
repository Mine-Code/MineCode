use crate::ast::{BinaryOp, Expr};

use super::core_trait::Walker;

pub struct ByteCodeWalker {}

impl Walker<Vec<u8>, Vec<u8>> for ByteCodeWalker {
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
        unimplemented!()
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
        unimplemented!()
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
