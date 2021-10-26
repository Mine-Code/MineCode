from typing import List
from ..expr.exprbase import ExprBase
from .stmt import Stmt


class While(Stmt):
    def __init__(self, conditional: ExprBase, body: List[Stmt]):
        super().__init__()
        self.conditional = conditional
        self.body = body

    def __str__(self):
        return f'while ({self.conditional}) {{\n{self.body}\n}}'

    def __repr__(self):
        return f'While({self.conditional}, {self.body})'
