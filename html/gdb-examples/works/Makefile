OBJS=	main.o primes.o

all: primes

primes: ${OBJS}
	${CC} -g ${OBJS} -o primes -lm

.c.o:
	${CC} -g -c $< -o $@

clean:
	rm -f ${OBJS} primes primes.core
