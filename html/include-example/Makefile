NAME= foo
OBJS= main.o print.o

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o $@ $?

clean:
	rm -f ${NAME} ${OBJS}
