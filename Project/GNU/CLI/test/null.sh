#!/usr/bin/env bash

# TODO: make bwfmetaedit returns error ?

script_path="${PWD}/test"
. ${script_path}/helpers.sh

test="null"
testfile="test.wav"

mkdir "${test}"

ffmpeg -nostdin -f lavfi -i anoisesrc=duration=2 ${test}/${testfile} >/dev/null 2>&1 || fatal "internal" "ffmpeg command failed"

run_bwfmetaedit --Description="description_string" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "${test}/add" "command failed"
fi

run_bwfmetaedit --IART="iart_string" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "${test}/add" "command failed"
fi

if [ "$(xxd -p -l 1 -s 62 ${test}/${testfile})" -ne "0" ] ; then
    error "${test}/add" "not null terminated description string"
fi

if [ "$(xxd -p -l 1 -s 955 ${test}/${testfile})" -ne "0" ] ; then
    error "${test}/add" "not null terminated iart string"
fi

rm -fr "${test}"

exit ${status}
