from typing import Iterable, List


class Line:
    def __init__(self, line: str, line_number: int, depth: int):
        self.line = line
        self.line_number = line_number
        self.depth = depth
        self.children = []

    def __str__(self):
        line = str(self.line_number).rjust(5)
        depth = str(self.depth).ljust(5)
        ret = f"{line}|{depth}: {self.line}"

        for child in self.children:
            ret += "\n" + str(child)

        return ret


def lineSplitter(src: str) -> Iterable[Line]:
    """
    Splits a string into a list of lines.
    """
    raw_lines = src.split('\n')
    for i, raw_line in enumerate(raw_lines):
        depth = 0
        while raw_line.startswith(' '):
            depth += 1
            raw_line = raw_line[1:]

        if not raw_line:
            continue

        yield Line(raw_line, i, depth)

        if raw_lines[i+1]
