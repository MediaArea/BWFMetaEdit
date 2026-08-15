##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

#!/bin/bash

set -e # fail on any error

#-----------------------------------------------------------------------
# Setup
release_directory="$(realpath -- "$(dirname -- "${BASH_SOURCE[0]}")")"
version="$(<"${release_directory}/../Project/version.txt")"

macosx_version_min="11.0"

export MAKEOPTS=-j$(($(sysctl -n hw.logicalcpu)+1))

#-----------------------------------------------------------------------
# Cleanup
rm -f "${release_directory}/BWFMetaEdit_GUI_${version}_Mac.dmg"
rm -f "${release_directory}/../Project/Mac/BWFMetaEdit_GUI_${version}_Mac.dmg"

#-----------------------------------------------------------------------
# Build c2pa-rs
pushd "${release_directory}/../Source/ThirdParty/c2pa-rs"
    for arch in x86_64 aarch64 ; do
        MACOSX_DEPLOYMENT_TARGET="${macosx_version_min}" cargo build --release -p c2pa -p c2pa-c-ffi --target "${arch}-apple-darwin"
    done

    mkdir -p target/release
    lipo -create -output target/release/libc2pa_c.dylib target/x86_64-apple-darwin/release/libc2pa_c.dylib target/aarch64-apple-darwin/release/libc2pa_c.dylib
    install_name_tool -id "@rpath/libc2pa_c.dylib" "target/release/libc2pa_c.dylib"
    cp -a "target/x86_64-apple-darwin/release/c2pa.h" "target/release/c2pa.h"
popd

#-----------------------------------------------------------------------
# Build BWF MetaEdit
pushd "${release_directory}/../Project/QtCreator"
    qmake ENABLE_C2PA=yes
    make
    if ! otool -l "BWF MetaEdit.app/Contents/MacOS/BWF MetaEdit" | grep -q "@executable_path/../Frameworks" ; then
        install_name_tool -add_rpath "@executable_path/../Frameworks" "BWF MetaEdit.app/Contents/MacOS/BWF MetaEdit"
    fi
popd

#-----------------------------------------------------------------------
# Package .dmg
pushd "${release_directory}/../Project/Mac"
    ./mkdmg.sh "BWF MetaEdit" GUI "${version}"
popd

#-----------------------------------------------------------------------
# Copy artifacts
cp -a "${release_directory}/../Project/Mac/BWFMetaEdit_GUI_${version}_Mac.dmg" "${release_directory}/"
