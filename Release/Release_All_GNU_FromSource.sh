#! /bin/sh

#-----------------------------------------------------------------------
# Clean up
test -e BWF_MetaEdit_GNU_FromSource.tar     && rm    BWF_MetaEdit_GNU_FromSource.tar
test -e BWF_MetaEdit_GNU_FromSource.tar.bz2 && rm    BWF_MetaEdit_GNU_FromSource.tar.bz2
test -d BWF_MetaEdit_GNU_FromSource         && rm -r BWF_MetaEdit_GNU_FromSource
mkdir BWF_MetaEdit_GNU_FromSource


#-----------------------------------------------------------------------
# Preparing : Project
cd ../Project/GNU/CLI
chmod u+x autogen
./autogen
cd ../../../Release
cd ../Project/GNU/GUI
chmod u+x autogen
./autogen
cd ../../../Release


#-----------------------------------------------------------------------
# Copying : Project
mkdir -p BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI
cp ../Project/GNU/CLI/aclocal.m4 BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/autogen BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/config.guess BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/config.sub BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/configure BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/configure.ac BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/depcomp BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/install-sh BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/Makefile.am BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/Makefile.in BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/missing BWF_MetaEdit_GNU_FromSource/Project/GNU/CLI/
mkdir -p BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI
cp ../Project/GNU/GUI/aclocal.m4 BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/autogen BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/config.guess BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/config.sub BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/configure BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/configure.ac BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/depcomp BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/install-sh BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/Makefile.am BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/Makefile.in BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/
cp ../Project/GNU/GUI/missing BWF_MetaEdit_GNU_FromSource/Project/GNU/GUI/

#-----------------------------------------------------------------------
# Copying : Source
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/CLI
cp -r ../Source/CLI/*.h BWF_MetaEdit_GNU_FromSource/Source/CLI/
cp -r ../Source/CLI/*.cpp BWF_MetaEdit_GNU_FromSource/Source/CLI/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Common
cp -r ../Source/Common/*.h BWF_MetaEdit_GNU_FromSource/Source/Common/
cp -r ../Source/Common/*.cpp BWF_MetaEdit_GNU_FromSource/Source/Common/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/GUI/Qt
cp -r ../Source/GUI/Qt/*.h BWF_MetaEdit_GNU_FromSource/Source/GUI/Qt/
cp -r ../Source/GUI/Qt/*.cpp BWF_MetaEdit_GNU_FromSource/Source/GUI/Qt/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/MD5
cp -r ../Source/MD5/*.h BWF_MetaEdit_GNU_FromSource/Source/MD5/
cp -r ../Source/MD5/*.c BWF_MetaEdit_GNU_FromSource/Source/MD5/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Resource
cp -r ../Source/Resource/*.qrc BWF_MetaEdit_GNU_FromSource/Source/Resource/
cp -r ../Source/Resource/*.html BWF_MetaEdit_GNU_FromSource/Source/Resource/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Resource/Image/Menu
cp -r ../Source/Resource/Image/Menu/*.png BWF_MetaEdit_GNU_FromSource/Source/Resource/Image/Menu/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Resource/Image/Logo
cp -r ../Source/Resource/Image/Logo/*.png BWF_MetaEdit_GNU_FromSource/Source/Resource/Image/Logo/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Resource/Documentation
cp -r ../Source/Resource/Documentation/*.html BWF_MetaEdit_GNU_FromSource/Source/Resource/Documentation/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/Riff
cp -r ../Source/Riff/*.h BWF_MetaEdit_GNU_FromSource/Source/Riff/
cp -r ../Source/Riff/*.cpp BWF_MetaEdit_GNU_FromSource/Source/Riff/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/TinyXml
cp -r ../Source/TinyXml/*.h BWF_MetaEdit_GNU_FromSource/Source/TinyXml/
cp -r ../Source/TinyXml/*.cpp BWF_MetaEdit_GNU_FromSource/Source/TinyXml/
mkdir -p BWF_MetaEdit_GNU_FromSource/Source/ZenLib
cp -r ../Source/ZenLib/*.h BWF_MetaEdit_GNU_FromSource/Source/ZenLib/
cp -r ../Source/ZenLib/*.cpp BWF_MetaEdit_GNU_FromSource/Source/ZenLib/

#-----------------------------------------------------------------------
# Copying : Release
mkdir -p BWF_MetaEdit_GNU_FromSource/Release
cp *.sub BWF_MetaEdit_GNU_FromSource/Release/
cp *.sh BWF_MetaEdit_GNU_FromSource/Release/
chmod u+x BWF_MetaEdit_GNU_FromSource/Release/*.sh

#-----------------------------------------------------------------------
# Copying : Information
cp ../License.html BWF_MetaEdit_GNU_FromSource/
cp ../History*.txt BWF_MetaEdit_GNU_FromSource/
cp ../conformance_point_document.xsd BWF_MetaEdit_GNU_FromSource/
dos2unix BWF_MetaEdit_GNU_FromSource/History*.txt
cp ReadMe_CLI_Linux.txt BWF_MetaEdit_GNU_FromSource/Release/
cp ReadMe_CLI_Mac.txt BWF_MetaEdit_GNU_FromSource/Release/
cp ReadMe_GUI_Linux.txt BWF_MetaEdit_GNU_FromSource/Release/
cp ReadMe_GUI_Mac.txt BWF_MetaEdit_GNU_FromSource/Release/


#-----------------------------------------------------------------------
# Preparing Archive : BWF_MetaEdit
mv BWF_MetaEdit_GNU_FromSource BWF_MetaEdit

#-----------------------------------------------------------------------
# Preparing Archive : Automation
cp ../Project/GNU/CLI/AddThisToRoot_CLI_compile.sh BWF_MetaEdit/CLI_Compile.sh
chmod u+x BWF_MetaEdit/CLI_Compile.sh
cp ../Project/GNU/GUI/AddThisToRoot_GUI_compile.sh BWF_MetaEdit/GUI_Compile.sh
chmod u+x BWF_MetaEdit/GUI_Compile.sh


#-----------------------------------------------------------------------
# Compressing Archive
tar jchf BWF_MetaEdit_GNU_FromSource.tar.bz2 BWF_MetaEdit/*

#-----------------------------------------------------------------------
# Clean up
rm -r BWF_MetaEdit
