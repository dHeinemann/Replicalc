PDCURSES = ..\pdcurses\dos
CC = wcc
CCL = wcl
CFLAGS = -bt=dos -mm
LDFLAGS = $(PDCURSES)\pdcurses.lib

rcalc: cursesui.o
	$(CCL) ..\src\main.c -fe=rcalc.exe $(CFLAGS) *.o $(LDFLAGS) -i="..\pdcurses"

run: rcalc
	rcalc.exe

cursesui.o: calc.o chartype.o strfun.o history.o pdcurses
	$(CC) ..\src\cursesui.c -fo=cursesui.o $(CFLAGS) -i="..\pdcurses"

calc.o: chartype.o dstack.o strstack.o
	$(CC) ..\src\calc.c -fo=calc.o $(CFLAGS)

chartype.o:
	$(CC) ..\src\chartype.c -fo=chartype.o $(CFLAGS)

strfun.o:
	$(CC) ..\src\strfun.c -fo=strfun.o $(CFLAGS)

dstack.o:
	$(CC) ..\src\dstack.c -fo=dstack.o $(CFLAGS)

strstack.o:
	$(CC) ..\src\strstack.c -fo=strstack.o $(CFLAGS)

history.o:
	$(CC) ..\src\history.c -fo=history.o $(CFLAGS)

.NOCHECK
pdcurses:
	cd $(PDCURSES)
	wmake -f makefile.wcc MODEL=m
	cd ..\..\dos

clean:
	rm *.o
	rm *.exe
	rm *.err

cleancurses:
	rm $(PDCURSES)\*.obj
	rm $(PDCURSES)\*.lib
