module lexer

struct LineSplitter {
mut:
	iter Iterator<string>
}

pub fn (mut this LineSplitter) next() ?string {
	mut ch := this.iter.next() or { return none }

	mut line := ''
	for ch != '\n' {
		ch = this.iter.next() or { break }
		line += ch
	}
	return line
}
