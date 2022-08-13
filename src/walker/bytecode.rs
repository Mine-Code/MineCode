use crate::ast::Expr;

use super::core_trait::Walker;

pub struct ByteCodeWalker {}

impl Walker<Vec<u8>, Vec<u8>> for ByteCodeWalker {
    fn walk_load_module(module_name: String) -> Vec<u8> {
        let ret = vec![0u8];
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
}
