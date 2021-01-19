dnl #########################################################################
dnl ### Init
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Test if we are at the good place
dnl
AC_INIT(../../../Source/GUI/Qt/GUI_Main.cpp)

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD

dnl -------------------------------------------------------------------------
dnl Name and version
dnl
AM_INIT_AUTOMAKE(bwfmetaedit-gui,0)

dnl -------------------------------------------------------------------------
dnl Autotools init
dnl
CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

dnl #########################################################################
dnl ### Configure
dnl #########################################################################

Home=`pwd`

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Arguments - Enable
dnl
AC_ARG_ENABLE(debug,            AC_HELP_STRING([--enable-debug],            [Enable full debug]),                         , enable_debug=no)
AC_ARG_ENABLE(gprof,            AC_HELP_STRING([--enable-gprof],            [Enable gprof log]),                          , enable_gprof=no)
AC_ARG_ENABLE(universal_binary, AC_HELP_STRING([--enable-universal_binary], [Create Mac x84_64/arm64 Universal binary]),  , enable_universal_binary=no)
AC_ARG_ENABLE(arch-ppc,         AC_HELP_STRING([--with-arch-ppc],           [Create Mac PPC Universal binary]),           , enable_arch_ppc=no)
AC_ARG_ENABLE(arch-i386,        AC_HELP_STRING([--with-arch-i386],          [Create Mac i386 Universal binary]),          , enable_arch_i386=no)
AC_ARG_ENABLE(arch-x86_64,      AC_HELP_STRING([--enable-arch-x86_64],      [Create Mac x86_64 architecture]),            , enable_arch_x86_64=no)
AC_ARG_ENABLE(arch-arm64,       AC_HELP_STRING([--enable-arch-arm64],       [Create Mac arm64 architecture]),             , enable_arch_arm64=no)
AC_ARG_ENABLE(large_files,      AC_HELP_STRING([--disable-large_files],     [Disable large files support]),               , enable_large_files=yes)

dnl -------------------------------------------------------------------------
dnl Arguments - With
dnl
AC_ARG_WITH(macosx-sdk,         AC_HELP_STRING([--with-macosx-sdk],         [Force the Mac SDK]),                         , with_macosx_sdk=no)
AC_ARG_WITH(macosx-version-min, AC_HELP_STRING([--with-macosx-version-min], [Force the Mac Version]),                     , with_macosx_version_min=no)

dnl #########################################################################
dnl ### Parsing options
dnl #########################################################################

dnl #########################################################################
dnl ### Specific flags
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Use Qt (v2)
dnl
dnl if test "$with_qt" != "no"; then
	if test -d ../../../qt/include; then
		CFLAGS="$CFLAGS -I../../../qt/include"
		CXXFLAGS="$CXXFLAGS -I../../../qt/include"

		MOC="../../../qt/bin/moc"
		RCC="../../../qt/bin/rcc"

		case "${build}" in
			*-*-darwin* )
				# Qt5 Don't support pkgconfig on MacOS
				CFLAGS="$CFLAGS -I../../../qt/lib/QtCore.framework/Headers -I../../../qt/lib/QtGui.framework/Headers -I../../../qt/lib/QtWidgets.framework/Headers -F../../../qt/lib"
				CXXFLAGS="$CXXFLAGS -I../../../qt/lib/QtCore.framework/Headers -I../../../qt/lib/QtGui.framework/Headers -I../../../qt/lib/QtWidgets.framework/Headers -F../../../qt/lib"
				LIBS="$LIBS -framework Carbon -framework AppKit -framework ApplicationServices -framework QtCore -framework QtGui -framework QtWidgets"
				break
			;;
			*)
				LIBS="$LIBS -L../../../qt/lib $LIBS -lQtCore -lQtGui -lQtWidgets -lz"
			;;
		esac
		with_qt="builtin"

	elif pkg-config --exists Qt5Widgets; then
		CFLAGS="$CFLAGS $(pkg-config Qt5Widgets --cflags)"
		CXXFLAGS="$CXXFLAGS $(pkg-config Qt5Widgets --cflags)"
		LIBS="$LIBS $(pkg-config Qt5Widgets --libs)"

		MOC="$(pkg-config Qt5Gui --variable=prefix)/bin/moc"
		RCC="$(pkg-config Qt5Gui --variable=prefix)/bin/rcc"

		with_qt="system (qt5)"
	elif pkg-config --exists QtGui; then
		CFLAGS="$CFLAGS $(pkg-config QtGui --cflags)"
		CXXFLAGS="$CXXFLAGS $(pkg-config QtGui --cflags)"
		LIBS="$LIBS $(pkg-config QtGui --libs)"

		MOC="$(pkg-config QtGui --variable=prefix)/bin/moc"
		RCC="$(pkg-config QtGui --variable=prefix)/bin/rcc"

		with_qt="system (qt4)"
	else
		AC_MSG_ERROR([Qt configuration is not found])
	fi

	if test ! -x $MOC ; then
		AC_MSG_ERROR([
		Qt MOC is not found on your qt directory
		Have you compiled Qt?
		])
	fi
	if test ! -x $RCC; then
		AC_MSG_ERROR([
		Qt RCC is not found on your qt directory
		Have you compiled Qt?
		])
	fi

	test -e _Automated || mkdir _Automated

	$MOC ../../../Source/GUI/Qt/GUI_About.h -o_Automated/moc_GUI_About.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Help.h -o_Automated/moc_GUI_Help.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main.h -o_Automated/moc_GUI_Main.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Core_Table.h -o_Automated/moc_GUI_Main_Core_Table.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Core_Text.h -o_Automated/moc_GUI_Main_Core_Text.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Output_Log.h -o_Automated/moc_GUI_Main_Output_Log.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Output_stderr.h -o_Automated/moc_GUI_Main_Output_stderr.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Output_Trace.h -o_Automated/moc_GUI_Main_Output_Trace.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Technical_Table.h -o_Automated/moc_GUI_Main_Technical_Table.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_Technical_Text.h -o_Automated/moc_GUI_Main_Technical_Text.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_UndoDialog.h -o_Automated/moc_GUI_Main_UndoDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx__Common.h -o_Automated/moc_GUI_Main_xxxx__Common.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_Bext.h -o_Automated/moc_GUI_Main_xxxx_Bext.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h -o_Automated/moc_GUI_Main_xxxx_CodingHistoryDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_DateDialog.h -o_Automated/moc_GUI_Main_xxxx_DateDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_Loudness.h -o_Automated/moc_GUI_Main_xxxx_Loudness.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_TextEditDialog.h -o_Automated/moc_GUI_Main_xxxx_TextEditDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h -o_Automated/moc_GUI_Main_xxxx_TimeReferenceDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Main_xxxx_UmidDialog.h -o_Automated/moc_GUI_Main_xxxx_UmidDialog.cpp
	$MOC ../../../Source/GUI/Qt/GUI_Preferences.h -o_Automated/moc_GUI_Preferences.cpp

	$RCC ../../../Source/Resource/GUI_Main.qrc -o _Automated/qrc_GUI_Main.cpp
