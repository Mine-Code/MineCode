use crate::ast::BinaryOp;

use super::Expr;

impl Expr {
    pub fn multiply_immediate(self, coefficient: i32) -> Expr {
        if coefficient == 0 {
            Expr::Num(0)
        } else if coefficient == 1 {
            self
        } else if self == Expr::Num(0) {
            Expr::Num(0)
        } else if self == Expr::Num(1) {
            Expr::Num(coefficient)
        } else {
            Expr::ApplyOperator(
                BinaryOp::Mul,
                Box::new(Expr::Num(coefficient)),
                Box::new(self),
            )
        }
    }

    pub fn is_known_type(&self) -> bool {
        match self {
            Expr::ApplyOperator(_, a, b) if a == b => a.is_known_type(),
            Expr::ApplyOperator(_, _, _) => false,
            Expr::Num(_) => true,
            Expr::SizedNum(_, _) => true,
            Expr::Ident(_) => false,
            Expr::DirectFuncCall(_, _) => false,
            Expr::As(_, b) => b.is_known_type(),
            Expr::Assignment(_, _) => false,
            Expr::Attribute(_, _) => false,
            Expr::CompileTime(_) => false,
            Expr::Exprs(_) => false,
            Expr::For { .. } => false,
            Expr::FuncCall(_, _) => false,

            Expr::If { .. } => false,
            Expr::Keyword(_) => false,

            Expr::Ranged(_, _) => false,
            Expr::Storage(_) => false,
            Expr::String(_) => false,
            Expr::Subscript(_, _) => false,
            Expr::TypeHolder(_) => false,
            Expr::UnaryOp(_, _) => false,
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_multiple_expr_coff_0() {
        let expr = Expr::multiply_immediate(Expr::Num(1), 0);
        assert_eq!(expr, Expr::Num(0));
    }

    #[test]
    fn test_multiple_expr_coff_1() {
        let expr = Expr::multiply_immediate(Expr::Ident("a".to_string()), 1);
        assert_eq!(expr, Expr::Ident("a".to_string()));
    }

    #[test]
    fn test_multiple_expr_coff_2() {
        let expr = Expr::multiply_immediate(Expr::Ident("a".to_string()), 2);
        assert_eq!(
            expr,
            Expr::ApplyOperator(
                BinaryOp::Mul,
                Box::new(Expr::Num(2)),
                Box::new(Expr::Ident("a".to_string())),
            )
        );
    }
}
