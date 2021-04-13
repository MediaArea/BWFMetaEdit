#!/usr/bin/env bash

script_path="${PWD}/test"
. ${script_path}/helpers.sh

test="overwrite"
testfile="test.wav"

mkdir "${test}"

ffmpeg -nostdin -f lavfi -i anoisesrc=duration=2 ${test}/${testfile} >/dev/null 2>&1 || fatal "internal" "ffmpeg command failed"

run_bwfmetaedit --Description="long_description" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "${test}/add" "command failed"
fi

run_bwfmetaedit --reject-overwrite --Description="overwrite" "${test}/${testfile}"
check_failure
if [ "${?}" -ne 0 ] ; then
    error "${test}/overwrite" "command failed"
fi

# read metadata
run_bwfmetaedit --out-core-xml="${test}/test.xml" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "${test}/read" "command failed"
fi

[ "$(xmllint --xpath 'string(///Description)' ${test}/test.xml)" == "long_description" ] || error "metadata/read" "description overwrited despite --reject-overwrite option"

rm -fr "${test}"

exit ${status}
