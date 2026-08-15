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
$arch_c2pa="${arch}"
if ("${arch}" -eq "Win32" ) {
    $arch_c2pa="i686"
}
elseif ("${arch}" -eq "x64" ) {
    $arch_c2pa="x86_64"
}

#-----------------------------------------------------------------------
# Build c2pa-rs
# Build under a short path in the Windows temp folder to preserve some path length
# as MAX_PATH is otherwise exceeded when building under a long workspace path.
if (-Not (Test-Path -Path "${release_directory}\..\Source\ThirdParty\c2pa-rs\target\${arch_c2pa}-pc-windows-msvc\release")) {
    $Env:CARGO_TARGET_DIR = Join-Path "${Env:TEMP}" "c2pa-build-cli-${arch_c2pa}"
    Push-Location -Path "${release_directory}\..\Source\ThirdParty\c2pa-rs"
        cargo build --release -p c2pa -p c2pa-c-ffi --target "${arch_c2pa}-pc-windows-msvc"
    Pop-Location

    $c2pa_target_dir = "${release_directory}\..\Source\ThirdParty\c2pa-rs\target\${arch_c2pa}-pc-windows-msvc"
    New-Item -Force -ItemType Directory "${c2pa_target_dir}" | Out-Null
    Copy-Item -Force -Recurse -Path "${Env:CARGO_TARGET_DIR}\${arch_c2pa}-pc-windows-msvc\release" -Destination "${c2pa_target_dir}"

    Remove-Item -Force -Recurse -Path "${Env:CARGO_TARGET_DIR}"
    Remove-Item Env:\CARGO_TARGET_DIR
}

#-----------------------------------------------------------------------
# Build
Push-Location -Path "${release_directory}\..\Project\MSVC2022"
    MSBuild -p:"Configuration=Release;Platform=${arch}"
Pop-Location
