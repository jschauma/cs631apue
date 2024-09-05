#! /bin/sh
#
# Helper script to start a NetBSD VM using qemu.
#
# See https://stevens.netmeister.org/631/utm/ for more information.

which timeout >/dev/null 2>&1
if [ $? -gt 0 ]; then
	# e.g., macOS; we assume Perl is present
	timeout() { perl -e 'alarm shift; exec @ARGV' "$@"; }
fi


echo "=> Starting VM..."
drives=""

for f in /Users/${USER}/Library/Containers/com.utmapp.UTM/Data/Documents/apue.utm/Data/*.qcow2; do
	drives="${drives} -drive file=${f},format=qcow2"
done

qemu-system-aarch64 \
	-L /Applications/UTM.app/Contents/Resources/qemu  	\
	-smp cpus=4,sockets=1,cores=4,threads=1 		\
	-accel hvf -accel tcg,tb-size=1024     			\
	-m 2048     						\
	-cpu host 						\
	-M virt,highmem=off 					\
	-drive file=/Applications/UTM.app/Contents/Resources/qemu/edk2-aarch64-code.fd,if=pflash,format=raw,readonly=on         		\
	${drives} 						\
	-nographic 						\
	-serial tcp::4444,server,telnet,nowait 			\
	-net user,hostfwd=tcp::2222-:22 -net nic >/dev/null 2>&1 &

/bin/echo -n "==> Waiting for VM to boot..."
n=1
while [ 1 ]; do
	timeout 2 /bin/sh -c "echo | nc localhost 2222 >/dev/null 2>&1" >/dev/null 2>&1
	if [ $? -eq 0 ]; then
		# we still need a few more seconds
		/bin/echo -n "."
		sleep 3
		echo;
		echo "=> VM up and ready to log in."
		break
	fi
	sleep 1
	/bin/echo -n "."
	n=$(( n + 1 ))
	if [ $n -gt 60 ]; then
		echo "VM still not up after 60 seconds, bailing out." >&2
		break
	fi
done
