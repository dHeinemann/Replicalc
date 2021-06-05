CFLAGS = --std c90
LDFLAGS = -lm -lncurses
DBFLAGS = -g -O0 -da -Wall -Wextra

rcalc: builddir ncursesui.o
	$(CC) $(CFLAGS) $(LDFLAGS) src/main.c -o build/rcalc build/*.o

run: rcalc
	./build/rcalc

builddir:
	@mkdir -p build

ncursesui.o: calc.o chartype.o strfun.o
	$(CC) $(CFLAGS) -c src/ncursesui.c -o build/ncursesui.o

calc.o: chartype.o dstack.o strstack.o
	$(CC) $(CFLAGS) -c src/calc.c -o build/calc.o

chartype.o:
	$(CC) $(CFLAGS) -c src/chartype.c -o build/chartype.o

strfun.o:
	$(CC) $(CFLAGS) -c src/strfun.c -o build/strfun.o

dstack.o:
	$(CC) $(CFLAGS) -c src/dstack.c -o build/dstack.o

strstack.o:
	$(CC) $(CFLAGS) -c src/strstack.c -o build/strstack.o

debug: builddir ncursesui.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) src/main.c -o build/rcalc build/*.o

test: debug
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) tests/tests.c -o build/tests build/*.o Unity/src/unity.c
	@./build/tests

clean:
	$(RM) build/*
