from .lexer.lexer import Lexer
from .lexer.line_splitter import lineSplitter

with open("MineCode/tests/alltest", "r") as f:
    src = f.read()

for line in lineSplitter(src):
    print(line)
