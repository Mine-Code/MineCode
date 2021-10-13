module lexer

pub enum TokenKind {
	// Base Tokens
	tok_integer
	tok_float
	tok_string
	tok_identifier
	// Keywords
	tok_for
	tok_if
	tok_else
	tok_while
	tok_return
	tok_break
}

pub struct Token {
pub:
	kind TokenKind
}

fn a() {
}
