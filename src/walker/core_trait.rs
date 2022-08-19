use crate::ast::{BinaryOp, Expr, Stmt};

pub trait Walker {
    type StmtT;
    type ExprT;

    fn add_stmt(&mut self, stmt: Self::StmtT);

    fn get_stmts(&self) -> &Vec<Self::StmtT>;

    fn walk<T: IntoIterator<Item = Stmt>>(&mut self, code: T) -> &Vec<Self::StmtT> {
        for stmt in code {
            self.walk_stmt(&stmt);
        }
        self.get_stmts()
    }
    fn walk_ref<'a, T: IntoIterator<Item = &'a Stmt>>(&mut self, code: T) -> &Vec<Self::StmtT> {
        for stmt in code {
            self.walk_stmt(stmt);
        }
        self.get_stmts()
    }

    fn walk_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Expression(expr) => self.walk_stmt_expr(expr),
            Stmt::LoadModule { module } => self.walk_load_module(module.clone()),
            Stmt::FuncDef { name, args, body } => {
                self.walk_func_def(name.clone(), args.clone(), body)
            }
        }
    }

    fn walk_load_module(&mut self, module_name: String);
    fn walk_stmt_expr(&mut self, expr: &Expr);
    fn walk_func_def(&mut self, name: String, args: Vec<String>, body: &Expr);

    fn walk_expr(&mut self, expr: &Expr) -> Self::ExprT {
        match expr {
            Expr::Num(x) => self.walk_num(*x),
            Expr::Ident(x) => self.walk_ident(x.clone()),
            Expr::Storage(x) => self.walk_storage(*x),
            Expr::String(x) => self.walk_string(x.clone()),
            Expr::FuncCall(x, y) => self.walk_func_call(&**x, y),
            Expr::DirectFuncCall(addr, args) => self.walk_direct_func_call(*addr, args),

            Expr::Ranged(x, y) => self.walk_ranged(&**x, &**y),
            Expr::Pointer(x) => self.walk_pointer(&**x),
            Expr::CompileTime(x) => self.walk_compile_time(&**x),
            Expr::ApplyOperator(x, y, z) => self.walk_apply_operator(x.clone(), &**y, &**z),
            Expr::LogicalNot(x) => self.walk_logical_not(&**x),

            Expr::BitwiseNot(x) => self.walk_bitwise_not(&**x),
            Expr::Negative(x) => self.walk_negative(&**x),
            Expr::Subscript(x, y) => self.walk_subscript(&**x, &**y),
            Expr::Attribute(x, y) => self.walk_attribute(&**x, y.clone()),
            Expr::If { branches, fallback } => self.walk_if(branches, &**fallback),
            Expr::For {
                name,
                iter,
                body,
                value,
            } => self.walk_for(name.clone(), &**iter, &**body, &**value),
            Expr::Exprs(x) => self.walk_exprs(x),
            Expr::Nil => self.walk_nil(),
        }
    }
    fn walk_num(&mut self, num: i32) -> Self::ExprT;
    fn walk_ident(&mut self, ident: String) -> Self::ExprT;
    fn walk_storage(&mut self, index: usize) -> Self::ExprT;
    fn walk_string(&mut self, string: String) -> Self::ExprT;
    fn walk_func_call(&mut self, func_name: &Expr, args: &Vec<Expr>) -> Self::ExprT;
    fn walk_direct_func_call(&mut self, addr: u64, args: &Vec<Expr>) -> Self::ExprT;
    fn walk_ranged(&mut self, start: &Expr, end: &Expr) -> Self::ExprT;
    fn walk_pointer(&mut self, expr: &Expr) -> Self::ExprT;
    fn walk_compile_time(&mut self, expr: &Expr) -> Self::ExprT;
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) -> Self::ExprT;
    fn walk_logical_not(&mut self, expr: &Expr) -> Self::ExprT;
    fn walk_bitwise_not(&mut self, expr: &Expr) -> Self::ExprT;
    fn walk_negative(&mut self, expr: &Expr) -> Self::ExprT;
    fn walk_subscript(&mut self, expr: &Expr, index: &Expr) -> Self::ExprT;
    fn walk_attribute(&mut self, expr: &Expr, attr: String) -> Self::ExprT;
    fn walk_if(&mut self, branches: &Vec<(Expr, Expr)>, fallback: &Expr) -> Self::ExprT;
    fn walk_for(&mut self, name: String, iter: &Expr, body: &Expr, value: &Expr) -> Self::ExprT;
    fn walk_exprs(&mut self, exprs: &Vec<Expr>) -> Self::ExprT;
    fn walk_nil(&mut self) -> Self::ExprT;
}
