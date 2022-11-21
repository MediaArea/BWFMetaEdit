#!/usr/bin/env bash

script_path="${PWD}/test"
. ${script_path}/helpers.sh

test="xmloutput"
testfile="test.wav"

# xml
xml_simple="<root>\n\
    <item/>\n\
</root>"

xml_with_decl="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n\
<root>\n\
    <item/>\n\
</root>"

xml_with_procinst="<?procinst attribute=\"value\" ?>\n\
<root>\n\
    <item/>\n\
</root>"

xml_incompatible_encoding="<?xml version=\"1.0\" encoding=\"UTF-16\" ?>\n\
<root>\n\
    <item/>\n\
</root>"

xml_invalid="<root/>a"

generate_testfile() {
    ffmpeg -nostdin -f lavfi -i anoisesrc=duration=2 "${test}/${testfile}" >/dev/null 2>&1 || fatal "internal" "ffmpeg command failed"
}

mkdir "${test}" || fatal "internal" "mkdir failed"

# create xml files
echo -e "${xml_simple}" > "${test}/xml-simple.xml" || fatal "internal" "unable to create file"
echo -e "${xml_with_decl}" > "${test}/xml-with-decl.xml" || fatal "internal" "unable to create file"
echo -e "${xml_with_procinst}" > "${test}/xml-with-procinst.xml" || fatal "internal" "unable to create file"
echo -e "${xml_incompatible_encoding}" > "${test}/xml-incompatible-encoding.xml" || fatal "internal" "unable to create file"
echo -e "${xml_invalid}" > "${test}/xml-invalid.xml" || fatal "internal" "unable to create file"

# create wav file
generate_testfile

# populate xml fields
run_bwfmetaedit --in-xmp="${test}/xml-simple.xml" --in-ixml="${test}/xml-with-decl.xml" --in-axml="${test}/xml-with-procinst.xml" --iart="testartist" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/add" "command failed"
fi

# check xmp output to stdout
run_bwfmetaedit --out-xmp "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-xmp" "command failed"
fi

[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(root/item)' -)" -gt 0 ] || error "xmloutput/out-xmp" "output is missing/invalid"

# check xmp output to sidecar file
run_bwfmetaedit --out-xmp-xml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-xmp-xml" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.XMP.xml")" -gt 0 ] || error "xmloutput/out-xmp-xml" "output is missing/invalid"

# check xmp output to file
run_bwfmetaedit --out-xmp="${test}/${testfile}.XMP" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-xmp=" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.XMP")" -gt 0 ] || error "xmloutput/out-xmp=" "output is missing/invalid"

# check ixml output to stdout
run_bwfmetaedit --out-ixml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-ixml" "command failed"
fi

[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(root/item)' -)" -gt 0 ] || error "xmloutput/out-ixml" "output is missing/invalid"

# check ixml output to sidecar file
run_bwfmetaedit --out-ixml-xml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-ixml-xml" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.iXML.xml")" -gt 0 ] || error "xmloutput/out-ixml-xml" "output is missing/invalid"

# check ixml output to file
run_bwfmetaedit --out-ixml="${test}/${testfile}.iXML" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-ixml=" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.iXML")" -gt 0 ] || error "xmloutput/out-ixml=" "output is missing/invalid"

# check axml output to stdout
run_bwfmetaedit --out-axml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-axml" "command failed"
fi

[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(root/item)' -)" -gt 0 ] || error "xmloutput/out-axml" "output is missing/invalid"

# check axml output to sidecar file
run_bwfmetaedit --out-axml-xml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-axml-xml" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.aXML.xml")" -gt 0 ] || error "xmloutput/out-axml-xml" "output is missing/invalid"

# check axml output to file
run_bwfmetaedit --out-axml="${test}/${testfile}.aXML" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-axml=" "command failed"
fi

[ "$(xmllint --xpath 'count(root/item)' "${test}/${testfile}.aXML")" -gt 0 ] || error "xmloutput/out-axml=" "output is missing/invalid"

# check selecting multiple displays
run_bwfmetaedit --out-ixml --out-axml "${test}/${testfile}"
check_failure
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-ixml-and-out-axml" "command succeeded"
fi

# check opening multiples files with one output
run_bwfmetaedit --out-ixml="${test}/${testfile}.iXML" "${test}/${testfile}" "${test}/${testfile}"
check_failure
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-ixml=" "command succeeded with multiples input files"
fi

# check unified xml output to stdout
run_bwfmetaedit --out-xml "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-xml" "command failed"
fi

# Check presence of tech section
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(conformance_point_document/File/Technical/*)' -)" -gt 0 ] || error "xmloutput/out-xml" "missing technical section"

# Check presence of core section
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(conformance_point_document/File/Core/*)' -)" -gt 0 ] || error "xmloutput/out-xml" "missing core section"

# Check presence of xmp section
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(conformance_point_document/File/XMP/*)' -)" -gt 0 ] || error "xmloutput/out-xml" "missing xmp section"

# Check presence of ixml section
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(conformance_point_document/File/iXML/*)' -)" -gt 0 ] || error "xmloutput/out-xml" "missing ixml section"

# Check presence of axml section
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'count(conformance_point_document/File/aXML/*)' -)" -gt 0 ] || error "xmloutput/out-xml" "missing axml section"

run_bwfmetaedit --in-ixml="${test}/xml-invalid.xml" --in-axml="${test}/xml-incompatible-encoding.xml" "${test}/${testfile}"
check_success
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/add" "command failed"
fi

run_bwfmetaedit --out-xml "${test}/${testfile}"
check_failure
if [ "${?}" -ne 0 ] ; then
    error "xmloutput/out-xml" "command failed"
fi

# Check presence of enclosing cdata for invalid xml
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'conformance_point_document/File/iXML/text()' -)" != "<![CDATA["* ] || error "xmloutput/out-xml" "missing cdata for invalid xml"

# Check presence of enclosing cdata for non-utf-8-compatible encoded xml
[ "$(echo "${cmd_stdout}" | xmllint --xpath 'conformance_point_document/File/iXML/text()' -)" != "<![CDATA["* ] || error "xmloutput/out-xml" "missing cdata for non-utf8-compatible encoding"

rm -fr "${test}"

exit ${status}
