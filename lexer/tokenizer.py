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

    def read_token(self) -> Token:
        if self.get_char() == "\"":
            return self.read_string()

        if self.get_char() in string.digits:
            return self.read_number()

    def read_string(self) -> Token:
        pass

    def read_number(self) -> Token:
        pass
