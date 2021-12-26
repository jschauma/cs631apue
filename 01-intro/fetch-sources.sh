#! /bin/sh
set -eu

umask 022

for set in gnusrc sharesrc src syssrc; do
	echo "Fetching ${set}..."
	ftp -V ftp.netbsd.org:/pub/NetBSD/NetBSD-9.0/source/sets/${set}.tgz
	echo "Extracting ${set}..."
	su root -c "tar zxf ${set}.tgz -C /"
	rm -f "${set}.tgz"
done
