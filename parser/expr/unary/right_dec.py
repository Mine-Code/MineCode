from ..exprbase import ExprBase


class RightDec(ExprBase):
    def __init__(self, expr):
        self.expr = expr

    def __str__(self):
        return str(self.expr) + '--'

    def __repr__(self):
        return 'RightDec' + repr(self.expr) + ')'
