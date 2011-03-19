@rem echo off

@rem --- Clean up ---
del   BWF_MetaEdit_CLI_Windows_x64.zip
rmdir BWF_MetaEdit_CLI_Windows_x64 /S /Q
mkdir BWF_MetaEdit_CLI_Windows_x64

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2010\CLI\x64\Release\bwfmetaedit.exe BWF_MetaEdit_CLI_Windows_x64\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\Custom\* BWF_MetaEdit_CLI_Windows_x64\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.html BWF_MetaEdit_CLI_Windows_x64\
copy ..\History_CLI.txt BWF_MetaEdit_CLI_Windows_x64\History.txt
copy ..\conformance_point_document.xsd BWF_MetaEdit_CLI_Windows_x64\
copy Readme_CLI_Windows.txt BWF_MetaEdit_CLI_Windows_x64\ReadMe.txt

rem --- Compressing Archive ---
cd BWF_MetaEdit_CLI_Windows_x64\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -tzip ..\BWF_MetaEdit_CLI_Windows_x64.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWF_MetaEdit_CLI_Windows_x64\ /S /Q
:SkipCleanUp
