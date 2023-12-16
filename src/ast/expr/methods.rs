use crate::ast::{BinaryOp, Keyword, UnaryOp};

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
            Expr::As(_, b) => b.is_known_type(),

            _ => false,
        }
    }

    pub fn get_type(&self) -> Expr {
        println!("{:#}", self);
        match self {
            Expr::ApplyOperator(_, a, b) => {
                if a.get_type() == b.get_type() {
                    a.get_type()
                } else {
                    panic!("Type mismatch")
                }
            }
            Expr::As(_, b) => (**b).clone(),

            Expr::Keyword(Keyword::AnyType) => self.clone(),
            Expr::UnaryOp(UnaryOp::Reference, _) => self.clone(),

            _ => unimplemented!(),
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
