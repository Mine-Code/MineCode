from typing import List, Union

from ..expr.value import Value, Range
from .stmt import Stmt

Iterable = Union[Value, Range]


class For(Stmt):
    def __init__(self, target: str, iter: Iterable, body: List[Stmt]):
        super().__init__()
        self.target = target
        self.iter = iter
        self.body = body

    def __str__(self):
        return f"For<{self.target} in {self.iter}> {{\n{self.body}\n}}"

    def __repr__(self):
        return f"For<{self.target} in {self.iter}, {self.body}>"
