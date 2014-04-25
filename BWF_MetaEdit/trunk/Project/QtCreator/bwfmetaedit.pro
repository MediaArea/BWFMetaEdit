QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "BWF MetaEdit"
TEMPLATE = app

CONFIG += qt release
CONFIG += no_keywords

DEFINES += TIXML_USE_STL

HEADERS = \
    ../../Source/Common/Common_About.h \
    ../../Source/Common/Core.h \
    ../../Source/GUI/Qt/App.h \
    ../../Source/GUI/Qt/GUI_About.h \
    ../../Source/GUI/Qt/GUI_Help.h \
    ../../Source/GUI/Qt/GUI_Main.h \
    ../../Source/GUI/Qt/GUI_Main_Core_Table.h \
    ../../Source/GUI/Qt/GUI_Main_Core_Text.h \
    ../../Source/GUI/Qt/GUI_Main_Output_Log.h \
    ../../Source/GUI/Qt/GUI_Main_Output_Trace.h \
    ../../Source/GUI/Qt/GUI_Main_Technical_Table.h \
    ../../Source/GUI/Qt/GUI_Main_Technical_Text.h \
    ../../Source/GUI/Qt/GUI_Main_UndoDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_Bext.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_DateDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_Loudness.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_TextEditDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx_UmidDialog.h \
    ../../Source/GUI/Qt/GUI_Main_xxxx__Common.h \
    ../../Source/GUI/Qt/GUI_Preferences.h \
    ../../Source/MD5/md5.h \
    ../../Source/Riff/Riff_Base.h \
    ../../Source/Riff/Riff_Chunks.h \
    ../../Source/Riff/Riff_Handler.h \
    ../../Source/TinyXml/tinystr.h \
    ../../Source/TinyXml/tinyxml.h \
    ../../Source/ZenLib/BitStream.h \
    ../../Source/ZenLib/BitStream_LE.h \
    ../../Source/ZenLib/Conf.h \
    ../../Source/ZenLib/Conf_Internal.h \
    ../../Source/ZenLib/CriticalSection.h \
    ../../Source/ZenLib/Dir.h \
    ../../Source/ZenLib/File.h \
    ../../Source/ZenLib/FileName.h \
    ../../Source/ZenLib/InfoMap.h \
    ../../Source/ZenLib/int128s.h \
    ../../Source/ZenLib/int128u.h \
    ../../Source/ZenLib/MemoryDebug.h \
    ../../Source/ZenLib/OS_Utils.h \
    ../../Source/ZenLib/PreComp.h \
    ../../Source/ZenLib/Thread.h \
    ../../Source/ZenLib/Trace.h \
    ../../Source/ZenLib/Translation.h \
    ../../Source/ZenLib/Utils.h \
    ../../Source/ZenLib/Ztring.h \
    ../../Source/ZenLib/ZtringList.h \
    ../../Source/ZenLib/ZtringListList.h \
    ../../Source/ZenLib/ZtringListListF.h

SOURCES = \
    ../../Source/Common/Common_About.cpp \
    ../../Source/Common/Core.cpp \
    ../../Source/GUI/Qt/App.cpp \
    ../../Source/GUI/Qt/GUI_About.cpp \
    ../../Source/GUI/Qt/GUI_Help.cpp \
    ../../Source/GUI/Qt/GUI_Main.cpp \
    ../../Source/GUI/Qt/GUI_Main_Core_Table.cpp \
    ../../Source/GUI/Qt/GUI_Main_Core_Text.cpp \
    ../../Source/GUI/Qt/GUI_Main_Menu.cpp \
    ../../Source/GUI/Qt/GUI_Main_Output_Log.cpp \
    ../../Source/GUI/Qt/GUI_Main_Output_Trace.cpp \
    ../../Source/GUI/Qt/GUI_Main_Technical_Table.cpp \
    ../../Source/GUI/Qt/GUI_Main_Technical_Text.cpp \
    ../../Source/GUI/Qt/GUI_Main_UndoDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_Bext.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_DateDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_Loudness.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_TextEditDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx_UmidDialog.cpp \
    ../../Source/GUI/Qt/GUI_Main_xxxx__Common.cpp \
    ../../Source/GUI/Qt/GUI_Preferences.cpp \
    ../../Source/MD5/md5.c \
    ../../Source/Riff/Riff_Base.cpp \
    ../../Source/Riff/Riff_Base_Streams.cpp \
    ../../Source/Riff/Riff_Chunks_.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_aXML.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_bext.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_data.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_ds64.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_FLLR.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_fmt_.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_INFO.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_INFO_xxxx.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_iXML.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE_MD5_.cpp \
    ../../Source/Riff/Riff_Chunks_WAVE__PMX.cpp \
    ../../Source/Riff/Riff_Handler.cpp \
    ../../Source/TinyXml/tinystr.cpp \
    ../../Source/TinyXml/tinyxml.cpp \
    ../../Source/TinyXml/tinyxmlerror.cpp \
    ../../Source/TinyXml/tinyxmlparser.cpp \
    ../../Source/ZenLib/Conf.cpp \
    ../../Source/ZenLib/CriticalSection.cpp \
    ../../Source/ZenLib/Dir.cpp \
    ../../Source/ZenLib/File.cpp \
    ../../Source/ZenLib/FileName.cpp \
    ../../Source/ZenLib/InfoMap.cpp \
    ../../Source/ZenLib/int128s.cpp \
    ../../Source/ZenLib/int128u.cpp \
    ../../Source/ZenLib/MemoryDebug.cpp \
    ../../Source/ZenLib/OS_Utils.cpp \
    ../../Source/ZenLib/PreComp.cpp \
    ../../Source/ZenLib/Thread.cpp \
    ../../Source/ZenLib/Translation.cpp \
    ../../Source/ZenLib/Utils.cpp \
    ../../Source/ZenLib/Ztring.cpp \
    ../../Source/ZenLib/ZtringList.cpp \
    ../../Source/ZenLib/ZtringListList.cpp \
    ../../Source/ZenLib/ZtringListListF.cpp

RESOURCES += \
    ../../Source/Resource/GUI_Main.qrc

INCLUDEPATH += $$PWD/../../Source

macx:
{
    ICON = ../../Source/Resource/Image/FADGI/Logo.icns 
    QMAKE_INFO_PLIST = Info.plist
}