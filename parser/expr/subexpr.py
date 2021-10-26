from .expr import ExprBase


class SubExpr(ExprBase):
    def __init__(self, expr: ExprBase):
        self.expr = expr

    def __str__(self):
        return f"({self.expr})"

    def __repr__(self):
        return "SubExpr(" + repr(self.expr) + ")"
