from .value import Value
from .identifier import Identifier


class FuncCall(Value):
    pass


class RawFuncCall(Value):
    # TODO(syoch): annotation 'args'
    def __init__(self, address: int, args):
        self.address = address
        self.args = args

    def __str__(self):
        return f"func[{self.address}]{self.args}"

    def __repr__(self):
        return f"RawFuncCall(address={self.address}, args={repr(self.args)})"


class NamedFuncCall(Value):
    # TODO(syoch): annotation 'args'
    def __init__(self, address: Identifier, args):
        self.name = address
        self.args = args

    def __str__(self):
        return f"{self.name}{self.args}"

    def __repr__(self):
        return f"NamedFuncCall(name={repr(self.name)}, args={repr(self.args)})"
