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

temp_directory="$(mktemp -d)"
trap 'rm -rf -- "${temp_directory}"' EXIT

#-----------------------------------------------------------------------
# Source archives
pushd "${temp_directory}"
    mkdir bwfmetaedit
    cp -r "${release_directory}/.." bwfmetaedit

    # Remove what is not wanted
    rm -rf bwfmetaedit/.git*
    rm -rf bwfmetaedit/Source/ThirdParty/c2pa/.git*
    rm -fr bwfmetaedit/Source/ThirdParty/c2pa-rs/sdk/tests # Remove huge tests directory from c2pa-rs

    # Make source archives
    GZIP=-9 tar -cz --owner=root --group=root -f "bwfmetaedit_${version}.tar.gz" bwfmetaedit
    BZIP=-9 tar -cj --owner=root --group=root -f "bwfmetaedit_${version}.tar.bz2" bwfmetaedit
    XZ_OPT=-9e tar -cJ --owner=root --group=root -f "bwfmetaedit_${version}.tar.xz" bwfmetaedit
    7za a -t7z -mx=9 -bd "bwfmetaedit_${version}.7z" bwfmetaedit
popd

#-----------------------------------------------------------------------
# Unix CLI archives
pushd "${temp_directory}"
    mkdir BWFMetaEdit_CLI_GNU_FromSource
    cp -r "${release_directory}/.." BWFMetaEdit_CLI_GNU_FromSource

    # Script preparation
    mv BWFMetaEdit_CLI_GNU_FromSource/Project/GNU/CLI/AddThisToRoot_CLI_compile.sh BWFMetaEdit_CLI_GNU_FromSource/CLI_Compile.sh
    chmod +x BWFMetaEdit_CLI_GNU_FromSource/CLI_Compile.sh
    chmod +x BWFMetaEdit_CLI_GNU_FromSource/Project/GNU/CLI/autogen.sh
    chmod +x BWFMetaEdit_CLI_GNU_FromSource/Project/Mac/BR_extension_CLI.sh
    chmod +x BWFMetaEdit_CLI_GNU_FromSource/Project/Mac/mkdmg.sh

    # Autotools
    pushd BWFMetaEdit_CLI_GNU_FromSource/Project/GNU/CLI
        ./autogen.sh
    popd

    # Remove what is not wanted
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/.git*
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Source/ThirdParty/c2pa/.git*
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Source/ThirdParty/c2pa-rs/sdk/tests # Remove huge tests directory from c2pa-rs
    rm -f BWFMetaEdit_CLI_GNU_FromSource/History_GUI.txt
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/debian
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Project/MSVC2022
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Project/OBS
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Project/GNU/GUI
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Project/QtCreator
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Project/Mac/*_GUI.sh
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Source/GUI
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Source/Resource
    rm -rf BWFMetaEdit_CLI_GNU_FromSource/Source/Install

    # Make source archives
    GZIP=-9 tar -cz --owner=root --group=root -f "BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.gz" BWFMetaEdit_CLI_GNU_FromSource
    BZIP=-9 tar -cj --owner=root --group=root -f "BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.bz2" BWFMetaEdit_CLI_GNU_FromSource
    XZ_OPT=-9e tar -cJ --owner=root --group=root -f "BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.xz" BWFMetaEdit_CLI_GNU_FromSource
popd

#-----------------------------------------------------------------------
# Unix GUI archives
pushd "${temp_directory}"
    mkdir BWFMetaEdit_GUI_GNU_FromSource
    cp -r "${release_directory}/.." BWFMetaEdit_GUI_GNU_FromSource

    # Script preparation
    mv BWFMetaEdit_GUI_GNU_FromSource/Project/QtCreator/AddThisToRoot_GUI_compile.sh BWFMetaEdit_GUI_GNU_FromSource/GUI_Compile.sh
    chmod +x BWFMetaEdit_GUI_GNU_FromSource/GUI_Compile.sh
    chmod +x BWFMetaEdit_GUI_GNU_FromSource/Project/Mac/BR_extension_GUI.sh
    chmod +x BWFMetaEdit_GUI_GNU_FromSource/Project/Mac/mkdmg.sh

    # Remove what is not wanted
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/.git*
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Source/ThirdParty/c2pa/.git*
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Source/ThirdParty/c2pa-rs/sdk/tests # Remove huge tests directory from c2pa-rs
    rm -f BWFMetaEdit_GUI_GNU_FromSource/History_CLI.txt
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/debian
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Project/MSVC2022
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Project/OBS
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Project/GNU/CLI
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Project/Mac/*_CLI.sh
    rm -rf BWFMetaEdit_GUI_GNU_FromSource/Source/CLI

    # Make source archives
    GZIP=-9 tar -cz --owner=root --group=root -f "BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.gz" BWFMetaEdit_GUI_GNU_FromSource
    BZIP=-9 tar -cj --owner=root --group=root -f "BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.bz2" BWFMetaEdit_GUI_GNU_FromSource
    XZ_OPT=-9e tar -cJ --owner=root --group=root -f "BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.xz" BWFMetaEdit_GUI_GNU_FromSource
popd

#-----------------------------------------------------------------------
# Create vendor archive for OBS
pushd "${temp_directory}"
    pushd bwfmetaedit/Source/ThirdParty/c2pa-rs
        cargo vendor --locked "${temp_directory}/vendor"
    popd
    XZ_OPT=-9e tar -cJ --owner=root --group=root -f "bwfmetaedit-vendor_${version}.tar.xz" vendor
popd

#-----------------------------------------------------------------------
# Copy the source packages to the source directory
mkdir "${release_directory}/source"
cp "${temp_directory}/bwfmetaedit_${version}.tar.gz" "${release_directory}/source"
cp "${temp_directory}/bwfmetaedit_${version}.tar.bz2" "${release_directory}/source"
cp "${temp_directory}/bwfmetaedit_${version}.tar.xz" "${release_directory}/source"
cp "${temp_directory}/bwfmetaedit_${version}.7z" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.gz" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.bz2" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_CLI_${version}_GNU_FromSource.tar.xz" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.gz" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.bz2" "${release_directory}/source"
cp "${temp_directory}/BWFMetaEdit_GUI_${version}_GNU_FromSource.tar.xz" "${release_directory}/source"
cp "${temp_directory}/bwfmetaedit-vendor_${version}.tar.xz" "${release_directory}/source"