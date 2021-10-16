from .lexer.lexer import lex

with open("MineCode/tests/alltest", "r") as f:
    src = f.read()

for line in lex(src):
    print(line)
