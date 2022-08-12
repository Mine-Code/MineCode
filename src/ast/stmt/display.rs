use super::Stmt;

impl std::fmt::Display for Stmt {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::LoadModule { module } => write!(f, "Load {}", module),
            Self::Expression(expr) => write!(f, "{}", expr),
            Self::FuncDef { name, args, body } => {
                write!(f, "def {}({}) {}", name, args.join(","), body)
            }
        }
    }
}

#[cfg(test)]
mod test {
    use crate::ast::Expr;

    use super::*;

    #[test]
    fn test_stmt_display() {
        let stmt = Stmt::LoadModule {
            module: "test".to_string(),
        };
        assert_eq!(stmt.to_string(), "Load test");
    }

    #[test]
    fn test_stmt_expr_display() {
        let stmt = Stmt::Expression(Expr::Num(1));
        assert_eq!(stmt.to_string(), "1");
    }

    #[test]
    fn test_stmt_func_display() {
        let stmt = Stmt::FuncDef {
            name: "test".to_string(),
            args: vec!["a".to_string(), "b".to_string()],
            body: Expr::Num(1),
        };
        assert_eq!(stmt.to_string(), "def test(a,b) 1");
    }
}
