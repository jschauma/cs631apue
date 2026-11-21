# A slightly better Makefile.  Let's define a "suffix rule" to
# create object files from source files.
# Let's also introduce the concept of variables.

PROG=	ls
OBJS=	cmp.o ls.o main.o print.o stat_flags.o util.o
CFLAGS=	-Wall -g

all: ${PROG}

depend:
	mkdep -- ${CFLAGS} *.c

${PROG}: ${OBJS}
	@echo $@ depends on $?
	${CC} ${OBJS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG} ${OBJS}
