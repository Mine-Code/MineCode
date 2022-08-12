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

// TODO: Write test
