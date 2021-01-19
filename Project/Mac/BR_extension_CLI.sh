#! /bin/sh

# Because of the autotools bug
cd Project/GNU/CLI
./autogen.sh
cd ../../..

./CLI_Compile.sh --enable-arch-x86_64 --enable-arch-arm64 --with-macosx-version-min=10.9
