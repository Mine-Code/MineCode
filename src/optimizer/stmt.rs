use crate::ast::Stmt;

impl Stmt {
    pub fn optimize(&self) -> Self {
        match self {
            Stmt::Expression(expr) => Stmt::Expression(expr.optimize()),
            _ => (*self).clone(),
        }
    }
}
