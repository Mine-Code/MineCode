from .stmt import Stmt
from ..expr.exprbase import ExprBase


class StmtExpr(Stmt):
    def __init__(self, expr: ExprBase):
        self.expr = expr

    def __str__(self):
        return str(self.expr)

    def __repr__(self):
        return str(self)
