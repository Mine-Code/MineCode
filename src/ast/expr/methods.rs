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
