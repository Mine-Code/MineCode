from typing import Iterable, Tuple, Union
from .line import Line


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
