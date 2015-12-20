all: libgreet.so

libgreet.so: greet.c
	cc -Wall -c -fPIC greet.c -o greet.o
	cc -shared -Wl,-soname,libgreet.so.1 -o libgreet.so.1.0 greet.o
	ln -sf libgreet.so.1.0 libgreet.so.1
	ln -sf libgreet.so.1 libgreet.so

clean:
	rm -f greet.o libgreet.so libgreet.so.1 libgreet.so.1.0
