all: libdecocolour-hack.so

%.o: %.c
	echo $(GTK)
	gcc -fPIC -c -Wall $*.c -o $*.o

libdecocolour-hack.so: preload.o
	gcc -shared preload.o -ldl -o libdecocolour-hack.so

clean:
	rm -f *.o *.so
