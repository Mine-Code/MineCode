module lexer

struct Lexer {
	input         string
	position      usize
	read_position usize
	ch            rune
}

fn new(input string) Lexer {
	return Lexer{
		input: input
		position: 0
		read_position: 0
		ch: ` `
	}
}
