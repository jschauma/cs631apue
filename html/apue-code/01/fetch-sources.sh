# This file is part of the sample code and exercises
# used by the class "Advanced Programming in the UNIX
# Environment" taught by Jan Schaumann
# <jschauma@netmeister.org> at Stevens Institute of
# Technology.
#
# This file is in the public domain.
#
# You don't have to, but if you feel like
# acknowledging where you got this code, you may
# reference me by name, email address, or point
# people to the course website:
# https://stevens.netmeister.org/631/

#! /bin/sh
set -eu

umask 022

for set in gnusrc sharesrc src syssrc; do
	echo "Fetching ${set}..."
	ftp -V ftp.netbsd.org:/pub/NetBSD/NetBSD-11.0/source/sets/${set}.tgz
	echo "Extracting ${set}..."
	su root -c "tar zxf ${set}.tgz -C /"
	rm -f "${set}.tgz"
done

echo "Chowning sources to ${USER}..."
su root -c "chown -R ${USER} /usr/src"
