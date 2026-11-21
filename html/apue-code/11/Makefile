CFLAGS=-Wall -Werror -fPIC

help:
	@echo "The following targets are available:"
	@echo "code-clean:  remove all object files, executables, libraries etc."
	@echo "evil:        build an evil dynamic library into lib3/libldtest.so"
	@echo "libldtest:   build a dynamic library into lib/libldtest.so"
	@echo "libldtest2:  build a second dynamic library into lib2/libldtest.so"
	@echo "rpath:       build an executable with a fixed rpath"

code-clean:
	rm -f *.o libldtest* a.out*
	rm -fr lib lib2 lib3

libldtest.a: ldtest1.o ldtest2.o
	ar -vq libldtest.a ldtest1.o ldtest2.o
	ar -t libldtest.a

rpath: lib/libldtest.so
	cc -Wall main.c -L./lib -lldtest -Wl,-rpath,./lib


libldtest: lib/libldtest.so

lib/libldtest.so: ldtest1.o ldtest2.o
	mkdir -p lib
	cc -shared -Wl,-soname,libldtest.so.1 -o lib/libldtest.so.1.0 ldtest1.o ldtest2.o
	ln -sf libldtest.so.1.0 lib/libldtest.so.1
	ln -sf libldtest.so.1 lib/libldtest.so

libldtest2: lib2/libldtest.so

lib2/libldtest.so: ldtest1.2.o ldtest2.o
	mkdir -p lib2
	cc -shared -Wl,-soname,libldtest.so.1 -o lib2/libldtest.so.1.0 ldtest1.2.o ldtest2.o
	ln -sf libldtest.so.1.0 lib2/libldtest.so.1
	ln -sf libldtest.so.1 lib2/libldtest.so

evil: evil.o ldtest1.o ldtest2.o
	mkdir -p lib3
	cc -shared -Wl,-soname,libldtest.so.1 -o lib3/libldtest.so.1.0 ldtest1.o ldtest2.o evil.o
	ln -sf libldtest.so.1.0 lib3/libldtest.so.1
	ln -sf libldtest.so.1 lib3/libldtest.so

libgreet: greet/libgreet.so
	cd greet && make
