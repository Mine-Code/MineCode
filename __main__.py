from .lexer import token

print(
    token.NumberToken(1),
    token.OperatorToken('+'),
)
