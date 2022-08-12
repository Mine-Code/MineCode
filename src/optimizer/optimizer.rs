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
    fn _collect_factors(&self) -> (Vec<&Expr>, Vec<&Expr>) {
        let mut numer = vec![];
        let mut denom = vec![];
        if let Expr::ApplyOperator(op, r, l) = self {
            if *op == BinaryOp::Mul {
                let (sub_numer, sub_denom) = r._collect_factors();
                numer.extend(sub_numer);
                denom.extend(sub_denom);

                let (sub_numer, sub_denom) = l._collect_factors();
                numer.extend(sub_numer);
                denom.extend(sub_denom);
            } else if *op == BinaryOp::Div {
                let (sub_numer, sub_denom) = r._collect_factors();
                numer.extend(sub_numer);
                denom.extend(sub_denom);

                let (sub_numer, sub_denom) = l._collect_factors();
                denom.extend(sub_numer);
                numer.extend(sub_denom);
            } else {
                numer.push(self);
            }
        } else {
            numer.push(self);
        }

        (numer, denom)
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
    fn _from_factors(root: Expr, factors: Vec<Expr>) -> Expr {
        let mut ret = root;
        for factor in factors {
            ret = Expr::ApplyOperator(BinaryOp::Mul, Box::new(ret), Box::new(factor));
        }
        ret
    }
    fn from_factors(factors: Vec<Expr>) -> Expr {
        if factors.is_empty() {
            Expr::Num(1)
        } else if factors.len() == 1 {
            factors[0].clone()
        } else {
            let mut ret = factors[0].clone();
            for factor in factors[1..].to_vec() {
                ret = Expr::ApplyOperator(BinaryOp::Mul, Box::new(ret), Box::new(factor));
            }
            ret
        }
    }
    fn multiply(coefficient: i32, value: Expr) -> Expr {
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

fn fold_factors(factors: Vec<Expr>) -> (i32, Vec<Expr>) {
    let mut constant = 1;
    let mut new_factors = Vec::new();

    for factor in factors {
        if let Expr::Num(x) = factor {
            constant *= x;
        } else {
            new_factors.push(factor)
        }
    }

    (constant, new_factors)
}

fn gcd(a: i32, b: i32) -> i32 {
    if b == 0 {
        a
    } else {
        gcd(b, a % b)
    }
}

fn fold_factors_with_optimize(factors: Vec<&Expr>) -> (i32, Vec<Expr>) {
    fold_factors(factors.iter().map(|e| e.optimize()).collect::<Vec<_>>())
}

fn div(a: i32, b: i32) -> (i32, i32) {
    let g = gcd(a, b);
    let a = a / g;
    let b = b / g;

    (a, b)
}

impl Expr {
    pub fn optimize(&self) -> Self {
        if let Expr::ApplyOperator(op, _, _) = self {
            if *op == BinaryOp::Add || *op == BinaryOp::Sub {
                let terms = self
                    ._collect_terms(false)
                    .iter()
                    .map(|(b, e)| (*b, e.optimize()))
                    .collect::<Vec<_>>();

                let mut constant = 0;
                let mut new_terms = Vec::new();

                for (is_positive, term) in terms {
                    if let Self::Num(x) = term {
                        constant += if is_positive { x } else { -x }
                    } else {
                        new_terms.push((is_positive, term))
                    }
                }

                return Expr::from_terms(Expr::Num(constant), new_terms);
            }

            if *op == BinaryOp::Mul || *op == BinaryOp::Div {
                let (numer, denom) = self._collect_factors();

                let (numer_constant, numer) = fold_factors_with_optimize(numer);
                let (denom_constant, denom) = fold_factors_with_optimize(denom);

                let (coeff_numer, coeff_denom) = div(numer_constant, denom_constant);

                let numer = Expr::multiply(coeff_numer, Expr::from_factors(numer));
                let denom = Expr::multiply(coeff_denom, Expr::from_factors(denom));

                if denom == Expr::Num(1) {
                    return numer;
                } else {
                    return Expr::ApplyOperator(BinaryOp::Div, Box::new(numer), Box::new(denom));
                }
            }
        }

        (*self).clone()
    }
}
