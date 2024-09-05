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
FAILED_TESTS=""
NFAIL=0
PROGNAME=${0##/}
TDIR="${TMPDIR:-/var/tmp}"
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
	checkCopyFailures
	checkCopySuccesses
}

# purpose : verify that the program fails when copying due to illegal args
# inputs  : none
# outputs : none if successful, error messages otherwise
checkCopyFailures() {
	local arg cmd
	local dir_to_file dir_to_dir file_to_subdir same socket_to_file

	dir_to_file=". file"
	dir_to_dir="/ ."
	file_to_subdir="${TEST_FILE} ./sub/dir"
	socket_to_file="/var/run/log log"

	cd ${DIR}
	for arg in dir_to_file dir_to_dir file_to_subdir socket_to_file; do
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${arg}" "${cmd}" 1
	done

	cp ${TEST_FILE} file

	runTest "too_many_arguments" "${CP} file file1 file2 file3 file4" 1

	ln file file2
	cmd="${CP} file file2"
	runTest "identical_source_and_dest" "${cmd}" 1
	if [ $? -eq 1 ]; then
		# must not truncate/modify existing file
		compareFiles "identical_source_and_dest" ${TEST_FILE} file
	fi

	# repeat to check path construction
	cp ${TEST_FILE} file
	cmd="${CP} file ."
	runTest "identical_source_and_test_path" "${cmd}" 1
	if [ $? -eq 1 ]; then
		# must not truncate/modify existing file
		compareFiles "identical_source_and_test_path" ${TEST_FILE} file
	fi

	cmd="${CP} s1 s2"
	runTest "symlink_loop" "${cmd}" 1
}

# purpose : verify that the program succeeds when it should
# inputs  : none
# outputs : none if successful, error messages otherwise
checkCopySuccesses() {
	local arg cmd
	local ro_file_to_file file_to_existing abs_file_to_dir rel_file_to_dir abs_file_to_subdir file_to_slash_dir
	local zero big

	ro_file_to_file="${TEST_FILE} file"
	rw_file_to_file="${TDIR}/f file"
	file_to_existing="${TEST_FILE} ${TDIR}/f"
	abs_file_to_dir="${TEST_FILE} ."
	rel_file_to_dir="g ./sub/dir/."
	abs_file_to_subdir="${TEST_FILE} ./sub/dir/."
	file_to_slash_dir="${TEST_FILE} ../$(basename $(pwd))/"
	zero="zero file"
	big="big file"

	mkdir -p ./sub/dir
	cp ${TEST_FILE} ${TDIR}/f
	cp ${TEST_FILE} ./g

	cd ${DIR}
	for arg in ro_file_to_file rw_file_to_file abs_file_to_dir \
		rel_file_to_dir abs_file_to_subdir file_to_existing \
		file_to_slash_dir zero big; do
		rm -f file >/dev/null 2>&1
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${arg}" "${cmd}" 0
		if [ ${FAILED} -eq 0 ]; then
			set -- ${cmd}
			if expr $arg : ".*dir" >/dev/null 2>&1; then
				compareFiles "${arg}" ${2} ${3}/$(basename ${2})
			else
				compareFiles "${arg}" ${2} ${3}
			fi
		fi
	done

	cp /etc/passwd existing
	cmd="${CP} small existing"
	runTest "small_over_existing" "${cmd}" 0
	# existing file must have been overwritten and
	# truncated not just the first few bites
	# overwritten
	if [ $? -eq 0 ]; then
		compareFiles "small_over_existing" small existing
	fi

	# cp of a fifo is an edge condition, so let's
	# only check the result, not the return code
	TOTAL=$(( ${TOTAL} + 1 ))
	verbose "Test #${TOTAL} (copy_fifo): Checking '${CP} fifo file2'..." 1
	timeout 5 ${CP} fifo file2 >/dev/null 2>&1 &
	timeout 1 /bin/sh -c "echo foo >fifo"
	echo foo >file1
	compareFiles "copy_fifo" file1 file2
}

