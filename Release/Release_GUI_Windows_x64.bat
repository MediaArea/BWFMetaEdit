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
del   BWFMetaEdit_GUI_Windows_x64.exe
del   BWFMetaEdit_GUI_Windows_x64_WithoutInstaller.7z
rmdir BWFMetaEdit_GUI_Windows_x64 /S /Q
mkdir BWFMetaEdit_GUI_Windows_x64


@rem --- Copying : Exe ---
copy  "..\Project\QtCreator\x64\BWF MetaEdit.exe" BWFMetaEdit_GUI_Windows_x64\bwfmetaedit-gui.exe

@rem --- Copying : Information files ---
copy ..\License.html BWFMetaEdit_GUI_Windows_x64\
copy ..\History_GUI.txt BWFMetaEdit_GUI_Windows_x64\History.txt
copy ..\conformance_point_document.xsd BWFMetaEdit_GUI_Windows_x64\
copy ReadMe_GUI_Windows.txt BWFMetaEdit_GUI_Windows_x64\ReadMe.txt


rem --- Compressing Archive ---
cd BWFMetaEdit_GUI_Windows_x64\
%BPATH%\Windows\7-Zip\7z a -r -tzip -mx9 ..\BWFMetaEdit_GUI_Windows_x64_WithoutInstaller.zip *
cd ..

rem --- Installer ---
pushd %BPATH%\Windows\NSIS
makensis.exe "%~dp0\..\Source\Install\BWF_MetaEdit_GUI_Windows_x64.nsi"
popd

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWFMetaEdit_GUI_Windows_x64\ /S /Q
:SkipCleanUp
