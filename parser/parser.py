from typing import Iterable, List, Optional, Union

from .stmt.program import Program


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

    def has_elements(self) -> bool:
        return len(self.elements) > 0

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

    def expect_identifier(self, expected: str = "", consume=True) -> str:
        test = self.expect_token(token.IdentifierToken, consume)
        if expected == "":
            return test

        if test == expected:
            return test

        raise Exception(f"Expected {expected}, got {test}")

    def parse(self) -> Program:
        ret = Program()

        stmt = self.parse_stmt()
        while self.has_elements():
            ret.add_stmt(stmt)
            stmt = self.parse_stmt()

        return ret
