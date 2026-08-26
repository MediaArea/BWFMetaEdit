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

#-----------------------------------------------------------------------
# Create obs repository
mkdir "${release_directory}/obs"

cp "${release_directory}/../Project/OBS/bwfmetaedit.spec" "${release_directory}/obs/"
cp "${release_directory}/../Project/OBS/bwfmetaedit.dsc" "${release_directory}/obs/"
cp "${release_directory}/../Project/OBS/PKGBUILD" "${release_directory}/obs/"

cp "${release_directory}/../debian/bwfmetaedit.dirs" "${release_directory}/obs/debian.bwfmetaedit.dirs"
cp "${release_directory}/../debian/bwfmetaedit.install" "${release_directory}/obs/debian.bwfmetaedit.install"
cp "${release_directory}/../debian/bwfmetaedit-plugin-c2pa.dirs" "${release_directory}/obs/debian.bwfmetaedit-plugin-c2pa.dirs"
cp "${release_directory}/../debian/bwfmetaedit-plugin-c2pa.install" "${release_directory}/obs/debian.bwfmetaedit-plugin-c2pa.install"
cp "${release_directory}/../debian/bwfmetaedit-gui.dirs" "${release_directory}/obs/debian.bwfmetaedit-gui.dirs"
cp "${release_directory}/../debian/bwfmetaedit-gui.install" "${release_directory}/obs/debian.bwfmetaedit-gui.install"
cp "${release_directory}/../debian/changelog" "${release_directory}/obs/debian.changelog"
cp "${release_directory}/../debian/compat" "${release_directory}/obs/debian.compat"
cp "${release_directory}/../debian/control" "${release_directory}/obs/debian.control"
cp "${release_directory}/../debian/copyright" "${release_directory}/obs/debian.copyright"
cp "${release_directory}/../debian/rules" "${release_directory}/obs/debian.rules"

cp "${release_directory}/source/bwfmetaedit_${version}.tar.xz" "${release_directory}/obs/"
cp "${release_directory}/source/bwfmetaedit-vendor_${version}.tar.xz" "${release_directory}/obs/"