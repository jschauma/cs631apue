#! /bin/sh
#
# curl --http1.0

SERVER="${1:?"Usage: $0 server port"}"
PORT="${2:?"Usage: $0 server port"}"

URIS="/cgi-bin/env.cgi
/cgi-bin/env.cgi?q=foo&food=bacon
/cgi-bin/post.cgi
/dir/file
/dir
/dir/
/dir/.
/dir2/
/file
/file2
/file space
/file>path
/file%%20space%%20%%3C%%20path%%20%%3E%%20name
/ls
/notallowed
no-leading-slash
/../../../../../../../../../etc/passwd
/dir/file/../../dir/file
/dir/./file
/~jschauma/
/~jschauma/oink
/~jschauma/d/f
/~nobody
/nowhere
/passwd
"

PROTOCOLS="
HTTP/0.9
HTTP/1.0
HTTP/1.0000
HTTP/1.1
HTTP/2.0
HTTP
HTTP/10000000000000000000000000000000000000000000000000000
BACON
BACON/3.2
"

REQUESTS="
GET
HEAD
POST
DELETE
BACON
"
protocols() {
	for P in ${PROTOCOLS}; do
		echo "=> GET / ${P}"
		printf "GET / ${P}\r\n\r\n" | nc ${SERVER} ${PORT}
	done
	echo "=> GET /"
	printf "GET /\r\n\r\n" | nc ${SERVER} ${PORT}
}

requests() {
	for R in ${REQUESTS}; do
		echo "=> ${R} / HTTP/1.0"
		printf "${R} / HTTP/1.0\r\n\r\n"  | nc ${SERVER} ${PORT}
	done
	echo "=>  / HTTP/1.0"
	printf "  / HTTP/1.0\r\n\r\n"  | nc ${SERVER} ${PORT}
}


uris() {
	IFS='
'
	for U in ${URIS}; do
		echo "=> GET '${U}' HTTP/1.0"
		printf "GET ${U} HTTP/1.0\r\n\r\n"  | nc ${SERVER} ${PORT}
	done
	echo "=> GET HTTP/1.0"
	printf "GET HTTP/1.0\r\n\r\n"  | nc ${SERVER} ${PORT}
}

cleanup() {
	pkill telnet
}

trap 'cleanup' 0

protocols
requests
uris

pid=$$
( sleep 300; kill -s ALRM $pid ) &

telnet ${SERVER} ${PORT} >/dev/null
