# A Makefile to build our 'Mergetars' project

C99     = cc -std=c99
CFLAGS  = -Wall -pedantic -Werror


Mergetars : Mergetars.o Globals.o FileList.o Processes.o
	$(C99) $(CFALGS) -o Mergetars Mergetars.o Globals.o FileList.o Processes.o



Mergetars.o : Mergetars.c Declaration.h
	$(C99) $(CFALGS) -c Mergetars.c


Globals.o : Globals.c Declaration.h
	$(C99) $(CFALGS) -c Globals.c


Processes.o : Processes.c Declaration.h
	$(C99) $(CFALGS) -c Processes.c


FileList.o : FileList.c Declaration.h
	$(C99) $(CFALGS) -c FileList.c




