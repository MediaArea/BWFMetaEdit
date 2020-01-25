#!/usr/bin/env bash

script_path="${PWD}/test"
. ${script_path}/helpers.sh

test="metadata"
testfile="test.wav"

# bext metadata
bext_version="1"
description="test_bext_description"
originator="test_bext_originator"
originator_reference="test_bext_originator_ref"
origination_date="1970-01-01"
origination_time="11:59:59"
time_reference="0"
umid="0000000011111111222222223333333344444444555555556666666677777777"
coding_history="A=ANALOGUE,F=48000,B=16,W=16,M=mono"

# list-info metadata
iarl="test_list_iarl"
iart="test_list_iart"
icms="test_list_icms"
icmt="test_list_icmt"
icop="test_list_icop"
icrd="1970-01-01"
ieng="test_list_ieng"
ignr="test_list_ignr"
ikey="test_list_ikey"
imed="test_list_imed"
inam="test_list_inam"
iprd="test_list_iprd"
isbj="test_list_isbj"
isft="test_list_isft"
isrc="test_list_isrc"
isrf="test_list_isrf"
itch="test_list_itch"

generate_testfile() {
    ffmpeg -nostdin -f lavfi -i anoisesrc=duration=2 "${test}/${testfile}" >/dev/null 2>&1 || fatal "internal" "ffmpeg command failed"
}

verify_metadata() {
    [ "$(xmllint --xpath 'string(///BextVersion)' ${test}/test.xml)" == "${bext_version}" ] || error "metadata/read" "BextVersion mismatch"
    [ "$(xmllint --xpath 'string(///Description)' ${test}/test.xml)" == "${description}" ] || error "metadata/read" "Description mismatch"
    [ "$(xmllint --xpath 'string(///Originator)' ${test}/test.xml)" == "${originator}" ] || error "metadata/read" "Originator mismatch"
    [ "$(xmllint --xpath 'string(///OriginationDate)' ${test}/test.xml)" == "${origination_date}" ] || error "metadata/read" "OriginationDate mismatch"
    [ "$(xmllint --xpath 'string(///OriginationTime)' ${test}/test.xml)" == "${origination_time}" ] || error "metadata/read" "OriginationTime mismatch"
    [ "$(xmllint --xpath 'string(///TimeReference)' ${test}/test.xml)" == "${time_reference}" ] || error "metadata/read" "TimeReference mismatch"
    [ "$(xmllint --xpath 'string(///CodingHistory)' ${test}/test.xml)" == "${coding_history}" ] || error "metadata/read" "CodingHistory mismatch"
    [ "$(xmllint --xpath 'string(///UMID)' ${test}/test.xml)" == "${umid}" ] || error "metadata/read" "UMID mismatch"
    [ "$(xmllint --xpath 'string(///IARL)' ${test}/test.xml)" == "${iarl}" ] || error "metadata/read" "IARL mismatch"
    [ "$(xmllint --xpath 'string(///IART)' ${test}/test.xml)" == "${iart}" ] || error "metadata/read" "IART mismatch"
    [ "$(xmllint --xpath 'string(///ICMS)' ${test}/test.xml)" == "${icms}" ] || error "metadata/read" "ICMS mismatch"
    [ "$(xmllint --xpath 'string(///ICMT)' ${test}/test.xml)" == "${icmt}" ] || error "metadata/read" "ICMT mismatch"
    [ "$(xmllint --xpath 'string(///ICOP)' ${test}/test.xml)" == "${icop}" ] || error "metadata/read" "ICOP mismatch"
    [ "$(xmllint --xpath 'string(///ICRD)' ${test}/test.xml)" == "${icrd}" ] || error "metadata/read" "ICRD mismatch"
    [ "$(xmllint --xpath 'string(///IENG)' ${test}/test.xml)" == "${ieng}" ] || error "metadata/read" "IENG mismatch"
    [ "$(xmllint --xpath 'string(///IGNR)' ${test}/test.xml)" == "${ignr}" ] || error "metadata/read" "IGNR mismatch"
    [ "$(xmllint --xpath 'string(///IKEY)' ${test}/test.xml)" == "${ikey}" ] || error "metadata/read" "IKEY mismatch"
    [ "$(xmllint --xpath 'string(///IMED)' ${test}/test.xml)" == "${imed}" ] || error "metadata/read" "IMED mismatch"
    [ "$(xmllint --xpath 'string(///INAM)' ${test}/test.xml)" == "${inam}" ] || error "metadata/read" "INAM mismatch"
    [ "$(xmllint --xpath 'string(///IPRD)' ${test}/test.xml)" == "${iprd}" ] || error "metadata/read" "IPRD mismatch"
    [ "$(xmllint --xpath 'string(///ISBJ)' ${test}/test.xml)" == "${isbj}" ] || error "metadata/read" "ISBJ mismatch"
    [ "$(xmllint --xpath 'string(///ISFT)' ${test}/test.xml)" == "${isft}" ] || error "metadata/read" "ISFT mismatch"
    [ "$(xmllint --xpath 'string(///ISRC)' ${test}/test.xml)" == "${isrc}" ] || error "metadata/read" "ISRC mismatch"
    [ "$(xmllint --xpath 'string(///ISRF)' ${test}/test.xml)" == "${isrf}" ] || error "metadata/read" "ISRF mismatch"
    [ "$(xmllint --xpath 'string(///ITCH)' ${test}/test.xml)" == "${itch}" ] || error "metadata/read" "ITCH mismatch"
}

mkdir "${test}"

# test metadata from command line
generate_testfile

run_bwfmetaedit --BextVersion="${bext_version}" --Description="${description}" --Originator="${originator}" \
                --OriginatorReference="${originator_reference}" --OriginationDate="${origination_date}" --OriginationTime="${origination_time}" \
                --TimeReference="${time_reference}" --UMID="${umid}"  --History="${coding_history}" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "metadata/add" "command failed"
fi

run_bwfmetaedit --IARL="${iarl}" --IART="${iart}" --ICMS="${icms}" --ICMT="${icmt}" --ICOP="${icop}" --ICRD="${icrd}" \
                --IENG="${ieng}" --IGNR="${ignr}" --IKEY="${ikey}" --IMED="${imed}" --INAM="${inam}" --IPRD="${iprd}" \
                --ISBJ="${isbj}" --ISFT="${isft}" --ISRC="${isrc}" --ISRF="${isrf}" --ITCH="${itch}" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "metadata/add" "command failed"
fi

run_bwfmetaedit --out-core-xml="${test}/test.xml" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "metadata/read" "command failed"
fi

verify_metadata

rm "${test}/${testfile}" || fatal "internal" "rm command failed"

# test metadata from xml
generate_testfile

run_bwfmetaedit --in-core="${test}/test.xml" "${test}/${testfile}"
if [ "${?}" -ne 0 ] ; then
    error "metadata/add" "command failed"
fi

run_bwfmetaedit --out-core-xml="${test}/test.xml" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "metadata/read" "command failed"
fi

verify_metadata

run_bwfmetaedit --in-core-remove "${test}/${testfile}"

run_bwfmetaedit --out-core-xml="${test}/test.xml" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "metadata/read" "command failed"
fi

[ "$(xmllint --xpath 'count(///Core/*)' ${test}/test.xml)" == "0" ] || error "metadata/read" "Core is not empty after in-core-remove"

rm -fr "${test}"

exit ${status}
