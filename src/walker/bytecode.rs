use crate::ast::{BinaryOp, Expr, Stmt};

use super::core_trait::Walker;

pub struct ByteCodeWalker {}

impl ByteCodeWalker {
    pub fn new() -> ByteCodeWalker {
        ByteCodeWalker {}
    }
}

impl Walker for ByteCodeWalker {
    type StmtT = Vec<u8>;
    type ExprT = Vec<u8>;

    fn walk_stmt(&self, stmt: Stmt) -> Self::StmtT {
        match stmt {
            Stmt::Expression(expr) => self.walk_expr(expr),
            Stmt::LoadModule { module } => self.walk_load_module(module),
            Stmt::FuncDef { name, args, body } => self.walk_func_def(name, args, body),
        }
    }

    fn walk_load_module(&self, module_name: String) -> Vec<u8> {
        let mut ret = vec![0u8];
        ret.extend(module_name.as_bytes().to_vec());
        ret
    }
    fn walk_expr(&self, expr: Expr) -> Vec<u8> {
        match expr {
            Expr::Num(x) => self.walk_num(x),
            Expr::Ident(x) => self.walk_ident(x),
            Expr::String(x) => self.walk_string(x),
            Expr::FuncCall(x, y) => self.walk_func_call(*x, y),

            Expr::Ranged(x, y) => self.walk_ranged(*x, *y),
            Expr::Pointer(x) => self.walk_pointer(*x),
            Expr::CompileTime(x) => self.walk_compile_time(*x),
            Expr::ApplyOperator(x, y, z) => self.walk_apply_operator(x, *y, *z),
            Expr::LogicalNot(x) => self.walk_logical_not(*x),

            Expr::BitwiseNot(x) => self.walk_bitwise_not(*x),
            Expr::Negative(x) => self.walk_negative(*x),
            Expr::Subscript(x, y) => self.walk_subscript(*x, *y),
            Expr::Attribute(x, y) => self.walk_attribute(*x, y),
            Expr::If { branches, fallback } => self.walk_if(branches, fallback.map(|x| *x)),
            Expr::For {
                name,
                iter,
                body,
                value,
            } => self.walk_for(name, *iter, *body, value.map(|x| *x)),
            Expr::Exprs(x) => self.walk_exprs(x),
        }
    }

    fn walk_num(&self, num: i32) -> Vec<u8> {
        let mut ret = vec![];
        if num < 0x100 {
            ret.push(0x90u8);
            ret.push(num as u8);
        } else if num < 0x10000 {
            ret.push(0x91u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);
        } else
        /* if num < 0x100000000 */
        {
            ret.push(0x92u8);
            ret.push((num >> 24) as u8);
            ret.push((num >> 16) as u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);
        }
        /*  else  if num < 0x10000000000000000{
            ret.push(0x93u8);
            ret.push((num >> 56) as u8);
            ret.push((num >> 48) as u8);
            ret.push((num >> 40) as u8);
            ret.push((num >> 32) as u8);
            ret.push((num >> 24) as u8);
            ret.push((num >> 16) as u8);
            ret.push((num >> 8) as u8);
            ret.push(num as u8);

        } */
        ret
    }
    fn walk_ident(&self, ident: String) -> Vec<u8> {
        // TODO: Make Identifier listup phase and fix this.
        let mut ret = vec![0xfdu8];
        ret.extend(ident.as_bytes().to_vec());
        ret.push(0u8);
        ret
    }
    fn walk_string(&self, string: String) -> Vec<u8> {
        let mut ret = vec![0x94u8];
        ret.extend(string.as_bytes().to_vec());
        ret.push(0u8);
        ret
    }
    fn walk_func_call(&self, func_name: Expr, args: Vec<Expr>) -> Vec<u8> {
        // TODO: FIX THIS! for 'func[x](y...)'
        let mut ret = vec![0xfcu8];
        ret.extend(self.walk_expr(func_name));
        ret.extend(self.walk_exprs(args));
        ret
    }
    fn walk_ranged(&self, start: Expr, end: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_pointer(&self, expr: Expr) -> Vec<u8> {
        let mut ret = vec![0x9du8];
        ret.extend(self.walk_expr(expr));
        ret
    }
    fn walk_compile_time(&self, expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_apply_operator(&self, op: BinaryOp, left: Expr, right: Expr) -> Vec<u8> {
        if op == BinaryOp::Assignment {
            let mut ret = vec![0xf4u8];
            ret.extend(self.walk_expr(left));
            ret.extend(self.walk_expr(right));
            return ret;
        }
        let mut ret = vec![op.into()];
        ret.extend(self.walk_expr(left));
        ret.extend(self.walk_expr(right));
        ret
    }
    fn walk_logical_not(&self, expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_bitwise_not(&self, expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_negative(&self, expr: Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_subscript(&self, expr: Expr, index: Expr) -> Vec<u8> {
        let mut ret = vec![0x9bu8];
        ret.extend(self.walk_expr(expr));
        ret.extend(self.walk_expr(index));
        ret
    }
    fn walk_attribute(&self, expr: Expr, attr: String) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_if(&self, branches: Vec<(Expr, Expr)>, fallback: Option<Expr>) -> Vec<u8> {
        // TODO: Optimize for the case where there is only one branch.
        // TODO: Optimize for the following case: if a == b => c
        let mut ret = vec![0xf5u8];
        for (cond, body) in branches {
            ret.extend(self.walk_expr(cond));
            ret.extend(self.walk_expr(body));
        }
        if let Some(fallback) = fallback {
            ret.extend(self.walk_expr(fallback));
        } else {
            ret.push(0xf7u8);
        }

        ret
    }
    fn walk_for(&self, name: String, iter: Expr, body: Expr, value: Option<Expr>) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_exprs(&self, exprs: Vec<Expr>) -> Vec<u8> {
        let mut ret = vec![0xf6u8];
        for expr in exprs {
            ret.extend(self.walk_expr(expr));
        }
        ret.push(0xff);
        ret
    }
    fn walk_func_def(&self, name: String, args: Vec<String>, body: Expr) -> Vec<u8> {
        unimplemented!()
    }
}
