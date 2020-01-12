#! /bin/sh
#
# This script runs a number of tests to verify that the program specified
# via "-p" can copy a regular file.

#set -eu

###
### Globals
###

# to be set via getopts
CP=""
# temporary directory created below
DIR=""
FAILED=0
NFAIL=0
PROGNAME=${0##/}
TOTAL=0
VERBOSE=0

TEST_FILE="/etc/passwd"

###
### Functions
###

# purpose : verify that the program copies files correctly
# inputs  : none
# outputs : none if successful, error messages otherwise
checkCopying() {
	verbose "Checking copying..."

	checkCopyFailures
	checkCopySuccesses
}

# purpose : verify that the program fails when copying due to illegal args
# inputs  : none
# outputs : none if successful, error messages otherwise
checkCopyFailures() {
	local arg cmd
	local dir_to_file dir_to_dir file_to_subdir same socket_to_file

	verbose "Checking copy failures..." 2

	dir_to_file=". file"
	dir_to_dir="/ ."
	file_to_subdir="${TEST_FILE} ./sub/dir"
	socket_to_file="/var/run/log log"

	cd ${DIR}
	for arg in dir_to_file dir_to_dir file_to_subdir socket_to_file; do
		TOTAL=$(( ${TOTAL} + 1 ))
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${cmd}" 1
	done

	cp ${TEST_FILE} file

	runTest "${CP} file file1 file2 file3 file4" 1

	ln file file2
	cmd="${CP} file file2"
	TOTAL=$(( ${TOTAL} + 1 ))
	runTest "${cmd}" 1
	TOTAL=$(( ${TOTAL} + 1 ))
	# must not truncate/modify existing file
	compareFiles ${TEST_FILE} file

	# repeat to check path construction
	cp ${TEST_FILE} file
	cmd="${CP} file ."
	TOTAL=$(( ${TOTAL} + 1 ))
	runTest "${cmd}" 1
	TOTAL=$(( ${TOTAL} + 1 ))
	# must not truncate/modify existing file
	compareFiles ${TEST_FILE} file
}

# purpose : verify that the program succeeds when it should
# inputs  : none
# outputs : none if successful, error messages otherwise
checkCopySuccesses() {
	local arg cmd
	local ro_file_to_file file_to_existing abs_file_to_dir rel_file_to_dir abs_file_to_subdir file_to_slash_dir
	local zero big

	verbose "Checking copy successes..." 2

	ro_file_to_file="${TEST_FILE} file"
	rw_file_to_file="${TMPDIR:-/tmp}/f file"
	file_to_existing="${TEST_FILE} ${TMPDIR:-/tmp}/f"
	abs_file_to_dir="${TEST_FILE} ."
	rel_file_to_dir="g ./sub/dir/."
	abs_file_to_subdir="${TEST_FILE} ./sub/dir/."
	file_to_slash_dir="${TEST_FILE} ../$(basename $(pwd))/"
	zero="zero file"
	big="big file"

	mkdir -p ./sub/dir
	cp ${TEST_FILE} ${TMPDIR:-/tmp}/f
	cp ${TEST_FILE} ./g

	cd ${DIR}
	for arg in ro_file_to_file rw_file_to_file abs_file_to_dir \
		rel_file_to_dir abs_file_to_subdir file_to_existing \
		file_to_slash_dir zero big; do
		verbose "Test case: ${arg}..." 3
		rm -f file >/dev/null 2>&1
		TOTAL=$(( ${TOTAL} + 1 ))
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${cmd}" 0
		if [ ${FAILED} -eq 0 ]; then
			TOTAL=$(( ${TOTAL} + 1 ))
			set -- ${cmd}
			if expr $arg : ".*dir" >/dev/null 2>&1; then
				compareFiles ${2} ${3}/$(basename ${2})
			else
				compareFiles ${2} ${3}
			fi
		fi
	done

	cp /etc/passwd existing
	cmd="${CP} small existing"
	runTest "${cmd}" 0
	TOTAL=$(( ${TOTAL} + 1 ))
	# existing file must have been overwritten and
	# truncated not just the first few bites
	# overwritten
	compareFiles small existing

	# cp of a fifo is an edge condition, so let's
	# only check the result, not the return code
	timeout 5 ${CP} fifo file2 >/dev/null 2>&1 &
	timeout 1 /bin/sh -c "echo foo >fifo"
	echo foo >file1
	compareFiles file1 file2
}

# purpose : verify program accepts the correct set of arguments
# inputs  : none
# outputs : none if successful, error messages otherwise
checkUsage() {
	local arg noarg onearg threeargs
	local cmd

	verbose "Checking usage..."

	noarg=""
	onearg="one"
	threeargs="one two three"

	for arg	in noarg onearg threeargs; do
		TOTAL=$(( ${TOTAL} + 1 ))
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${cmd}" 1
	done
}

# purpose : verify that two given files are identical
# inputs  : two pathnames
# outputs : nothing on success, an error message if both files are not the
#           same
compareFiles() {
	local file1=$1
	local file2=$2

	verbose "Comparing '${file1}' to '${file2}'..." 3
	cmp -s ${file1} ${file2} || {
		echo "Files '${file1}' and '${file2}' differ." >&2
		NFAIL=$(( ${NFAIL} + 1 ))
	};
}

# purpose : print given message to stderr and exit unsuccessfully
# inputs  : message
error() {
	local msg="$@"

	echo "$0: ${msg}" >&2
	exit 1
}

# purpose  : determine the full absolute path for a given binary
# inputs   : any pathname
# returns  : the absolute path

fullname() {
        local readonly arg="${1}"
        echo $(cd $(dirname ${arg}) && pwd -P)/$(basename ${arg})
}

# purpose : ensure the directory we work in contains only the files we
#           need
prepDir() {
	cd ${DIR} || error "Unable to chdir to ${DIR}!"

	verbose "Prepping test directory..." 3

	verbose "Removing anything already in the directory..." 4
	rm -fr * .??* >/dev/null 2>&1

	verbose "Creating a zero-length file..." 4
	touch zero

	verbose "Creating a large file..." 4
	if [ -f ${TMPDIR:-/tmp}/big ]; then
		ln ${TMPDIR:-/tmp}/big big
	else
		dd if=/dev/zero of=${TMPDIR:-/tmp}/big bs=512k count=4000 >/dev/null 2>&1
		ln ${TMPDIR:-/tmp}/big big
	fi

	verbose "Creating a small file..." 4
	echo "moo" > small

	verbose "Creating a fifo" 4
	mkfifo fifo
}

# purpose : run the given command
# inputs  : a command, 0 or 1 indicating whether or not we expect the
#           command to be successful (0) or not (1)
# outputs : nothing if the return code of the command is as we expect;
#           error message to stderr otherwise
runTest() {
	local cmd="$1"
	local success_or_fail=$2;
	local rval

	FAILED=0

	verbose "Checking '${cmd}'..." 3
	timeout 30 ${cmd} >/dev/null 2>&1
	rval=$?
	if [ $rval -gt 0 ]; then
		if [ $rval = 124 ]; then
			echo "Command timed out, marking as failure:"
			echo "  ${cmd}"
			NFAIL=$(( ${NFAIL} + 1 ))
			FAILED=1
		elif [ ${success_or_fail} -eq 0 ]; then
			echo "Expected success, but command failed:"
			echo "  ${cmd}"
			NFAIL=$(( ${NFAIL} + 1 ))
			FAILED=1
		fi
	else
		if [ ${success_or_fail} -eq 1 ]; then
			echo "Expected failure, but command returned 0:"
			echo "  ${cmd}"
			NFAIL=$(( ${NFAIL} + 1 ))
			FAILED=1
		fi
	fi
}


# purpose  : print usage statement
usage() {
	cat <<EOH
Usage: ${PROGNAME} [-hv] -p program
         -h          print this help and exit
         -p program  test given program
         -v          be verbose
EOH
}


# purpose  : print a given message if verbosity is greater than given
#            level
# inputs   : msg, level
verbose() {
	local readonly msg="${1}"
	local readonly level="${2:-1}"
	local i

	if [ ${VERBOSE} -lt ${level} ]; then
		return
	fi

	i=0
	while [ ${i} -lt ${level} ]; do
		# bash-as-/bin/sh on OS X does not seem to handle "echo
		# -n" correctly, so use the binary
		/bin/echo -n "="
		i=$(( ${i} + 1 ))
	done

	echo "> ${msg}"
}

###
### Main
###

DIR=$(mktemp -d ${TMPDIR:-/tmp}/cptest.XXXXXX)
if [ $? -ne 0 ]; then
	error "Unable to create temporary directory"
	# NOTREACHED
fi

trap "rm -fr ${DIR} 2>/dev/null" 0 1 2 3 4 5 6 7 8 10 11 12 13 14

while getopts 'hp:v' opt; do
	case ${opt} in
		h|\?)
			usage
			exit 0
			# NOTREACHED
			;;
		p)
			CP=$(fullname ${OPTARG})
			;;
		v)
			VERBOSE=$(( ${VERBOSE} + 1 ))
			;;
		*)
			usage
			exit 1
			# NOTREACHED
			;;
	esac
done
shift $(($OPTIND - 1))

if [ $# -gt 0 ]; then
	usage
	exit 1
	# NOTREACHED
fi

if [ -z "${CP}" ]; then
	error "Required flag '-p program' missing."
	# NOTREACHED
fi

checkUsage
prepDir
checkCopying

if [ ${NFAIL} -gt 0 ]; then
	error "${NFAIL}/${TOTAL} tests failed."
	# NOTREACHED
else
	echo "All ${TOTAL} tests passed."
fi

exit 0
