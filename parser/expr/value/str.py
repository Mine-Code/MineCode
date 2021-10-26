from .value import Value


class String(Value):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return f'"{self.value}"'

    def __repr__(self):
        return f'String("{self.value}")'
