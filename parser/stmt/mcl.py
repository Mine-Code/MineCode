from .stmt import Stmt


class Mcl(Stmt):
    def __init__(self, mcl_name):
        super().__init__()
        self.name = mcl_name

    def __str__(self):
        return f"MCL<{self.name}>"
