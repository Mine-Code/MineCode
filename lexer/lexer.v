module lexer

struct Lexer {
mut:
	input         string
	position      usize
	read_position usize
	ch            rune
}

pub fn new(input string) Lexer {
	return Lexer{
		input: input
		position: 0
	}
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
