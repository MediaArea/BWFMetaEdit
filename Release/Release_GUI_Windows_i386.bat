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
del   BWF_MetaEdit_GUI_Windows_i386.exe
del   BWF_MetaEdit_GUI_Windows_i386_WithoutInstaller.7z
rmdir BWF_MetaEdit_GUI_Windows_i386 /S /Q
mkdir BWF_MetaEdit_GUI_Windows_i386


@rem --- Copying : Exe ---
copy  ..\Project\MSVC2015\GUI\Win32\Release\BWF_MetaEdit_GUI.exe BWF_MetaEdit_GUI_Windows_i386\bwfmetaedit-gui.exe

@rem --- Copying : Information files ---
copy ..\License.html BWF_MetaEdit_GUI_Windows_i386\
copy ..\History_GUI.txt BWF_MetaEdit_GUI_Windows_i386\History.txt
copy ..\conformance_point_document.xsd BWF_MetaEdit_GUI_Windows_i386\
copy ReadMe_GUI_Windows.txt BWF_MetaEdit_GUI_Windows_i386\ReadMe.txt


rem --- Compressing Archive ---
cd BWF_MetaEdit_GUI_Windows_i386\
%BPATH%\Windows\7-Zip\7z a -r -tzip -mx9 ..\BWF_MetaEdit_GUI_Windows_i386_WithoutInstaller.zip *
cd ..

rem --- Installer ---
%BPATH%\Windows\NSIS\makensis ..\Source\Install\BWF_MetaEdit_GUI_Windows_i386.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWF_MetaEdit_GUI_Windows_i386\ /S /Q
:SkipCleanUp
