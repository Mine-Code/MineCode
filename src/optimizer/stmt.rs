use crate::ast::Stmt;

impl Stmt {
    pub fn optimize(&mut self) -> &Self {
        if let Stmt::Expression(expr) = self {
            *self = Stmt::Expression(expr.optimize());
        }
        self
    }
}
