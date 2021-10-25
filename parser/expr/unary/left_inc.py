from ..exprbase import ExprBase


class LeftInc(ExprBase):
    def __init__(self, expr):
        self.expr = expr

    def __str__(self):
        return '++' + str(self.expr)

    def __repr__(self):
        return 'LeftInc(' + repr(self.expr) + ')'
