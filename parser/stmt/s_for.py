from typing import Union

from ..expr.value import Value, Range
from .stmt import Stmt

Iterable = Union[Value, Range]


class For(Stmt):
    def __init__(self, target: str, iter: Iterable):
        super().__init__()
        self.target = target
        self.iter = iter

    def __str__(self):
        return f"For<{self.target} in {self.iter}>"
