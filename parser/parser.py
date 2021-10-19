from typing import Iterable, List, Optional

from lexer.token import NumberToken
from ..lexer.layer import Element, Token


class Parser:
    def __init__(self, elements: Iterable[Element]):
        self.elements = elements

    def peek(self, consume=True) -> Optional[Element]:
        if not self.element:
            return None

        element = self.element[0]

        if consume:
            self.elements.pop(0)

        return element

    def check(self, tok: str, consume=True):
        self.peek()
        if self.peek() != tok:
            return False

        if consume:
            self.next()

        return True

    def expect_token(self, consume=True, *, token_type: type):
        tok = self.peek(consume)

        if not isinstance(tok, Token):
            raise Exception(f"Expected {token_type}, got {tok}")

        if not isinstance(tok.token, token_type):
            raise Exception(f"Expected {token_type}, got {tok}")

        return tok.token.value

    def expect(self, tok: str, consume=True):
        if not self.check(tok, consume):
            raise Exception(f"Expected {tok}")
