use super::Expr;

impl std::fmt::Display for Expr {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            Self::Num(x) => x.to_string(),
            Self::Ident(x) => x.to_string(),
            Self::Storage(x) => format!("storage{}", x),
            Self::String(x) => format!("\"{}\"", x.escape_default()),
            Self::FuncCall(func, args) => format!(
                "{}({})",
                func,
                args.iter()
                    .map(|x| format!("{x}"))
                    .collect::<Vec<String>>()
                    .join(", ")
            ),
            Self::DirectFuncCall(addr, args) => format!(
                "$0x{:08x}({})",
                addr,
                args.iter()
                    .map(|x| format!("{x}"))
                    .collect::<Vec<String>>()
                    .join(", ")
            ),
            Self::Attribute(r, l) => format!("{}.{}", r, l),
            Self::Ranged(begin, end) => format!("Range<{} -> {}>", begin, end),
            Self::Pointer(x) => format!("Ptr<{}>", x),
            Self::CompileTime(x) => format!("CompileTime<{}>", x),
            Self::ApplyOperator(op, r, l) => format!("({} {} {})", r, op, l),
            Self::LogicalNot(x) => format!("!{}", x),
            Self::BitwiseNot(x) => format!("~{}", x),
            Self::Negative(x) => format!("-{}", x),
            Self::Subscript(arr, ind) => format!("{}[{}]", arr, ind),
            Self::If { branches, fallback } => format!(
                "if {}=> {}",
                branches
                    .iter()
                    .fold("".to_string(), |a, (c, e)| a + &format!("{} => {}, ", c, e)),
                fallback
            ),
            Self::Exprs(exprs) => format!(
                "{{{}}}",
                exprs
                    .iter()
                    .fold("".to_string(), |a, c| a + &format!("{}; ", c)),
            ),

            Self::For {
                name,
                iter,
                body,
                value,
            } => {
                format!("for {} in {}: [{}] => {}", name, iter, body, value)
            }

            Self::Nil => "nil".to_string(),
        };

        write!(f, "{}", s)
    }
}

#[cfg(test)]
mod test {
    use crate::ast::BinaryOp;

    use super::*;

    #[test]
    fn test_num_display() {
        let expr = Expr::Num(0);
        assert_eq!(expr.to_string(), "0");
    }
    #[test]
    fn test_ident_display() {
        let expr = Expr::Ident("x".to_string());
        assert_eq!(expr.to_string(), "x");
    }
    #[test]
    fn test_string_display() {
        let expr = Expr::String("abc\n\0def".to_string());
        assert_eq!(expr.to_string(), "\"abc\\n\\u{0}def\"");
    }
    #[test]
    fn test_funccall_display() {
        let expr = Expr::FuncCall(
            Box::new(Expr::Ident("x".to_string())),
            vec![Expr::Num(0), Expr::Num(1)],
        );
        assert_eq!(expr.to_string(), "x(0, 1)");
    }
    #[test]
    fn test_ranged_display() {
        let expr = Expr::Ranged(Box::new(Expr::Num(0)), Box::new(Expr::Num(1)));
        assert_eq!(expr.to_string(), "Range<0 -> 1>");
    }
    #[test]
    fn test_pointer_display() {
        let expr = Expr::Pointer(Box::new(Expr::Num(1000)));
        assert_eq!(expr.to_string(), "Ptr<1000>");
    }
    #[test]
    fn test_compiletime_display() {
        let expr = Expr::CompileTime(Box::new(Expr::FuncCall(
            Box::new(Expr::Ident("x".to_string())),
            vec![Expr::Num(0), Expr::Num(1)],
        )));
        assert_eq!(expr.to_string(), "CompileTime<x(0, 1)>");
    }
    #[test]
    fn test_applyoperator_display() {
        let expr = Expr::ApplyOperator(
            BinaryOp::Add,
            Box::new(Expr::Num(0)),
            Box::new(Expr::Num(1)),
        );
        assert_eq!(expr.to_string(), "0 + 1");
    }
    #[test]
    fn test_logicalnot_display() {
        let expr = Expr::LogicalNot(Box::new(Expr::Ident("x".to_string())));
        assert_eq!(expr.to_string(), "!x");
    }
    #[test]
    fn test_bitwisenot_display() {
        let expr = Expr::BitwiseNot(Box::new(Expr::Ident("x".to_string())));
        assert_eq!(expr.to_string(), "~x");
    }
    #[test]
    fn test_negative_display() {
        let expr = Expr::Negative(Box::new(Expr::Ident("x".to_string())));
        assert_eq!(expr.to_string(), "-x");
    }
    #[test]
    fn test_subscript_display() {
        let expr = Expr::Subscript(
            Box::new(Expr::Ident("x".to_string())),
            Box::new(Expr::Num(0)),
        );
        assert_eq!(expr.to_string(), "x[0]");
    }
    #[test]
    fn test_if_display() {
        let expr = Expr::If {
            branches: vec![
                (Expr::Ident("x".to_string()), Expr::Num(0)),
                (Expr::Ident("y".to_string()), Expr::Num(1)),
            ],
            fallback: Box::new(Expr::Num(2)),
        };
        assert_eq!(expr.to_string(), "if x => 0, y => 1, _ => 2");
    }
    #[test]
    fn test_for_display() {
        let expr = Expr::For {
            name: "x".to_string(),
            iter: Box::new(Expr::Ident("y".to_string())),
            body: Box::new(Expr::Num(0)),
            value: Box::new(Expr::Num(1)),
        };
        assert_eq!(expr.to_string(), "for x in y: [0] => 1");
    }

    #[test]
    fn test_exprs_display() {
        let expr = Expr::Exprs(vec![Expr::Num(0), Expr::Num(1), Expr::Num(2)]);

        assert_eq!(expr.to_string(), "{0; 1; 2; }");
    }

    #[test]
    fn test_storage_display() {
        let expr = Expr::Storage(0);
        assert_eq!(expr.to_string(), "storage0");
    }

    #[test]
    fn test_direct_function_call() {
        let expr = Expr::DirectFuncCall(0x1122334411223344, vec![Expr::Num(0), Expr::Num(1)]);
        assert_eq!(expr.to_string(), "$0x1122334411223344(0, 1)");
    }
}
