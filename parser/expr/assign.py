from .expr import ExprBase
from ..identifier import Identifier


class Assign(ExprBase):
    def __init__(
            self,
            name: Identifier, value: ExprBase,
            operator: str = "="
    ):
        self.name = name
        self.value = value
        self.operator = operator

    def __str__(self):
        return f"{self.name} {self.operator} {self.value}"
