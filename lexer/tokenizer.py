from typing import Iterable
from .token import *
import string


class Tokenizer:
    def __init__(self):
        self.input_string: str = ""
        self.input_index: int = 0
        pass

    def set_input(self, input_string):
        self.input_string = input_string
        self.input_index = 0

    def get_char(self):
        return self.input_string[self.input_index]

    def read_char(self):
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
        if self.get_char() == "\"":
            return self.read_string()

        return self.read_char()
        if self.get_char() in string.digits:
            return self.read_number()

    def read_string(self) -> Token:
        self.expect('"')

        value = ""
        while self.get_char() != '"':
            value += self.read_char()

        self.expect('"')

        return StringToken(value)

    def read_number(self) -> Token:
        pass

    def tokenize(self) -> Iterable[Token]:
        while self.has_data():
            yield self.read_token()
