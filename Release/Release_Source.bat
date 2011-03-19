@echo off

rem --- Clean up ---
del bwfmetaedit_.tar.bz2
del bwfmetaedit_Source.tar
del bwfmetaedit_-1.tar.gz 
del bwfmetaedit__AllInclusive.7z
rmdir BWF_MetaEdit_Source /S /Q
mkdir BWF_MetaEdit_Source


@rem --- Copying : Sources ---
xcopy ..\Source\*.cpp BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.c BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.h BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.dfm BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.xml BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.rc BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.txt BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.csv BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.ico BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\Install\BWF_MetaEdit.nsi BWF_MetaEdit_Source\Source\Install\
xcopy ..\Source\*.sh  BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.png BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.html BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.qrc BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.bat BWF_MetaEdit_Source\Source\ /S
xcopy ..\Source\*.nsi BWF_MetaEdit_Source\Source\ /S

@rem --- Copying : Projects ---
xcopy ..\Project\*.bpg BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.bpf BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.bpr BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.cpp BWF_MetaEdit_Source\Project\ /S
del ..\Project\MSVC\GUI\_Automated\*.cpp
rmdir ..\Project\MSVC\GUI\_Automated
xcopy ..\Project\*.c BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.h BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.bdsgroup BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.bdsproj BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.dev BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.sln BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.vc BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.vcproj BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.vcxproj BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.vcxproj.filter BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.ico BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.workspace BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.cbp BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.def BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.idl BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.rgs BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\*.bat BWF_MetaEdit_Source\Project\ /S
xcopy ..\Project\BCB\BWF_MetaEdit*.res BWF_MetaEdit_Source\Project\BCB /S
xcopy ..\Project\MSVC2008\*.rc BWF_MetaEdit_Source\Project\MSVC2008\ /S
xcopy ..\Project\MSVC2010\*.rc BWF_MetaEdit_Source\Project\MSVC2010\ /S
xcopy ..\Project\GNU\* BWF_MetaEdit_Source\Project\GNU\ /S

@rem --- Copying : Release ---
xcopy *.txt BWF_MetaEdit_Source\Release\
xcopy *.bat BWF_MetaEdit_Source\Release\
xcopy *.sh  BWF_MetaEdit_Source\Release\
xcopy *.sub BWF_MetaEdit_Source\Release\
xcopy BCB\*.txt BWF_MetaEdit_Source\Release\BCB\ /S
xcopy MSVC\*.txt BWF_MetaEdit_Source\Release\MSVC\ /S
xcopy GCC_MinGW32\*.txt BWF_MetaEdit_Source\Release\GCC_MinGW32\ /S
xcopy GCC_Linux_i386\*.txt BWF_MetaEdit_Source\Release\GCC_Linux_i386\ /S
xcopy Make_GCC.sh BWF_MetaEdit_Source\Release\

@rem --- Copying : debian ---
xcopy ..\debian\* BWF_MetaEdit_Source\debian\ /S

rem --- Copying : Information files ---
xcopy ..\*.txt BWF_MetaEdit_Source\
xcopy ..\*.html BWF_MetaEdit_Source\
xcopy ..\*.xsd BWF_MetaEdit_Source\

rem --- Copying : Contrib files ---
xcopy ..\Contrib\*.doc BWF_MetaEdit_Source\Contrib\ /S

rem --- Copying : CVS files ---
xcopy ..\*.cvsignore BWF_MetaEdit_Source\


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
move BWF_MetaEdit_Source BWF_MetaEdit
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -ttar -mx9 bwfmetaedit.tar BWF_MetaEdit\*
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -tbzip2 -mx9 bwfmetaedit_.tar.bz2 bwfmetaedit.tar
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -tgzip -mx9 bwfmetaedit_-1.tar.gz bwfmetaedit.tar
del bwfmetaedit.tar
move BWF_MetaEdit BWF_MetaEdit_Source
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir BWF_MetaEdit_Source /S /Q
:SkipCleanUp
