@rem echo off

@rem --- Clean up ---
del   BWF_MetaEdit_GUI_Windows_i386.exe
del   BWF_MetaEdit_GUI_Windows_i386_WithoutInstaller.7z
rmdir BWF_MetaEdit_GUI_Windows_i386 /S /Q
mkdir BWF_MetaEdit_GUI_Windows_i386


@rem --- Copying : Exe ---
copy  ..\Project\MSVC2010\GUI\Win32\Release\BWF_MetaEdit_GUI.exe BWF_MetaEdit_GUI_Windows_i386\bwfmetaedit-gui.exe

@rem --- Copying : Information files ---
copy ..\License.html BWF_MetaEdit_GUI_Windows_i386\
copy ..\History_GUI.txt BWF_MetaEdit_GUI_Windows_i386\History.txt
copy ..\conformance_point_document.xsd BWF_MetaEdit_GUI_Windows_i386\
copy ReadMe_GUI_Windows.txt BWF_MetaEdit_GUI_Windows_i386\ReadMe.txt


rem --- Compressing Archive ---
cd BWF_MetaEdit_GUI_Windows_i386\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -tzip -mx9 ..\BWF_MetaEdit_GUI_Windows_i386_WithoutInstaller.zip *
cd ..

rem --- Installer ---
..\..\Shared\Binary\Windows_i386\NSIS\makensis ..\Source\Install\BWF_MetaEdit_GUI_Windows_i386.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWF_MetaEdit_GUI_Windows_i386\ /S /Q
:SkipCleanUp