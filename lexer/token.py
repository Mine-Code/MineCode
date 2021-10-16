class Token:
    def __str__(self):
        return repr(self)


class NumberToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"Number({self.value})"


class StringToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"String({self.value})"


class IdentifierToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"Identifier({self.value})"


class NewLineToken:
    def __repr__(self):
        return "NewLine"


class IndentToken:
    def __init__(self, depth: int):
        self.depth = depth

    def __repr__(self) -> str:
        return f"Indent({self.depth})"


class OperatorToken:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return f"Operator({self.value})"
