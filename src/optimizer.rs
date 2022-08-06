use crate::ast::{BinaryOp, Expr, Stmt};

pub trait Optimize {
    fn optimize(&self) -> Self;
}

impl Optimize for Stmt {
    fn optimize(&self) -> Self {
        match self {
            Stmt::Expression(expr) => Stmt::Expression(expr.optimize()),
            _ => (*self).clone(),
        }
    }
}
impl Expr {
    fn _collect_terms<'a>(&'a self) -> Vec<(bool, &'a Expr)> {
        if let Expr::ApplyOperator(op, r, l) = self {
            if *op == BinaryOp::Add || *op == BinaryOp::Sub {
                let mut terms = vec![];
                terms.extend(l._collect_terms());
                terms.extend(r._collect_terms());
                terms
            } else {
                vec![(true, self)]
            }
        } else {
            vec![(true, self)]
        }
    }
    fn _collect_factors<'a>(&'a self) -> Vec<(bool, &'a Expr)> {
        if let Expr::ApplyOperator(op, r, l) = self {
            if *op == BinaryOp::Mul || *op == BinaryOp::Div {
                let mut factors = vec![];
                factors.extend(l._collect_factors());
                factors.extend(r._collect_factors());
                factors
            } else {
                vec![(true, self)]
            }
        } else {
            vec![(true, self)]
        }
    }
}

impl Optimize for Expr {
    fn optimize(&self) -> Self {
        println!("{}", self);
        if let Expr::ApplyOperator(op, r, l) = self {
            let r = r.optimize();
            let l = l.optimize();
            let expr = if let Self::Num(x) = r {
                if let Self::Num(y) = l {
                    println!("{self} => {}", op.calculate(x, y));
                    Self::Num(op.calculate(x, y))
                } else {
                    Expr::ApplyOperator((*op).clone(), Box::new(r), Box::new(l))
                }
            } else {
                Expr::ApplyOperator((*op).clone(), Box::new(r), Box::new(l))
            };

            if *op == BinaryOp::Add || *op == BinaryOp::Sub {
                let terms = expr._collect_terms();
            } else {
                expr
            }

            return expr;
        }

        (*self).clone()
    }
}

pub fn optimize<T: Optimize>(val: T) -> T {
    val.optimize()
}
