from typing import Iterable

from .token import Token


class Line:
    def __init__(self, line: str, line_number: int, indent: int):
        self.line = line
        self.tokens: Token = []
        self.line_number = line_number
        self.indent = indent
        self.children: Iterable[Line] = []

    def to_str(self) -> str:
        lines = [
            f"{self.line}"
        ]
        for child in self.children:
            for line in child.to_str().splitlines():
                lines.append(line)

        lines = [
            "| " + line
            for line in lines
        ]
        return "\n".join(lines)

    def __str__(self) -> str:
        return self.to_str()

    def __repr__(self) -> str:
        return f"Line({self.line})"

    def is_empty(self) -> bool:
        return self.line == ""
