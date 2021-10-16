from typing import Iterable
from .token import OperatorToken, IdentifierToken, NumberToken, StringToken, Token
import string

OPERATORS = [
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "%",
    "<<",
    ">>",
    "&",
    "^",
    "|"
]
OPERATOR_PREFIXES = [
    "=",
    "!",
    "<",
    ">",
    "+",
    "-",
    "*",
    "/",
    "%",
    "&",
    "^",
    "|"
]


class Tokenizer:
    def __init__(self):
        self.input_string: str = ""
        self.input_index: int = 0

    def set_input(self, input_string):
        self.input_string = input_string
        self.input_index = 0

    def get_char(self):
        if not self.has_data():
            return ""
        return self.input_string[self.input_index]

    def read_char(self) -> str:
        ch = self.get_char()
        self.input_index += 1
        return ch

    def expect(self, ch: str):
        if self.get_char() != ch:
            raise Exception("Expected '{}'".format(ch))
        self.read_char()

    def has_data(self):
        return self.input_index < len(self.input_string)

    def read_token(self) -> Token:
        ch = self.get_char()
        if ch == "\"":
            return self.read_string()

        if ch in string.digits:
            return self.read_number()

        if ch in string.ascii_lowercase:
            return self.read_identifier()

        return self.read_char()

    def read_identifier(self) -> Token:
        value = ""
        while self.get_char() in string.ascii_lowercase:
            value += self.read_char()

        return IdentifierToken(value)

    def read_string(self) -> Token:
        self.expect('"')

        value = ""
        while self.get_char() != '"':
            value += self.read_char()

        self.expect('"')

        return StringToken(value)

    def read_number(self) -> Token:
        sign = 1
        base = 10
        if self.get_char() == '-':
            sign = -1
            self.read_char()

        if self.get_char() == '0':
            self.read_char()
            mode = self.read_char()
            if mode == 'x':
                base = 16
            elif mode == 'o':
                base = 8
            elif mode == 'b':
                base = 2
        value = self._read_number(base)
        return NumberToken(value * sign)

    def _read_number(self, base=10) -> int:
        value = 0
        while self.get_char() in string.hexdigits and self.has_data():
            ch = self.read_char()

            if ch in string.digits:
                digit = ord(ch) - ord('0')
            elif ch in string.ascii_lowercase:
                digit = ord(ch) - ord('a') + 10

            value = value * base + digit
        return value

    def tokenize(self) -> Iterable[Token]:
        while self.has_data():
            yield self.read_token()
