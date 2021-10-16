from typing import List, Tuple, Union


class Line:
    def __init__(self, line: str, line_number: int, indent: int):
        self.line = line
        self.line_number = line_number
        self.indent = indent
        self.children = []

    def __str__(self):
        line = str(self.line_number).rjust(5)
        indent = str(self.indent).ljust(5)
        ret = f"{line}|{indent}: {self.line}"

        for child in self.children:
            ret += "\n" + " " + str(child)

        return ret

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

    def split(self) -> List[Line]:
        lines = []
        while self.has_line():
            line = self.read_line()
            if line.is_empty():
                continue

            lines.append(line)

        return lines