# purpose : verify program accepts the correct set of arguments
# inputs  : none
# outputs : none if successful, error messages otherwise
checkUsage() {
	local arg noarg onearg threeargs
	local cmd

	noarg=""
	onearg="one"
	threeargs="one two three"

	for arg	in noarg onearg threeargs; do
		cmd=$(eval echo ${CP} \$${arg})
		runTest "${arg}" "${cmd}" 1
	done
}

# purpose : verify that two given files are identical
# inputs  : two pathnames, name of the test
# outputs : nothing on success, an error message if both files are not the
#           same
compareFiles() {
	local name=$1
	local file1=$2
	local file2=$3

	verbose "Test #${TOTAL} (${name}): Comparing '${file1}' to '${file2}'..." 1
	cmp -s ${file1} ${file2}
	if [ $? -gt 0 ]; then
		echo "Files '${file1}' and '${file2}' differ." >&2
		NFAIL=$(( ${NFAIL} + 1 ))
		FAILED_TESTS="${FAILED_TESTS} ${name}"
	else
		verbose "Succeeded." 2
	fi
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

	verbose "Prepping test directory..." 1

	verbose "Removing anything already in the directory..." 2
	rm -fr * .??* >/dev/null 2>&1

	verbose "Creating a zero-length file..." 2
	touch zero

	verbose "Creating a large file..." 2
	if [ -f ${TDIR}/big ]; then
		ln ${TDIR}/big big
	else
		dd if=/dev/zero of=${TDIR}/big bs=512k count=4000 >/dev/null 2>&1
		ln ${TDIR}/big big
	fi

	verbose "Creating a small file..." 2
	echo "moo" > small

	verbose "Creating a fifo..." 2
	mkfifo fifo

	verbose "Creating a symlink loop..." 2
	ln -s s1 s2
	ln -s s2 s1
}

# purpose : report failure of a test
# inputs  : error message, cmd, name of the test
# outputs : error message, increments fail count
reportFailure() {
	local msg="$1"
	local cmd="$2"
	local name="$3"
	echo "${msg}:"
	echo "  ${cmd}"
	NFAIL=$(( ${NFAIL} + 1 ))
	FAILED=1
	FAILED_TESTS="${FAILED_TESTS} ${name}"
}

# purpose : run the given command
# inputs  : name of the test, a command, 0 or 1 indicating whether
#           or not we expect the command to be successful (0) or not (1)
# outputs : nothing if the return code of the command is as we expect;
#           error message to stderr otherwise
runTest() {
	local name="$1"
	local cmd="$2"
	local success_or_fail=$3;
	local rval

	FAILED=0
	TOTAL=$(( ${TOTAL} + 1 ))

	verbose "Test #${TOTAL} (${name}): Running '${cmd}'..." 1
	timeout 30 ${cmd} >/dev/null 2>&1
	rval=$?
	if [ $rval -gt 0 ]; then
		if [ $rval = 124 ]; then
			reportFailure "Command timed out, marking as failure" "${cmd}" "${name}"
		elif [ ${success_or_fail} -eq 0 ]; then
			reportFailure "Expected success, but command failed" "${cmd}" "${name}"
		else
			verbose "Failed as expected." 2
		fi
	else
		if [ ${success_or_fail} -eq 1 ]; then
			reportFailure "Expected failure, but command returned 0" "${cmd}" "${name}"
		fi
	fi

	return ${rval}
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

if [ x"$(whoami)" = x"root" ]; then
	error "Tests should be run as a normal user."
	# NOTREACHED
fi

DIR=$(mktemp -d ${TDIR}/cptest.XXXXXX)
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
	echo >&2
	echo "${NFAIL}/${TOTAL} tests failed." >&2
	echo "Failed tests: ${FAILED_TESTS}" >&2
	exit 1
	# NOTREACHED
else
	echo "All ${TOTAL} tests passed."
fi

exit 0
