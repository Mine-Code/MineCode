from typing import List


class Identifier:
    def __init__(self, name: str, attributes: List[str]):
        self.name = name
        self.attributes = attributes

    def __str__(self):
        return self.name + ".".join(self.attributes)

    def __repr__(self):
        ret = "Identifier("
        ret += f"{self.name}"
        if self.attributes:
            ret += f", {self.attributes}"
        ret += ")"
        return ret
