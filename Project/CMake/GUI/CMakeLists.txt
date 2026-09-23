cmake_minimum_required(VERSION 3.16)

project(bwfmetaedit-gui LANGUAGES CXX C)

# Prefer Qt6, fall back to Qt5 if not found
find_package(Qt6 COMPONENTS Core QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core)
    set(QT_VERSION_MAJOR 5)
endif()

# Qt6 requires C++17 at minimum
if(QT_VERSION_MAJOR EQUAL 6)
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(APPLE)
    enable_language(OBJCXX)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_C2PA "Enable C2PA support" OFF)
option(C2PA_DYNAMIC "Load the C2PA library dynamically at runtime" OFF)
option(MACSTORE "Build for the Mac App Store" OFF)
option(WIN32_CONSOLE "On Windows, build as a console app so qWarning/qDebug show up when run from a terminal" OFF)

get_filename_component(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source" ABSOLUTE)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Quick QuickWidgets QuickControls2 Svg)

set(HEADERS
    ${SOURCE_DIR}/Common/Common_About.h
    ${SOURCE_DIR}/Common/Codes.h
    ${SOURCE_DIR}/Common/Core.h
    ${SOURCE_DIR}/GUI/Qt/App.h
    ${SOURCE_DIR}/GUI/Qt/GUI_About.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Help.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Core_Table.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_PerFile.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Core_Text.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Output_Log.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Output_Trace.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Technical_Table.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Technical_Text.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_UndoDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_Bext.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_DateDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_Loudness.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_TextEditDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_UmidDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_EditMenu.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CueDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CodePageDialog.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx__Common.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Colors.h
    ${SOURCE_DIR}/GUI/Qt/GUI_Preferences.h
    ${SOURCE_DIR}/MD5/md5.h
    ${SOURCE_DIR}/Riff/Riff_Base.h
    ${SOURCE_DIR}/Riff/Riff_Chunks.h
    ${SOURCE_DIR}/Riff/Riff_Handler.h
    ${SOURCE_DIR}/TinyXml2/tinyxml2.h
    ${SOURCE_DIR}/ZenLib/BitStream.h
    ${SOURCE_DIR}/ZenLib/BitStream_Fast.h
    ${SOURCE_DIR}/ZenLib/BitStream_LE.h
    ${SOURCE_DIR}/ZenLib/Conf.h
    ${SOURCE_DIR}/ZenLib/Conf_Internal.h
    ${SOURCE_DIR}/ZenLib/CriticalSection.h
    ${SOURCE_DIR}/ZenLib/Dir.h
    ${SOURCE_DIR}/ZenLib/File.h
    ${SOURCE_DIR}/ZenLib/FileName.h
    ${SOURCE_DIR}/ZenLib/InfoMap.h
    ${SOURCE_DIR}/ZenLib/int128s.h
    ${SOURCE_DIR}/ZenLib/int128u.h
    ${SOURCE_DIR}/ZenLib/MemoryDebug.h
    ${SOURCE_DIR}/ZenLib/MemoryUtils.h
    ${SOURCE_DIR}/ZenLib/OS_Utils.h
    ${SOURCE_DIR}/ZenLib/PreComp.h
    ${SOURCE_DIR}/ZenLib/Thread.h
    ${SOURCE_DIR}/ZenLib/Trace.h
    ${SOURCE_DIR}/ZenLib/Translation.h
    ${SOURCE_DIR}/ZenLib/Utils.h
    ${SOURCE_DIR}/ZenLib/Ztring.h
    ${SOURCE_DIR}/ZenLib/ZtringList.h
    ${SOURCE_DIR}/ZenLib/ZtringListList.h
    ${SOURCE_DIR}/ZenLib/ZtringListListF.h
)

set(SOURCES
    ${SOURCE_DIR}/Common/Common_About.cpp
    ${SOURCE_DIR}/Common/Codes.cpp
    ${SOURCE_DIR}/Common/Core.cpp
    ${SOURCE_DIR}/GUI/Qt/App.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_About.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Help.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Core_Table.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_PerFile.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Core_Text.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Menu.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Output_Log.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Output_Trace.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Technical_Table.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_Technical_Text.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_UndoDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_Bext.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_DateDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_Loudness.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_TextEditDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_UmidDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_EditMenu.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CueDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_CodePageDialog.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx__Common.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Colors.cpp
    ${SOURCE_DIR}/GUI/Qt/GUI_Preferences.cpp
    ${SOURCE_DIR}/MD5/md5.c
    ${SOURCE_DIR}/Riff/Riff_Base.cpp
    ${SOURCE_DIR}/Riff/Riff_Base_Streams.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_aXML.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_bext.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_data.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_ds64.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_FLLR.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_fmt_.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_INFO.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_INFO_xxxx.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_iXML.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_MD5_.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE__PMX.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_cue_.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_adtl.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_adtl_labl.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_adtl_note.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_adtl_ltxt.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_CSET.cpp
    ${SOURCE_DIR}/Riff/Riff_Chunks_WAVE_C2PA.cpp
    ${SOURCE_DIR}/Riff/Riff_Handler.cpp
    ${SOURCE_DIR}/TinyXml2/tinyxml2.cpp
    ${SOURCE_DIR}/ZenLib/Conf.cpp
    ${SOURCE_DIR}/ZenLib/CriticalSection.cpp
    ${SOURCE_DIR}/ZenLib/Dir.cpp
    ${SOURCE_DIR}/ZenLib/File.cpp
    ${SOURCE_DIR}/ZenLib/FileName.cpp
    ${SOURCE_DIR}/ZenLib/InfoMap.cpp
    ${SOURCE_DIR}/ZenLib/int128s.cpp
    ${SOURCE_DIR}/ZenLib/int128u.cpp
    ${SOURCE_DIR}/ZenLib/MemoryDebug.cpp
    ${SOURCE_DIR}/ZenLib/OS_Utils.cpp
    ${SOURCE_DIR}/ZenLib/PreComp.cpp
    ${SOURCE_DIR}/ZenLib/Thread.cpp
    ${SOURCE_DIR}/ZenLib/Translation.cpp
    ${SOURCE_DIR}/ZenLib/Utils.cpp
    ${SOURCE_DIR}/ZenLib/Ztring.cpp
    ${SOURCE_DIR}/ZenLib/ZtringList.cpp
    ${SOURCE_DIR}/ZenLib/ZtringListList.cpp
    ${SOURCE_DIR}/ZenLib/ZtringListListF.cpp
)

set(RESOURCES ${SOURCE_DIR}/Resource/GUI_Main.qrc)

if(ENABLE_C2PA)
    list(APPEND SOURCES
        ${SOURCE_DIR}/Riff/Riff_C2PA_Helpers.cpp
        ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_C2PADialog.cpp
    )

    list(APPEND HEADERS
        ${SOURCE_DIR}/ThirdParty/json/json.h
        ${SOURCE_DIR}/Riff/Riff_C2PA_Helpers.h
        ${SOURCE_DIR}/GUI/Qt/GUI_Main_xxxx_C2PADialog.h
    )

    if(WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(C2PA_TARGET_DIR ${SOURCE_DIR}/ThirdParty/c2pa-rs/target/i686-pc-windows-msvc/release)
        else()
            set(C2PA_TARGET_DIR ${SOURCE_DIR}/ThirdParty/c2pa-rs/target/x86_64-pc-windows-msvc/release)
        endif()
    else()
        set(C2PA_TARGET_DIR ${SOURCE_DIR}/ThirdParty/c2pa-rs/target/release)
    endif()
endif()

if(APPLE)
    set(APP_ICON ${SOURCE_DIR}/../Project/Mac/Logo.icns)
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    list(APPEND SOURCES ${SOURCE_DIR}/Common/Mac_Helpers.mm)
    list(APPEND HEADERS ${SOURCE_DIR}/Common/Mac_Helpers.h)
endif()

# The target name stays a plain identifier as some generators (e.g.
# Xcode or Visual Studio) don't like spaces there, the full
# "BWF MetaEdit" output name is applied below via OUTPUT_NAME instead.
set(TARGET_NAME bwfmetaedit-gui)

if(WIN32)
    add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES}
        ${SOURCE_DIR}/../Project/CMake/GUI/bwfmetaedit-gui.rc)
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "BWF MetaEdit")
    if(WIN32_CONSOLE)
        set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
    endif()
