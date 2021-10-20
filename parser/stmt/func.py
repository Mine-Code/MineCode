from typing import Iterable
from .stmt import Stmt
from ...lexer.token import IdentifierToken


class Func(Stmt):
    def __init__(self, func_name: str, args: Iterable[IdentifierToken]):
        super().__init__()
        self.name = func_name
        self.args = args

    def __str__(self):
        return f"Func<{self.name}>({self.args})"
