CC = gcc
CFLAGS = -g
IFLAGS = -I. -I/usr/lib/gcc/x86_64-linux-gnu/7/include

all: et_crt ep_crt para_mm

para_mm: para_mm.o timing.o
	gcc -pthread para_mm.o timing.o -o para_mm
et_crt: et.o timing.o
	gcc -pthread et.o timing.o -o et_crt
ep_crt: ep.o timing.o
	gcc  ep.o timing.o -o ep_crt
para_mm.o: para_mm.c
	gcc -c para_mm.c
ep.o: ep.c
	gcc -c ep.c
et.o: et.c
	gcc -c et.c
timing.o: timing.c
	gcc -c timing.c
clean:
	rm *.o et_crt ep_crt para_mm.c

depend:
	makedepend ${IFLAGS} ep.c et.c timing.c
