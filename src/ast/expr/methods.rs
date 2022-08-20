use crate::ast::BinaryOp;

use super::Expr;

impl Expr {
    pub fn multiply(coefficient: i32, value: Expr) -> Expr {
        if coefficient == 0 {
            Expr::Num(0)
        } else if coefficient == 1 {
            value
        } else if value == Expr::Num(0) {
            Expr::Num(0)
        } else if value == Expr::Num(1) {
            Expr::Num(coefficient)
        } else {
            Expr::ApplyOperator(
                BinaryOp::Mul,
                Box::new(Expr::Num(coefficient)),
                Box::new(value),
            )
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_multiple_expr_coff_0() {
        let expr = Expr::multiply(0, Expr::Num(1));
        assert_eq!(expr, Expr::Num(0));
    }

    #[test]
    fn test_multiple_expr_coff_1() {
        let expr = Expr::multiply(1, Expr::Ident("a".to_string()));
        assert_eq!(expr, Expr::Ident("a".to_string()));
    }

    #[test]
    fn test_multiple_expr_coff_2() {
        let expr = Expr::multiply(2, Expr::Ident("a".to_string()));
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
