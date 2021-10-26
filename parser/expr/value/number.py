from .value import Value


class Number(Value):
    def __init__(self, value: float):
        self.value = value

    def __str__(self):
        return str(self.value)

    def __repr__(self):
        return f'Number({self.value})'
