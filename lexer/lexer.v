module lexer

struct Line {
	line   string
	number int
	depth  int
}

pub struct Lexer {
pub:
	input string [required]
mut:
	pos   int
	lines []Line
}

pub fn (mut this Lexer) preprocess() {
	for i, line in this.input.split('\n') {
		this.lines << Line{
			line: line
			number: i
			depth: 0
		}
	}
}

fn (mut it Lexer) next() ?Token {
	if it.pos >= it.lines.len {
		return none
	}

	line := it.lines[it.pos]

	it.pos += 1

	return IdentifierToken{
		val: '$line.number| $line.depth> $line.line'
	}
}
