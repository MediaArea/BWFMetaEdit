##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
##
##  Use of this source code is governed by a BSD-style license that can
##  be found in the License.html file in the root of the source tree.
##

Param([parameter(Mandatory=$true)][String]$arch)

$ErrorActionPreference = "Stop"

#-----------------------------------------------------------------------
# Setup
$release_directory = $PSScriptRoot
$version = (Get-Content "${release_directory}\..\Project\version.txt" -Raw).Trim()

#-----------------------------------------------------------------------
# Build
Push-Location -Path "${release_directory}\..\Project\QtCreator"
    New-Item -Force -ItemType Directory "${arch}"
    Push-Location -Path "${arch}"
        qmake ..
        nmake
    Pop-Location
Pop-Location
