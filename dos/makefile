CFLAGS = -bt=dos -ml
LDFLAGS = ..\pdcurses\dos\pdcurses.lib


rcalc: cursesui.o
	wcl ..\src\main.c -fe=rcalc.exe $(CFLAGS) *.o $(LDFLAGS) -i="..\pdcurses"

run: rcalc
	rcalc.exe

cursesui.o: calc.o chartype.o strfun.o pdcurses
	wcc ..\src\cursesui.c -fo=cursesui.o $(CFLAGS) -i="..\pdcurses"

calc.o: chartype.o dstack.o strstack.o
	wcc ..\src\calc.c -fo=calc.o $(CFLAGS)

chartype.o:
	wcc ..\src\chartype.c -fo=chartype.o $(CFLAGS)

strfun.o:
	wcc ..\src\strfun.c -fo=strfun.o $(CFLAGS)

dstack.o:
	wcc ..\src\dstack.c -fo=dstack.o $(CFLAGS)

strstack.o:
	wcc ..\src\strstack.c -fo=strstack.o $(CFLAGS)

.NOCHECK
pdcurses:
	cd ..\pdcurses\dos
	wmake -f makefile.wcc
	cd ..\..\dos

clean:
	rm *.o
	rm *.exe
	rm *.err
	rm ..\pdcurses\dos\*.obj
	rm ..\pdcurses\dos\*.lib
