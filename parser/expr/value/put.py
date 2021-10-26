from .value import Value
from .identifier import Identifier
from ..exprbase import ExprBase


class Put(Value):
    def __init__(self, target: Identifier, expr: ExprBase):
        self.target = target
        self.expr = expr

    def __str__(self):
        return f'{self.target} << {self.expr}'

    def __repr__(self):
        return f'Put(target={repr(self.target)}, expr={repr(self.expr)})'
