#! /bin/sh
#
# Helper script to start a NetBSD VM using qemu.
#
# See https://stevens.netmeister.org/631/utm/ for more information.
#

# timeout(1) isn't necessarily present on macOS
tout="$(which timeout 2>/dev/null)"
if [ -z "${tout}" ]; then
	function timeout() { perl -e 'alarm shift; exec @ARGV' "$@"; }
fi


echo "=> Starting VM..."
qemu-system-aarch64 \
	-L /Applications/UTM.app/Contents/Resources/qemu  	\
	-smp cpus=4,sockets=1,cores=4,threads=1 		\
	-accel hvf -accel tcg,tb-size=1024     			\
	-m 2048     						\
	-cpu host 						\
	-M virt,highmem=off 					\
	-drive file=/Applications/UTM.app/Contents/Resources/qemu/edk2-aarch64-code.fd,if=pflash,format=raw,readonly=on         		\
	-drive file=/Users/${USER}/Library/Containers/com.utmapp.UTM/Data/Documents/apue.utm/Images/data.qcow2,format=qcow2     				\
	-nographic \
	-serial tcp::4444,server,telnet,nowait \
	-net user,hostfwd=tcp::2222-:22 -net nic >/dev/null 2>&1 &

/bin/echo -n "==> Waiting for VM to boot..."
n=1
while [ 1 ]; do
	timeout 2 "echo | nc localhost 2222 >/dev/null 2>&1" >/dev/null 2>&1
	if [ $? -eq 0 ]; then
		# we still need one or two more seconds
		/bin/echo -n "."
		sleep 2
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
