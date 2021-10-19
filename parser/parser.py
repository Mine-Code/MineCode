from typing import Iterable, List, Optional, Union

from ..lexer import token
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

    def expect_token(self, token_type: type, consume=True) -> Union[int, str]:
        tok = self.peek(consume)

        if not isinstance(tok, Token):
            raise Exception(f"Expected {token_type}, got {tok}")

        token = tok.token
        if not isinstance(token, token_type):
            raise Exception(f"Expected {token_type}, got {tok}")

        return token.value

    def expect_number(self, consume=True) -> int:
        return self.expect_token(token.NumberToken, consume)

    def expect_string(self, consume=True) -> str:
        return self.expect_token(token.StringToken, consume)

    def expect_identifier(self, expected: str, consume=True):
        test = self.expect_token(token.IdentifierToken, consume)
        if test != expected:
            raise Exception(f"Expected {expected}, got {test}")
