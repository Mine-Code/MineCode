from typing import Iterable

from .token import Token


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
