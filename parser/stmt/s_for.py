from .stmt import Stmt


class For(Stmt):
    def __init__(self, target: str, iter):
        super().__init__()

    def __str__(self):
        return f"For<{self.name}>({self.args})"
