import types

fn main() {
	a := types.Program{
		do_once: false
	}
	executor := Executor{
		program: a
	}
	executor.execute()
}
