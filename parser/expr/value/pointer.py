from ..exprbase import ExprBase
from .value import Value


class Pointer(Value):
    def __init__(self, expr: ExprBase):
        self.expr = expr

    def __str__(self):
        return f'[{self.expr}]'

    def __repr__(self):
        return f'Pointer({self.expr})'
