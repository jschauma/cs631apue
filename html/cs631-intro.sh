#! /bin/sh
#
# Copyright (c) 2013, Jan Schaumann. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    1. Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# Originally written by Jan Schaumann <jschauma@netmeister.org> in August
# 2013.
#
# The class CS631 Advanced Programming in the Unix Environment
# (http://www.cs.stevens.edu/~jschauma/631/) has no strict pre-requisites,
# but I need to assess whether a prospective student might be completely
# lost and not likely to be able to perform even the most basic of tasks
# required to follow the course.  I use this script to see if students are
# able to operate within a unix-like environment and know how to at least
# compile a simple program.
#
# The script prompts the user to perform certain tasks; all interactions
# are logged via script(1), and the output is emailed to the instructor.

NAME="CS631-Intro"

INSTRUCTOR="jschauma"
DOMAIN="stevens.edu"
SCRIPT_DIR="$(mktemp -d "${TMPDIR:-/tmp}/${NAME}.${USER}.XXXX")"
MYCWD="$(pwd)"

trap "cleanup" 0

cleanup() {
	rm -fr "${SCRIPT_DIR}"
}

submitOutput() {
	mutt -n -s "${USER} completed ${NAME}"	\
		-a "${SCRIPT_DIR}/${NAME}.1" 	\
		-a "${SCRIPT_DIR}/${NAME}.2"	\
		-a "${SCRIPT_DIR}/${NAME}.3"	\
		-a "${SCRIPT_DIR}/${NAME}.4"	\
		-a "${SCRIPT_DIR}/${NAME}.5"	\
		-a "${SCRIPT_DIR}/${NAME}.6"	\
		-a "${SCRIPT_DIR}/${NAME}.7"	\
		-a "${SCRIPT_DIR}/${NAME}.8"	\
		-a "${SCRIPT_DIR}/${NAME}.10" 	\
		-- ${INSTRUCTOR}@${DOMAIN} </dev/null >/dev/null 2>&1
}

task() {
	local readonly n="${1}"

	echo
	echo "When you are done, just type 'exit'."
	echo
	cd intro-${USER} 2>/dev/null
	script -q "${SCRIPT_DIR}/${NAME}.${n}"
	cd ${MYCWD}
	date
	clear
}

###
### Main
###

if ! expr "$(hostname -f)" : ".*srcit.stevens-tech.edu" >/dev/null 2>&1; then
	echo "Please run this script on linux-lab.cs.stevens.edu." >&2
	exit 1
fi


## Intro

cat <<EOF
Hello, ${USER}!

This little script will ask you to perform a few simple tasks.  Don't
worry, this is not a test and will not be graded -- it is merely to give
your instructor an idea of your level of confidence on our Unix systems.

This should not take you very long.  You should not need to look up
anything on the internet.  All the information you would need should be
available to you via the manual pages on the system.  Sometimes, the
instructions will suggest some relevant commands by referring to the
specific manual pages.

You also should not have a need to open another terminal.  All commands
that you execute should be run from within *this* login session.  (This
script logs which commands you use to give your instructor an idea of how
familiar you are with the environment.  Do not worry about completing this
flawlessly or without trying out a few things.)

Every task will start a new shell for you.  When you are finished with the
given task, just type 'exit'.

EOF

read -p "If you are ready to proceed, just hit return now." null

date
clear

## echo

cat <<EOF
Ok, let's get started.

Your current username is stored in the \$USER environment variable.
Display its value. (See: env(1), echo(1))
EOF

task 1

## mkdir

cat <<EOF

Alright, now create a new directory called 'intro-${USER}', change into
that directory and display the pathname of the current working directory.
(See: mkdir(1), cd(1), pwd(1))
EOF

task 2

## write hello-world

cat <<EOF

Create a new file called 'hello-cs631.c' containing the famous "Hello
World" program in the C programming language. (See: vi(1), emacs(1),
nano(1), pico(1))

Just type 'exit' when you have finished writing the file.  Feel free to
compile, debug, run the program or just jot down the code if you can.
EOF

task 3

## compile

cat <<EOF

Compile the program into an executable. (See: gcc(1))
EOF

task 4

## ldd

cat <<EOF

Display the shared libraries the program is linked against. (See: ldd(1))
EOF

task 5

## run the program

cat <<EOF

Run the program.
EOF

task 6

## wget

cat <<EOF

Fetch the sample program 'simple-cat.c' from
http://www.cs.stevens.edu/~jschauma/631/simple-cat.c. (See: curl(1),
wget(1))
EOF

task 7

## compile into output binary

cat <<EOF

Compile the program into an executable named 'cat'. (See: gcc(1))
EOF

task 8

## run cat

cat <<EOF

Use the 'cat' executable to display the source code for your hello-cs631
program.
EOF

task 9

## tar it up

cat <<EOF

Change into the current parent directory and create a tar archive of the
'intro-$USER' directory, named 'intro-$USER.tar'. (See: tar(1))
EOF

task 10

## That's all, folks!
submitOutput

cat <<EOF

Alright, that was all!  See, that wasn't so bad, now was it?
If you wish, you can remove the directory 'intro-$USER' now.

Goodbye.
EOF
