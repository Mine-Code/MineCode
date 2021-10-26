from typing import Union
from .value import Value
from .. import value

_Range = Union[
    value.Pointer,
    value.Identifier,
    value.Identifier,
    value.Number,
    value.FuncCall,
    value.Put
]


class Range(Value):

    def __init__(self, start: _Range, end: _Range):
        self.start = start
        self.end = end

    def __str__(self):
        return f'({self.start})...({self.end})'

    def __repr__(self):
        return f'Range({self.start}, {self.end})'
