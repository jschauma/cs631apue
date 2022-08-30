#! /bin/sh
#
# Helper script to start a NetBSD VM using qemu.
#
# See https://stevens.netmeister.org/631/utm/ for more information.
#
echo "=> Starting VM..."
qemu-system-aarch64 \
	-L /Applications/UTM.app/Contents/Resources/qemu  	\
	-smp cpus=2,sockets=1,cores=2,threads=1 		\
	-accel hvf -accel tcg,tb-size=1024     			\
	-m 2048     						\
	-cpu host 						\
	-M virt,highmem=off 					\
	-drive file=/Applications/UTM.app/Contents/Resources/qemu/edk2-aarch64-code.fd,if=pflash,format=raw,readonly=on \
	-drive file=/Users/${USER}/Library/Containers/com.utmapp.UTM/Data/Documents/apue.utm/Images/data.qcow2,format=qcow2 \
	-nographic 						\
	-serial tcp::4444,server,telnet,nowait			\
	-net user,hostfwd=tcp::2222-:22 -net nic >/dev/null 2>&1 &

echo "Waiting for VM to boot..."
n=1
while [ 1 ]; do
	echo | nc localhost 2222 >/dev/null 2>&1
	if [ $? -eq 0 ]; then
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
