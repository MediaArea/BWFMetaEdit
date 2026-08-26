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

macosx_version_min="10.9"

export MAKEOPTS=-j$(($(sysctl -n hw.logicalcpu)+1))

#-----------------------------------------------------------------------
# Cleanup
rm -f "${release_directory}/BWFMetaEdit_CLI_${version}_Mac.dmg"

#-----------------------------------------------------------------------
# Build c2pa-rs
pushd "${release_directory}/../Source/ThirdParty/c2pa-rs"
    for arch in x86_64 aarch64 ; do
        MACOSX_DEPLOYMENT_TARGET="${macosx_version_min}" cargo build --release -p c2pa-c-ffi --no-default-features --features "rust_native_crypto, http, file_io" --target "${arch}-apple-darwin"
    done

    mkdir -p target/release
    lipo -create -output target/release/libc2pa_c.dylib target/x86_64-apple-darwin/release/libc2pa_c.dylib target/aarch64-apple-darwin/release/libc2pa_c.dylib
    install_name_tool -id /usr/local/lib/bwfmetaedit/lib/libc2pa_c.dylib target/release/libc2pa_c.dylib
    install_name_tool -id @rpath/libc2pa_c.dylib target/release/libc2pa_c.dylib
    cp -a target/x86_64-apple-darwin/release/c2pa.h target/release/c2pa.h
popd

#-----------------------------------------------------------------------
# Build bwfmetaedit
pushd "${release_directory}/../Project/GNU/CLI"
    ./autogen.sh
    ./configure --enable-arch-x86_64 --enable-arch-arm64 --enable-c2pa=dynamic --with-macosx-version-min="${macosx_version_min}"
    make

    install_name_tool -add_rpath /usr/local/lib/bwfmetaedit/lib bwfmetaedit
popd

#-----------------------------------------------------------------------
# Package .pkg and .dmg
pushd "${release_directory}/../Project/Mac"
    ./mkdmg.sh BWFMetaEdit CLI "${version}"
popd

#-----------------------------------------------------------------------
# Copy  artifacts
cp -a "${release_directory}/../Project/Mac/BWFMetaEdit_CLI_${version}_Mac.dmg" "${release_directory}/"
