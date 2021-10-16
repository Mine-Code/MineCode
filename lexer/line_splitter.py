from typing import Iterable, List, Tuple, Union


class Line:
    def __init__(self, line: str, line_number: int, indent: int):
        self.line = line
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


def count_indent(line: str) -> Tuple[str, int]:
    """
    Counts the indentation of a line.
    """
    indent = 0
    while line.startswith(' '):
        indent += 1
        line = line[1:]

    return line, indent


class LineSplitter:
    def __init__(self, src: str):
        self.raw_lines = src.splitlines()
        self.line_number = 1

    def consume(self) -> str:
        line = self.raw_lines.pop(0)
        self.line_number += 1
        return line

    def peek(self) -> Union[str, None]:
        if self.has_line():
            return self.raw_lines[0]

        return ""

    def has_line(self) -> bool:
        return len(self.raw_lines) > 0

    def read_line(self) -> Line:
        line, indent = count_indent(self.consume())
        return Line(line, self.line_number, indent)

    def peek_line(self) -> Line:
        line, ident = count_indent(self.peek())
        return Line(line, self.line_number, ident)

    def split(self, until=-1) -> Iterable[Line]:
        while self.has_line():
            line = self.peek_line()
            if line.is_empty():
                self.consume()
                continue

            if line.indent <= until:
                break

            self.consume()

            if self.has_line() and self.peek_line().indent > line.indent:
                line.children = [*self.split(until=line.indent)]

            yield line
