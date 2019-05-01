CC = gcc
CFLAGS =  -g
IFLAGS = -I. -I/usr/lib/gcc/x86_64-linux-gnu/7/include

hw1: main.o pie.o
	gcc -o hw1 main.o pie.o

clean:
	rm hw1 main.o pie.o
 
depend:
	makedepend ${IFLAGS} main.c pie.c 
# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT

main.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
main.o: /usr/include/features.h /usr/include/stdc-predef.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stddef.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/bits/types/__FILE.h /usr/include/bits/types/FILE.h
main.o: /usr/include/bits/libio.h /usr/include/bits/_G_config.h
main.o: /usr/include/bits/types/__mbstate_t.h
main.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stdarg.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pie.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
pie.o: /usr/include/features.h /usr/include/stdc-predef.h
pie.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pie.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
pie.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stddef.h
pie.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pie.o: /usr/include/bits/types/__FILE.h /usr/include/bits/types/FILE.h
pie.o: /usr/include/bits/libio.h /usr/include/bits/_G_config.h
pie.o: /usr/include/bits/types/__mbstate_t.h
pie.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stdarg.h
pie.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pie.o: /usr/include/string.h /usr/include/bits/types/locale_t.h
pie.o: /usr/include/bits/types/__locale_t.h /usr/include/strings.h
