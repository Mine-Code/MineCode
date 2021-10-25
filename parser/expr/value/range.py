from typing import Union
from .value import Value

_Range = Union[]


class Range(Value):

    def __init__(self, start, end):
        self.start = start
        self.end = end
