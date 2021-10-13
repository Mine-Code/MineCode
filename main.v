import lexer
import os

fn main() {
	src := os.read_file('tests/alltest') or { panic("Can't open file") }
	lexer := lexer.new(src)

	for tok in lexer {
	}
}
