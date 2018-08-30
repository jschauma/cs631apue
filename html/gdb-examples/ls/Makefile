# Almost identical to 'Makefile.2' - use diff to find out what's
# different.  Then use both Makefile.2 and Makefile.4 to illustrate
# precedence of variables.
PROG=	ls
OBJS=	cmp.o ls.o main.o print.o stat_flags.o util.o
LIBS=	-lbsd

# If commented out, defaults will be used. If uncommented, these values
# will be used, even though there is no other explicit mention of this
# variable in this Makefile.
CFLAGS=	-Wall -g

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o ${PROG} ${LIBS}

clean:
	rm -f ls *.o
