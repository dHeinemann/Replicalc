CFLAGS = --std c90
LDFLAGS = -lm -lncurses
DBFLAGS = -g -O0 -da -Wall -Wextra

ifeq ($(MAKECMDGOALS), debug)
FLAGS = $(CFLAGS) $(DBFLAGS)
else
FLAGS = $(CFLAGS)
endif

rcalc: cursesui.o
	$(CC) $(FLAGS) $(LDFLAGS) ../src/main.c -o rcalc *.o

run: rcalc
	./rcalc

cursesui.o: calc.o chartype.o strfun.o history.o
	$(CC) $(FLAGS) -c ../src/cursesui.c -o cursesui.o

calc.o: chartype.o dstack.o strstack.o
	$(CC) $(FLAGS) -c ../src/calc.c -o calc.o

chartype.o:
	$(CC) $(FLAGS) -c ../src/chartype.c -o chartype.o

strfun.o:
	$(CC) $(FLAGS) -c ../src/strfun.c -o strfun.o

dstack.o:
	$(CC) $(FLAGS) -c ../src/dstack.c -o dstack.o

strstack.o:
	$(CC) $(FLAGS) -c ../src/strstack.c -o strstack.o

history.o:
	$(CC) $(FLAGS) -c ../src/history.c -o history.o

debug: rcalc

test: debug
	$(CC) $(FLAGS) $(LDFLAGS) $(DBFLAGS) ../tests/tests.c -o tests *.o ../Unity/src/unity.c
	@./tests

clean:
	$(RM) *.o
	$(RM) rcalc
