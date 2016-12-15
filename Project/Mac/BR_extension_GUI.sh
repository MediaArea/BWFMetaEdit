#! /bin/sh

# Because of the autotools bug
cd Project/GNU/GUI
./autogen.sh
cd ../../..

./GUI_Compile.sh
