use crate::ast::{BinaryOp, Expr};

impl Expr {
    fn _collect_terms(&self, invert: bool) -> Vec<(bool, &Expr)> {
        if let Expr::ApplyOperator(op, l, r) = self {
            if *op == BinaryOp::Add || *op == BinaryOp::Sub {
                let mut terms = vec![];
                terms.extend(l._collect_terms(invert ^ false));
                terms.extend(r._collect_terms(invert ^ (*op == BinaryOp::Sub)));
                terms
            } else {
                vec![(!invert, self)]
            }
        } else {
            vec![(!invert, self)]
        }
    }

    fn from_terms(root: Expr, terms: Vec<(bool, Expr)>) -> Expr {
        let mut ret = root;
        for (is_positive, term) in terms {
            if is_positive {
                ret = Expr::ApplyOperator(BinaryOp::Add, Box::new(ret), Box::new(term))
            } else {
                ret = Expr::ApplyOperator(BinaryOp::Sub, Box::new(ret), Box::new(term))
            }
        }
        ret
    }
}

pub fn optimize_terms(expr: &Expr) -> Expr {
    let terms = expr
        ._collect_terms(false)
        .iter()
        .map(|(b, e)| (*b, e.optimize()))
        .collect::<Vec<_>>();

    let mut constant = 0;
    let mut new_terms = Vec::new();

    for (is_positive, term) in terms {
        if let Expr::Num(x) = term {
            constant += if is_positive { x } else { -x }
        } else {
            new_terms.push((is_positive, term))
        }
    }

    Expr::from_terms(Expr::Num(constant), new_terms)
}
