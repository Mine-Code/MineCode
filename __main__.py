from .lexer.tokenizer import Tokenizer

with open("MineCode/tests/alltest", "r") as f:
    src = f.read()
#
# splitter = LineSplitter(src)
# for line in splitter.split():
#     print(line)

tokenizer = Tokenizer()
tokenizer.set_input(src)
for token in tokenizer.tokenize():
    print(token)
