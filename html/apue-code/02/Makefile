catio: tmpfiles simple-cat.c
	@i=0; for n in 3145728 1048576 32768 16384 4096 1024 256 128 64 1; do		\
		echo "BUFFSIZE = $$n";					\
		cc -Wall -DBUFFSIZE=$$n simple-cat.c;			\
		i=$$(( $$i + 1 ));					\
		for j in $$(jot 5); do					\
			time ./a.out <tmp/file$$i >tmp/out;			\
		done; 							\
		echo;							\
		echo;							\
	done;

clean:
	rm -fr tmp

tmpfiles: tmp/file9

tmp/file9:
	@mkdir -p tmp
	@for n in $$(jot 10); do					\
		echo "Creating file number $$n...";			\
		dd if=/dev/urandom of=tmp/file$$n bs=$$(( 1024 * 1024 )) count=100 2>/dev/null;\
	done;

sync: tmpfiles scat
	time ./scat <tmp/file9 >out

scat: sync-cat.c
	cc -Wall sync-cat.c -o scat


async: tmpfiles ascat
	time ./ascat <tmp/file9 >out

ascat: async-cat.c
	cc -Wall $? -o $@

async-cat.c: sync-cat.c
	sed -e 's|\(.*O_SYNC.*\)|//\1|' sync-cat.c > async-cat.c
