from .lexer.tokenizer import Tokenizer

# with open("MineCode/tests/alltest", "r") as f:
#     src = f.read()
#
# splitter = LineSplitter(src)
# for line in splitter.split():
#     print(line)

tokenizer = Tokenizer()
tokenizer.set_input('"hello" 123 1.23 0.123 0x123 0o123 0b110')
for token in tokenizer.tokenize():
    print(token)
