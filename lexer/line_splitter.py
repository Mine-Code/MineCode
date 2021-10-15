from typing import Iterable, List


class Line:
    def __init__(self, line: str, line_number: int, depth: int):
        self.line = line
        self.line_number = line_number
        self.depth = depth

    def __str__(self):
        return f"{str(self.line_number).rjust(5)}|{str(self.depth).ljust(5)}: {self.line}"


def lineSplitter(src: str) -> Iterable[Line]:
    """
    Splits a string into a list of lines.
    """
    for i, raw_line in enumerate(src.split('\n')):
        depth = 0
        while raw_line.startswith(' '):
            depth += 1
            raw_line = raw_line[1:]

        if not raw_line:
            continue

        yield Line(raw_line, i, depth)
