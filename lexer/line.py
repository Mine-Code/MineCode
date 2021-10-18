from typing import Iterable, List

from .token import Token
from . import layer


class Line:
    def __init__(self, line: str, line_number: int, indent: int):
        self.line = line
        self.tokens: Token = []
        self.line_number = line_number
        self.indent = indent
        self.children: Iterable[Line] = []

    def __str__(self) -> str:
        lines = []
        if self.tokens:
            lines.append(", ".join([str(x) for x in self.tokens]))
        else:
            lines.append(self.line)
        for child in self.children:
            for line in str(child).splitlines():
                lines.append(line)

        lines = [
            "| " + line
            for line in lines
        ]
        return "\n".join(lines)

    def __repr__(self) -> str:
        return f"({self.tokens}){self.children}"

    def is_empty(self) -> bool:
        return self.line == ""

    def to_element(self) -> List[layer.Element]:
        ret = [layer.Token(tok) for tok in self.tokens]

        tmp: List[layer.Element] = []
        for child in self.children:
            tmp.extend(child.to_element())

        ret.append(layer.Layer(tmp))

        return ret
