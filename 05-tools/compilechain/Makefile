all: a.out

cpp: preprocess

preprocess: hello.i

hello.i: hello.c
	cpp -P $> >$@

compile: hello.s

hello.s: hello.i
	cc -S $> -o $@

assemble: hello.o

hello.o: hello.s
	as -o $@ $<

link:	a.out

a.out: hello.o
	ld -dynamic-linker /usr/libexec/ld.elf_so /usr/lib/crt0.o	\
		/usr/lib/crti.o /usr/lib/crtbegin.o hello.o 		\
		-lc /usr/lib/crtend.o /usr/lib/crtn.o



clean:
	rm -f hello.i hello.o hello.s a.out
