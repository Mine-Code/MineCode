import lexer
import os

fn main() {
	src := os.read_file('tests/alltest') or { panic("Can't open file") }
	mut lexer := lexer.Lexer{
		input: src
	}
	lexer.preprocess()

	for tok in lexer {
		println(tok)
	}
}
