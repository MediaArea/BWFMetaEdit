# Copyright (c) 2010 info@mediaarea.net
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

# norootforbuild

%global bwfmetaedit_version		24.10

Name:			bwfmetaedit
Version:		%bwfmetaedit_version
Release:		1
Summary:		Supplies technical and tag information about a video or audio file (CLI)
Group:			Productivity/Multimedia/Other
License:		0BSD
URL:			https://mediaarea.net/BWFMetaEdit
Source0:		bwfmetaedit_%{version}-1.tar.gz
Prefix:		%{_prefix}
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires:	pkgconfig
BuildRequires:	automake
BuildRequires:	autoconf
BuildRequires:	gcc-c++

%if 0%{?rhel} >= 8
BuildRequires:  alternatives
%endif

%description
bwfmetaedit CLI (Command Line Interface)

BWF MetaEdit provides this service:

(To be filled)

%package gui
Summary:	Supplies technical and tag information about a video or audio file (GUI)
Group:		Productivity/Multimedia/Other

BuildRequires:	pkgconfig(Qt5Gui)
BuildRequires:	pkgconfig(Qt5Svg)
BuildRequires:	pkgconfig(Qt5QuickWidgets)
BuildRequires:	pkgconfig(Qt5QuickControls2)

%if 0%{?suse_version}
BuildRequires:	update-desktop-files
%endif

%description gui
bwfmetaedit GUI (Graphical User Interface)

BWF MetaEdit provides this service:

(To be filled)

This package contains the graphical user interface

%prep
%setup -q -n bwfmetaedit
dos2unix     *.txt Release/*.txt conformance_point_document.xsd
%__chmod 644 *.html *.txt Release/*.txt

%build
export CFLAGS="-g $RPM_OPT_FLAGS"
export CXXFLAGS="-g $RPM_OPT_FLAGS"
export QMAKEOPTS="CONFIG+=force_debug_info"

# build CLI
pushd Project/GNU/CLI
	%__chmod +x autogen
	./autogen
	%configure

	%__make %{?jobs:-j%{jobs}}
popd

# now build GUI
pushd Project/QtCreator
	%__chmod +x prepare
	./prepare $QMAKEOPTS BINDIR=%{_bindir}

	%__make %{?jobs:-j%{jobs}}
popd

%install
pushd Project/GNU/CLI
	%__make install DESTDIR=%{buildroot}
popd

pushd Project/QtCreator
	%__make install INSTALL_ROOT=%{buildroot}
popd

# icon
%__install -dm 755 %{buildroot}%{_datadir}/icons/hicolor/128x128/apps
%__install -m 644 Source/Resource/Image/Logo/Logo128.png \
	%{buildroot}%{_datadir}/icons/hicolor/128x128/apps/%{name}.png
%__install -dm 755 %{buildroot}%{_datadir}/pixmaps
%__install -m 644 Source/Resource/Image/Logo/Logo.png \
	%{buildroot}%{_datadir}/pixmaps/%{name}.png

# menu-entry
%__install -dm 755 %{buildroot}/%{_datadir}/applications
%__install -m 644 Project/GNU/GUI/bwfmetaedit-gui.desktop \
	%{buildroot}/%{_datadir}/applications
%if 0%{?suse_version}
  %suse_update_desktop_file -n bwfmetaedit-gui AudioVideo AudioVideoEditing
%endif
%__install -dm 755 %{buildroot}/%{_datadir}/apps/konqueror/servicemenus
%__install -m 644 Project/GNU/GUI/bwfmetaedit-gui.kde3.desktop \
	%{buildroot}/%{_datadir}/apps/konqueror/servicemenus/bwfmetaedit-gui.desktop
%if 0%{?suse_version}
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/apps/konqueror/servicemenus/bwfmetaedit-gui.desktop AudioVideo AudioVideoEditing
%endif
%__install -dm 755 %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/
%__install -m 644 Project/GNU/GUI/bwfmetaedit-gui.kde4.desktop \
	%{buildroot}/%{_datadir}/kde4/services/ServiceMenus/bwfmetaedit-gui.desktop
%__install -dm 755 %{buildroot}/%{_datadir}/kservices5/ServiceMenus/
%__install -m 644 Project/GNU/GUI/bwfmetaedit-gui.kde4.desktop \
	%{buildroot}/%{_datadir}/kservices5/ServiceMenus/bwfmetaedit-gui.desktop
%if 0%{?suse_version}
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/bwfmetaedit-gui.desktop AudioVideo AudioVideoEditing
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/kservices5/ServiceMenus/bwfmetaedit-gui.desktop AudioVideo AudioVideoEditing
%endif
%if 0%{?fedora_version}
install -dm 755 %{buildroot}%{_datadir}/metainfo/
install -m 644 Project/GNU/GUI/bwfmetaedit-gui.metainfo.xml %{buildroot}%{_datadir}/metainfo/bwfmetaedit-gui.metainfo.xml
%else
install -dm 755 %{buildroot}%{_datadir}/appdata/
install -m 644 Project/GNU/GUI/bwfmetaedit-gui.metainfo.xml %{buildroot}%{_datadir}/appdata/bwfmetaedit-gui.appdata.xml
%endif

%clean
[ -d "%{buildroot}" -a "%{buildroot}" != "" ] && %__rm -rf "%{buildroot}"

%files
%defattr(-,root,root,-)
%doc Release/ReadMe_CLI_Linux.txt
%doc License.html History_CLI.txt conformance_point_document.xsd
%{_bindir}/bwfmetaedit

%files gui
%defattr(-,root,root,-)
%doc Release/ReadMe_GUI_Linux.txt
%doc License.html History_GUI.txt conformance_point_document.xsd
%{_bindir}/bwfmetaedit-gui
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps/*.png
%dir %{_datadir}/icons/hicolor
%dir %{_datadir}/icons/hicolor/128x128
%dir %{_datadir}/icons/hicolor/128x128/apps
%{_datadir}/icons/hicolor/128x128/apps/*.png
%dir %{_datadir}/apps
%dir %{_datadir}/apps/konqueror
%dir %{_datadir}/apps/konqueror/servicemenus
%{_datadir}/apps/konqueror/servicemenus/*.desktop
%dir %{_datadir}/kde4
%dir %{_datadir}/kde4/services
%dir %{_datadir}/kde4/services/ServiceMenus
%{_datadir}/kde4/services/ServiceMenus/*.desktop
%dir %{_datadir}/kservices5
%dir %{_datadir}/kservices5/ServiceMenus
%{_datadir}/kservices5/ServiceMenus/*.desktop
%if 0%{?fedora_version}
%dir %{_datadir}/metainfo
%{_datadir}/metainfo/*.xml
%else
%dir %{_datadir}/appdata
%{_datadir}/appdata/*.xml
%endif

%changelog
* Tue Jan 01 2010 Jerome Martinez <info@mediaarea.net> - 24.10-0
- See History.txt for more info and real dates
