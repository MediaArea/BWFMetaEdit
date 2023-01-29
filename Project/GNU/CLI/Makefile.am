AUTOMAKE_OPTIONS = foreign subdir-objects

bin_PROGRAMS = bwfmetaedit
bwfmetaedit_SOURCES = \
    ../../../Source/CLI/CLI_Help.cpp \
    ../../../Source/CLI/CLI_Main.cpp \
    ../../../Source/CLI/CommandLine_Parser.cpp \
    ../../../Source/Common/Common_About.cpp \
    ../../../Source/Common/Codes.cpp \
    ../../../Source/Common/Core.cpp \
    ../../../Source/MD5/md5.c \
    ../../../Source/Riff/Riff_Base.cpp \
    ../../../Source/Riff/Riff_Base_Streams.cpp \
    ../../../Source/Riff/Riff_Chunks_.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE__PMX.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_aXML.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_bext.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_data.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_ds64.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_FLLR.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_fmt_.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_INFO.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_INFO_xxxx.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_iXML.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_MD5_.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_cue_.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_adtl.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_adtl_labl.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_adtl_note.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_adtl_ltxt.cpp \
    ../../../Source/Riff/Riff_Chunks_WAVE_CSET.cpp \
    ../../../Source/Riff/Riff_Handler.cpp \
    ../../../Source/TinyXml2/tinyxml2.cpp \
    ../../../Source/ZenLib/Conf.cpp \
    ../../../Source/ZenLib/CriticalSection.cpp \
    ../../../Source/ZenLib/Dir.cpp \
    ../../../Source/ZenLib/File.cpp \
    ../../../Source/ZenLib/FileName.cpp \
    ../../../Source/ZenLib/InfoMap.cpp \
    ../../../Source/ZenLib/int128s.cpp \
    ../../../Source/ZenLib/int128u.cpp \
    ../../../Source/ZenLib/MemoryDebug.cpp \
    ../../../Source/ZenLib/OS_Utils.cpp \
    ../../../Source/ZenLib/Translation.cpp \
    ../../../Source/ZenLib/Thread.cpp \
    ../../../Source/ZenLib/Utils.cpp \
    ../../../Source/ZenLib/Ztring.cpp \
    ../../../Source/ZenLib/ZtringList.cpp \
    ../../../Source/ZenLib/ZtringListList.cpp \
    ../../../Source/ZenLib/ZtringListListF.cpp

#bwfmetaedit_LDFLAGS     = -no-undefined -version-info 0:0:0
AM_CXXFLAGS = -std=c++11

AM_TESTS_FD_REDIRECT = 9>&2

TESTS = test/version.sh test/metadata.sh test/overwrite.sh test/null.sh test/gap.sh test/xmloutput.sh

AM_CPPFLAGS = -I../../../Source
