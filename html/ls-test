#! /bin/sh
#set -e

COMMANDS="./ls
./ls -l
./ls -la
./ls -lai
./ls -lairt
./ls -lairtus
./ls -d
./ls -d .
./ls -d . .. /
./ls -n /home
./ls -l /dev
./ls -lsh
./ls -lF
./ls -A ~jschauma/apue/ls-test
./ls -w ~jschauma/apue/ls-test
./ls ~jschauma/apue/ls-test | more
BLOCKSIZE=bacon ./ls -ls
BLOCKSIZE=0 ./ls -ls
BLOCKSIZE=2048 ./ls -ls
BLOCKSIZE=-50 ./ls -ls
BLOCKSIZE=50 ./ls -ls
TZ=PST8PDT ./ls -lc
TZ=bacon ./ls -lc
./ls -lks
./ls / /tmp ~ .
./ls -?
./ls /does/not/exit
./ls /nowhere"

IFS="
"

for c in ${COMMANDS}; do
	echo ${c}
	timeout --foreground 60 sh -c "eval ${c}" || echo timed out
done

echo "./ls -lR /"
timeout 600 ./ls -lR / || echo timed out
