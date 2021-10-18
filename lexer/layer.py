from typing import List
from .token import Token as RawToken


class Element:
    pass


class Layer(Element):
    def __init__(self) -> None:
        super().__init__()
        self.tokens: List[Element] = []


class Token(Element):
    def __init__(self, token: RawToken):
        super().__init__()
        self.token = token