elseif(APPLE)
    add_executable(${TARGET_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${RESOURCES} ${APP_ICON})
    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME "BWF MetaEdit"
        MACOSX_BUNDLE_INFO_PLIST ${SOURCE_DIR}/../Project/Mac/Info.plist
        MACOSX_BUNDLE_ICON_FILE Logo.icns
    )
    target_link_libraries(${TARGET_NAME} PRIVATE "-framework CoreFoundation" "-framework Foundation")
else()
    add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${SOURCE_DIR})

target_compile_definitions(${TARGET_NAME} PRIVATE
    TIXML_USE_STL
    UNICODE
    _UNICODE
    _LARGE_FILES
    _FILE_OFFSET_BITS=64
    QT_NO_KEYWORDS
)

if(APPLE AND MACSTORE)
    target_compile_definitions(${TARGET_NAME} PRIVATE MACSTORE)
    target_compile_options(${TARGET_NAME} PRIVATE -gdwarf-2)
endif()

if(NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)
endif()

if(ENABLE_C2PA)
    target_compile_definitions(${TARGET_NAME} PRIVATE ENABLE_C2PA)
    target_include_directories(${TARGET_NAME} PRIVATE ${C2PA_TARGET_DIR})

    if(C2PA_DYNAMIC)
        target_compile_definitions(${TARGET_NAME} PRIVATE C2PA_DYNAMIC_LOADING)
    elseif(WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE ${C2PA_TARGET_DIR}/c2pa_c.dll.lib)
    else()
        target_link_directories(${TARGET_NAME} PRIVATE ${C2PA_TARGET_DIR})
        target_link_libraries(${TARGET_NAME} PRIVATE c2pa_c)
    endif()
endif()

target_link_libraries(${TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Svg
)

# Links additional Qt plugins needed for when using a static Qt build.
get_target_property(_qt_core_type Qt${QT_VERSION_MAJOR}::Core TYPE)
if(_qt_core_type STREQUAL "STATIC_LIBRARY")
    target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::QSvgPlugin)
    if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        if(COMMAND qt6_import_qml_plugins)
            set_property(TARGET ${TARGET_NAME} PROPERTY _qt_generated_qrc_files ${RESOURCES})
            qt6_import_qml_plugins(${TARGET_NAME})
        else()
            message(FATAL_ERROR "qt6_import_qml_plugins not found; a static build will not render PerFile.qml")
        endif()
    else()
        find_package(Qt5 COMPONENTS QmlImportScanner QUIET)
        if(COMMAND qt5_import_qml_plugins)
            qt5_import_qml_plugins(${TARGET_NAME} PATH_TO_SCAN "${SOURCE_DIR}/GUI/Qt")
        else()
            message(FATAL_ERROR "Qt5QmlImportScanner not found; a static build will not render PerFile.qml")
        endif()
    endif()
endif()
unset(_qt_core_type)

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
