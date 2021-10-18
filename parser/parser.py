from typing import Iterable, List, Optional
from ..lexer.layer import Element


class Parser:
    def __init__(self, elements: Iterable[Element]):
        self.elements = elements

    def next(self) -> Element:
        return self.element.pop(0)

    def peek(self) -> Optional[Element]:
        if not self.element:
            return None

        return self.element[0]

    def check(self, tok: str, consume=True):
        self.peek()
        if self.peek(). != tok:
            return False

        if consume:
            self.next()

        return True

    def expect(self, tok: str, consume=True):
        if not self.check(tok, consume):
            raise Exception(f"Expected {tok}")
