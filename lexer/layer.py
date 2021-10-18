from typing import List
from .token import Token as RawToken


class Element:
    pass


class Layer(Element):
    def __init__(self, elements: List[Element]) -> None:
        super().__init__()
        self.tokens: List[Element] = elements

    def __str__(self) -> str:
        return str(self.tokens)

    def __repr__(self) -> str:
        return str(self)


class Token(Element):
    def __init__(self, token: RawToken):
        super().__init__()
        self.token = token

    def __str__(self) -> str:
        return str(self.token)

    def __repr__(self) -> str:
        return str(self)
