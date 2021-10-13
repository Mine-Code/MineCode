module lexer

struct Lexer {
mut:
	input    []rune [required]
	position usize
	ch       rune
}

pub fn new(input string) Lexer {
	return Lexer{
		input: input.runes()
	}
}

fn (this Lexer) is_out_of_range() bool {
	return this.position >= this.input.len
}

fn (mut it Lexer) next_char() {
	if it.is_out_of_range() {
		it.ch = -1
	} else {
		it.ch = it.input[it.position]
	}
	it.position += 1
}

fn (mut it Lexer) next() ?string {
	if it.is_out_of_range() {
		return none
	}

	mut ret := ''

	for it.ch != `\n` && !it.is_out_of_range() {
		ret += it.ch.str()
		it.next_char()
	}

	it.next_char() // skip endline

	return ret
}
