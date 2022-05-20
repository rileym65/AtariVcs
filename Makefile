PROJECT=vcs
CC=gcc
DEFS=
INCDIR=-I../
INCS=
LIBDIR=
LIBS=-lX11
OBJS=\
	cpu6502.o \
	debugger.o \
	disassem.o \
	loadfile.o \
	main.o \
	pia.o \
	tia.o

$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(OBJS) $(LIBDIR) $(LIBS) -o $(PROJECT)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm *.o
	-rm $(PROJECT)

cpu6502.o:      header.h cpu6502.h cpu6502.c
debugger.o:     header.h debugger.c
disassem.o:     header.h disassem.c
loadfile.o:     header.h loadfile.c
main.o:         header.h cpu6502.h tia.h main.c
pia.o:          header.h pia.h pia.c
tia.o:          header.h tia.h tia.c
