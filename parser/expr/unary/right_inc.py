from ..exprbase import ExprBase


class RightInc(ExprBase):
    def __init__(self, expr):
        self.expr = expr

    def __str__(self):
        return str(self.expr) + '++'

    def __repr__(self):
        return 'RightInc(' + repr(self.expr) + ')'
