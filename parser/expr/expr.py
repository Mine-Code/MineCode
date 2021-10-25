class ExprBase:
    def __init__(self):
        pass

    def eval(self):
        raise NotImplementedError

    def __str__(self):
        raise NotImplementedError

    def __repr__(self):
        return self.__str__()
