module lexer

pub struct NumberToken {
pub:
	val f64
}

pub struct StringToken {
pub:
	val string
}

pub struct IdentifierToken {
pub:
	val string
}

pub enum Keyword {
	kw_for
	kw_if
	kw_else
	kw_while
	kw_return
	kw_break
}

pub struct KeywordToken {
pub:
	val Keyword
}

pub struct OperatorToken {
pub:
	val string
}

type Token = IdentifierToken | KeywordToken | NumberToken | OperatorToken | StringToken

fn a() {
}
