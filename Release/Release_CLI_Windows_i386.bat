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
del   BWFMetaEdit_CLI_Windows_i386.zip
rmdir BWFMetaEdit_CLI_Windows_i386 /S /Q
mkdir BWFMetaEdit_CLI_Windows_i386

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2017\CLI\Win32\Release\bwfmetaedit.exe BWFMetaEdit_CLI_Windows_i386\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* BWFMetaEdit_CLI_Windows_i386\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.html BWFMetaEdit_CLI_Windows_i386\
copy ..\History_CLI.txt BWFMetaEdit_CLI_Windows_i386\History.txt
copy ..\conformance_point_document.xsd BWFMetaEdit_CLI_Windows_i386\
copy Readme_CLI_Windows.txt BWFMetaEdit_CLI_Windows_i386\ReadMe.txt

rem --- Compressing Archive ---
cd BWFMetaEdit_CLI_Windows_i386\
%BPATH%\Windows\7-Zip\7z a -r -tzip ..\BWFMetaEdit_CLI_Windows_i386.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWFMetaEdit_CLI_Windows_i386\ /S /Q
:SkipCleanUp