dnl fi

dnl -------------------------------------------------------------------------
dnl Use TinyXML
dnl
CXXFLAGS="$CXXFLAGS -DTIXML_USE_STL"

dnl -------------------------------------------------------------------------
dnl Use ZenLib
dnl
CXXFLAGS="$CXXFLAGS -pthread -fPIC"
LDFLAGS="$LDFLAGS -lpthread -pthread"

dnl -------------------------------------------------------------------------
dnl Large files
dnl
if test "$enable_large_files" = "yes"; then
	CXXFLAGS="$CXXFLAGS -D_LARGE_FILES -D_FILE_OFFSET_BITS=64"
	ZenLib_LargeFiles="yes"
else
	ZenLib_LargeFiles="no"
fi

dnl #########################################################################
dnl ### C/C++ compiler options used to compile
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
dnl C/C++ compiler options used to compile
if test "$GXX" = yes ; then
	if test "$enable_debug" = "yes"; then
		CFLAGS="$CFLAGS -O0 -g -Wall -Wundef"
		CXXFLAGS="$CXXFLAGS -O0 -g -Wall -W -Wundef -Wcast-qual -Wno-ctor-dtor-privacy"
	else
		CFLAGS="$CFLAGS -O2"
		CXXFLAGS="$CXXFLAGS -O2"
	fi
	if test "$enable_gprof" = "yes"; then
		CFLAGS="$CFLAGS -pg -g"
		CXXFLAGS="$CXXFLAGS -pg -g"
	fi
fi

dnl #########################################################################
dnl ### Compiler specific
dnl #########################################################################

dnl -------------------------------------------------------------------------
case "${build}" in
  powerpc-montavista* )
    CFLAGS="$CFLAGS -D__MONTAVISTA__"
    CXXFLAGS="$CXXFLAGS -D__MONTAVISTA__"
  ;;
esac

dnl -------------------------------------------------------------------------
dnl Mac Architecture and SDK
dnl
if test "$enable_universal_binary" = "yes"; then
	enable_arch_x86_64=yes
	enable_arch_arm64=yes
fi
if test "$enable_arch_ppc" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch ppc"
	CFLAGS="$CFLAGS -arch ppc"
	LDFLAGS="$LDFLAGS -arch ppc"
fi
if test "$enable_arch_i386" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch i386"
	CFLAGS="$CFLAGS -arch i386"
	LDFLAGS="$LDFLAGS -arch i386"
fi
if test "$enable_arch_x86_64" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch x86_64"
	CFLAGS="$CFLAGS -arch x86_64"
	LDFLAGS="$LDFLAGS -arch x86_64"
fi
if test "$enable_arch_arm64" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch arm64"
	CFLAGS="$CFLAGS -arch arm64"
	LDFLAGS="$LDFLAGS -arch arm64"
fi
if test "x$with_macosx_sdk" != xno ; then
	CXXFLAGS="-isysroot $with_macosx_sdk $CXXFLAGS"
	CFLAGS="-isysroot $with_macosx_sdk $CFLAGS"
	LDFLAGS="-isysroot $with_macosx_sdk $LDFLAGS"
fi
if test "x$with_macosx_version_min" != xno ; then
	CXXFLAGS="-mmacosx-version-min=$with_macosx_version_min $CXXFLAGS"
	CFLAGS="-mmacosx-version-min=$with_macosx_version_min $CFLAGS"
	LDFLAGS="-mmacosx-version-min=$with_macosx_version_min $LDFLAGS"
fi

dnl #########################################################################
dnl ### Output
dnl #########################################################################

AC_OUTPUT(Makefile)

dnl #########################################################################
dnl ### Report how we have been configured
dnl #########################################################################

echo ""
echo "Configured BWF MetaEdit for '$build'"
echo ""
echo "  Using Qt?                                               $with_qt"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LIBS: $LIBS"
