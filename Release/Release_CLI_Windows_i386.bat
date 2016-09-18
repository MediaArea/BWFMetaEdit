@rem echo off

rem --- Search binaries ---
set BPATH=
if exist "%~dp0\..\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\..\MediaArea-Utils-Binaries"
if exist "%~dp0\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\MediaArea-Utils-Binaries"
if "%BPATH%"=="" (
    echo "ERROR: binaries path not found"
    exit /b 1
)

@rem --- Clean up ---
del   BWF_MetaEdit_CLI_Windows_i386.zip
rmdir BWF_MetaEdit_CLI_Windows_i386 /S /Q
mkdir BWF_MetaEdit_CLI_Windows_i386

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2015\CLI\Win32\Release\bwfmetaedit.exe BWF_MetaEdit_CLI_Windows_i386\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* BWF_MetaEdit_CLI_Windows_i386\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.html BWF_MetaEdit_CLI_Windows_i386\
copy ..\History_CLI.txt BWF_MetaEdit_CLI_Windows_i386\History.txt
copy ..\conformance_point_document.xsd BWF_MetaEdit_CLI_Windows_i386\
copy Readme_CLI_Windows.txt BWF_MetaEdit_CLI_Windows_i386\ReadMe.txt

rem --- Compressing Archive ---
cd BWF_MetaEdit_CLI_Windows_i386\
%BPATH%\Windows\7-Zip\7z a -r -tzip ..\BWF_MetaEdit_CLI_Windows_i386.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWF_MetaEdit_CLI_Windows_i386\ /S /Q
:SkipCleanUp

