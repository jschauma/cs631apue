# A slightly better Makefile.  As the first one, but much shorter, since
# we're using the various built-in rules and definitions in make(1).
#
# also see "cpp -M" and "cpp -MM"

PROG=	ls
OBJS=	cmp.o ls.o main.o print.o stat_flags.o util.o

# If commented out, defaults will be used. If uncommented, these values
# will be used, even though there is no other explicit mention of this
# variable in this Makefile.
#CFLAGS=	-Wall -g

all: ${PROG}

showvars:
	@echo "CC=${CC}"
	@echo "CFLAGS=${CFLAGS}"
	@echo "LDFLAGS=${LDFLAGS}"
	@echo "PROG=${PROG}"
	@echo "OBJS=${OBJS}"

${PROG}: ${OBJS}
	@echo $@ depends on $?
	${CC} ${LDFLAGS} ${OBJS} -o ${PROG}

clean:
	rm -f ls *.o
