use crate::ast::{BinaryOp, Expr};

use super::{addsub, term};

impl Expr {
    pub fn optimize(&self) -> Self {
        if let Expr::ApplyOperator(op, _, _) = self {
            if *op == BinaryOp::Add || *op == BinaryOp::Sub {
                return addsub::optimize_terms(self);
            }

            if *op == BinaryOp::Mul || *op == BinaryOp::Div {
                return term::optimize_terms(self);
            }
        }

        (*self).clone()
    }
}
