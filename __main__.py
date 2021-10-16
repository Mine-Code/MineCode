from .lexer.lexer import Lexer
from .lexer.line_splitter import LineSplitter

with open("MineCode/tests/alltest", "r") as f:
    src = f.read()

splitter = LineSplitter(src)
for line in splitter.split():
    pass
    print(line)
