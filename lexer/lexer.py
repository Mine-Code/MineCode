from typing import Iterable
from .line import Line
from .line_splitter import LineSplitter
from .tokenizer import Tokenizer

tokenizer = Tokenizer()


def _lex(lines: Iterable[Line]) -> Iterable[Line]:
    for line in lines:
        tokenizer.set_input(line.line)
        line.tokens = [*tokenizer.tokenize()]
        line.children = [*_lex(line.children)]

        yield line


def lex(src: str) -> Iterable[Line]:
    lines = LineSplitter(src).split()
    return _lex(lines)
