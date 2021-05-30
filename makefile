CC = gcc
cflags = --std c90 -lm -lncurses
debug_flags = -O0 -da -Wall -Wextra

deps = src/dstack.c src/strstack.c src/calc.c src/chartype.c src/strfun.c src/ncursesui.c
test_deps = $(deps) Unity/src/unity.c

rcalc:
	@mkdir -p build
	$(CC) src/main.c -o build/rcalc $(deps) $(cflags)

debug:
	@mkdir -p build
	$(CC) -g src/main.c -o build/rcalc $(deps) $(cflags) $(debug_flags)

test:
	@mkdir -p build
	$(CC) tests/tests.c -o build/tests $(test_deps) $(cflags) $(debug_flags)
	@./build/tests

clean:
	@rm build/rcalc build/tests