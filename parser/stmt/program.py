from typing import List
from .stmt import Stmt


class Program:
    def __init__(self):
        self.stmts: List[Stmt] = []

    def add_stmt(self, stmt: Stmt):
        self.stmts.append(stmt)
