#!/usr/bin/env bash

# TODO: make bwfmetaedit returns error ?

script_path="${PWD}/test"
. ${script_path}/helpers.sh

test="gap"
testfile="test.wav"

mkdir "${test}"

ffmpeg -nostdin -f lavfi -i anoisesrc=duration=2 -rf64 always ${test}/temp.wav >/dev/null 2>&1 || fatal "internal" "ffmpeg command failed"

# add gap after ds64 data
{ head -c 48 ${test}/temp.wav; printf %064s; tail -c +49 ${test}/temp.wav; } > ${test}/${testfile}

# update elements size
echo 5C | xxd -r -u -p -l 1 -s 16 - ${test}/${testfile}
echo AA | xxd -r -u -p -l 1 -s 20 - ${test}/${testfile}

# modify file with bwfmetaedit
run_bwfmetaedit --Description="description_string" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "${test}/add" "command failed"
fi

# test result
if [ "$(xxd -c 19 -u -p -l 19 -s 144 ${test}/${testfile})" != "6465736372697074696F6E5F737472696E6700" ] ; then
    error "${test}/verify" "file was not modified"
fi


if [ "$(xxd -c 64 -u -p -l 64 -s 48 ${test}/${testfile})" != "$(printf %064s | xxd -c 64 -u -p -l 64)" ] ; then
    error "${test}/verify" "gap deleted in rf64 element"
fi

rm -fr "${test}"

exit ${status}
