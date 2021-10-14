module lexer

pub struct Lexer {
pub:
	input string [required]
mut:
	pos   int
	lines []string
}

fn (mut this Lexer) preprocess() {
	this.lines = this.input.split('\n')
}

fn (mut it Lexer) next() ?Token {
	if it.pos > it.lines.len {
		return none
	}

	line := it.lines[it.pos]

	it.pos += 1

	return IdentifierToken{
		val: line
	}
}
