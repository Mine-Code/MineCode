from .lexer.lexer import lex
from .parser.parser import Parser

with open("MineCode/tests/alltest", "r") as f:
    src = f.read()

parser = Parser(lex(src))

parser.parse()
