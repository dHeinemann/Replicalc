CFLAGS = --std c90
LDFLAGS = -lm -lncurses
DBFLAGS = -g -O0 -da -Wall -Wextra

rcalc: cursesui.o
	$(CC) $(CFLAGS) $(LDFLAGS) ../src/main.c -o rcalc *.o

run: rcalc
	./rcalc

cursesui.o: calc.o chartype.o strfun.o
	$(CC) $(CFLAGS) -c ../src/cursesui.c -o cursesui.o

calc.o: chartype.o dstack.o strstack.o
	$(CC) $(CFLAGS) -c ../src/calc.c -o calc.o

chartype.o:
	$(CC) $(CFLAGS) -c ../src/chartype.c -o chartype.o

strfun.o:
	$(CC) $(CFLAGS) -c ../src/strfun.c -o strfun.o

dstack.o:
	$(CC) $(CFLAGS) -c ../src/dstack.c -o dstack.o

strstack.o:
	$(CC) $(CFLAGS) -c ../src/strstack.c -o strstack.o

debug: cursesui.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) ../src/main.c -o rcalc *.o

test: debug
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) ../tests/tests.c -o tests *.o ../Unity/src/unity.c
	@./tests

clean:
	$(RM) *.o
	$(RM) rcalc
