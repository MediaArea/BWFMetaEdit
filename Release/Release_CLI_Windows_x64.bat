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
del   BWFMetaEdit_CLI_Windows_x64.zip
rmdir BWFMetaEdit_CLI_Windows_x64 /S /Q
mkdir BWFMetaEdit_CLI_Windows_x64

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2017\CLI\x64\Release\bwfmetaedit.exe BWFMetaEdit_CLI_Windows_x64\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\Custom\* BWFMetaEdit_CLI_Windows_x64\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.html BWFMetaEdit_CLI_Windows_x64\
copy ..\History_CLI.txt BWFMetaEdit_CLI_Windows_x64\History.txt
copy ..\conformance_point_document.xsd BWFMetaEdit_CLI_Windows_x64\
copy Readme_CLI_Windows.txt BWFMetaEdit_CLI_Windows_x64\ReadMe.txt

rem --- Compressing Archive ---
cd BWFMetaEdit_CLI_Windows_x64\
%BPATH%\Windows\7-Zip\7z a -r -tzip ..\BWFMetaEdit_CLI_Windows_x64.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWFMetaEdit_CLI_Windows_x64\ /S /Q
:SkipCleanUp
