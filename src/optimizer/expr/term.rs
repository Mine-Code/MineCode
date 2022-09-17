use crate::ast::{BinaryOp, Expr};

impl Expr {
    fn _collect_factors(&self) -> (Vec<&Expr>, Vec<&Expr>) {
        let mut numer = vec![];
        let mut denom = vec![];
        if let Expr::ApplyOperator(op, r, l) = self {
            if *op == BinaryOp::Mul {
                // (a/b)/(c/d) = (a/b)*(c/d)
                let (sub_numer, sub_denom) = r._collect_factors();
                numer.extend(sub_numer);
                denom.extend(sub_denom);

                let (sub_numer, sub_denom) = l._collect_factors();
                numer.extend(sub_numer);
                denom.extend(sub_denom);
            } else if *op == BinaryOp::Div {
                // (a/b)/(c/d) = (a/b)*(d/c)
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

    fn from_factors(factors: Vec<Expr>) -> Expr {
        if factors.is_empty() {
            Expr::Num(1)
        } else if factors.len() == 1 {
            factors[0].clone()
        } else {
            let mut ret = factors[0].clone();
            for factor in factors[1..].iter().cloned() {
                ret = Expr::ApplyOperator(BinaryOp::Mul, Box::new(ret), Box::new(factor));
            }
            ret
        }
    }
}

fn fold_factors_with_optimize(factors: Vec<&Expr>) -> (i32, Vec<Expr>) {
    let mut constant = 1;
    let mut new_factors = Vec::new();

    for factor in factors {
        if let Expr::Num(x) = factor {
            constant *= x;
        } else {
            new_factors.push(factor.optimize())
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

fn div(a: i32, b: i32) -> (i32, i32) {
    let g = gcd(a, b);
    let a = a / g;
    let b = b / g;

    (a, b)
}

pub fn optimize_terms(expr: &Expr) -> Expr {
    // TODO(syoch): Impl optimizer for not Expr::Num type.
    let (numer, denom) = expr._collect_factors();

    let (numer_constant, numer) = fold_factors_with_optimize(numer);
    let (denom_constant, denom) = fold_factors_with_optimize(denom);

    let (coeff_numer, coeff_denom) = div(numer_constant, denom_constant);

    let numer = Expr::multiply_immediate(Expr::from_factors(numer), coeff_numer);
    let denom = Expr::multiply_immediate(Expr::from_factors(denom), coeff_denom);

    if denom == Expr::Num(1) {
        numer
    } else {
        Expr::ApplyOperator(BinaryOp::Div, Box::new(numer), Box::new(denom))
    }
}
