#!/usr/bin/env bash

script_path="${PWD}/test"
. ${script_path}/helpers.sh

# check version
run_bwfmetaedit --version
check_success
if [ "${?}" -ne 0 ] ; then
    error "version/version" "command failed"
fi

if [[ ! "${cmd_stdout}" =~ ^BWF\ MetaEdit\ ([0-9A-Za-z]+\.)+[0-9A-Za-z]+$ ]] ; then
    error "version/version" "invalid version string"
fi

exit ${status}
