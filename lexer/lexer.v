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

fn (mut it Lexer) next_char() {
	if it.position >= it.input.len {
		it.ch = 0
	} else {
		it.ch = it.input[it.position]
	}
	it.position += 1
}

fn (mut it Lexer) next() ?string {
	if it.position >= it.input.len {
		return none
	}

	ch := it.input[it.position]
	print('${ch:c}')
	it.position += 1

	return ''
}
