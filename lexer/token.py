class Token:
    def __str__(self):
        return repr(self)


class NumberToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"Number({self.value})"


class OperatorToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"Operator({self.value})"
