use crate::ast::{BinaryOp, Expr};

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

    fn walk_load_module(&mut self, module_name: String) -> Vec<u8> {
        let mut ret = vec![0u8];
        ret.extend(module_name.as_bytes().to_vec());
        ret
    }
    fn walk_stmt_expr(&mut self, expr: &Expr) -> Vec<u8> {
        self.walk_expr(expr)
    }

    fn walk_num(&mut self, num: i32) -> Vec<u8> {
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
    fn walk_ident(&mut self, ident: String) -> Vec<u8> {
        // TODO: Make Identifier listup phase and fix this.
        let mut ret = vec![0xfdu8];
        ret.extend(ident.as_bytes().to_vec());
        ret.push(0u8);
        ret
    }
    fn walk_string(&mut self, string: String) -> Vec<u8> {
        let mut ret = vec![0x94u8];
        ret.extend(string.as_bytes().to_vec());
        ret.push(0u8);
        ret
    }
    fn walk_func_call(&mut self, func_name: &Expr, args: &Vec<Expr>) -> Vec<u8> {
        // TODO: FIX THIS! for 'func[x](y...)'
        let mut ret = vec![0xfcu8];
        ret.extend(self.walk_expr(func_name));
        ret.extend(self.walk_exprs(args));
        ret
    }
    fn walk_ranged(&mut self, _start: &Expr, _end: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_pointer(&mut self, expr: &Expr) -> Vec<u8> {
        let mut ret = vec![0x9du8];
        ret.extend(self.walk_expr(expr));
        ret
    }
    fn walk_compile_time(&mut self, _expr: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_apply_operator(&mut self, op: BinaryOp, left: &Expr, right: &Expr) -> Vec<u8> {
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
    fn walk_logical_not(&mut self, _expr: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_bitwise_not(&mut self, _expr: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_negative(&mut self, _expr: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_subscript(&mut self, expr: &Expr, index: &Expr) -> Vec<u8> {
        let mut ret = vec![0x9bu8];
        ret.extend(self.walk_expr(expr));
        ret.extend(self.walk_expr(index));
        ret
    }
    fn walk_attribute(&mut self, _expr: &Expr, _attr: String) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_if(&mut self, branches: &Vec<(Expr, Expr)>, fallback: &Expr) -> Vec<u8> {
        // TODO: Optimize for the case where there is only one branch.
        // TODO: Optimize for the following case: if a == b => c
        let mut ret = vec![0xf5u8];
        for (cond, body) in branches {
            ret.extend(self.walk_expr(&cond));
            ret.extend(self.walk_expr(&body));
        }
        ret.extend(self.walk_expr(&fallback));

        ret
    }
    fn walk_for(&mut self, _name: String, _iter: &Expr, _body: &Expr, _value: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_exprs(&mut self, exprs: &Vec<Expr>) -> Vec<u8> {
        let mut ret = vec![0xf6u8];
        for expr in exprs {
            ret.extend(self.walk_expr(&expr));
        }
        ret.push(0xff);
        ret
    }
    fn walk_func_def(&mut self, _name: String, _args: Vec<String>, _body: &Expr) -> Vec<u8> {
        unimplemented!()
    }
    fn walk_nil(&mut self) -> Self::ExprT {
        vec![0xf7u8]
    }
}
