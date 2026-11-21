PROG=	simple-shell
OBJS=	getinput.o sig.o simple-shell.o

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o ${PROG}

clean:
	rm -f ${PROG} *.o
