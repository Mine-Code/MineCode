module lexer

struct Iterator<T> {
mut:
	input []T [required]

	position usize
}

pub fn (this Iterator<T>) is_out_of_range() bool {
	return this.position >= this.input.len
}

pub fn (mut it Iterator<T>) next() ?T {
	if it.is_out_of_range() {
		return none
	}

	mut ret := it.input[it.position]

	it.position += 1

	return ret
}
