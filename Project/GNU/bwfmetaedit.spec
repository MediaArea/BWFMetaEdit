# Copyright (c) 2010 info@mediaarea.net
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

# norootforbuild

%define bwfmetaedit_version		1.3.1

Name:			bwfmetaedit
Version:		%bwfmetaedit_version
Release:		1
Summary:		Supplies technical and tag information about a video or audio file (CLI)
Group:			Productivity/Multimedia/Other
License:		GPL
URL:			http://mediainfo.sourceforge.net/
Packager:		Jerome Martinez <info@mediaarea.net>
Source0:		bwfmetaedit_%{version}-1.tar.gz
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires: 	gcc-c++
BuildRequires:	pkgconfig
%if 0%{?suse_version}
BuildRequires:	update-desktop-files
%endif
BuildRequires:  automake
BuildRequires:  autoconf

%description
bwfmetaedit CLI (Command Line Interface)

BWF MetaEdit provides this service:

(To be filled)

%package gui
Summary:	Supplies technical and tag information about a video or audio file (GUI)
Group:		Productivity/Multimedia/Other
BuildRequires:	libqt4-devel
%if 0%{?suse_version}
BuildRequires:	update-desktop-files
%endif
%if 0%{?centos_version} ||  0%{?rhel_version} || 0%{?fedora_version}
Requires:	qt4 >= 4.0.0
%endif
%if 0%{?mandriva}
Requires:	libqtgui4 >= 4.0.0
%endif
%if 0%{?suse_version} ||  0%{?opensuse_version}
Requires:	libqt4 >= 4.0.0
Requires:	libqt4-x11 >= 4.0.0
%endif

%description gui
bwfmetaedit GUI (Graphical User Interface)

BWF MetaEdit provides this service:

(To be filled)

This package contains the graphical user interface

%prep
%setup -q -n BWF_MetaEdit
dos2unix     *.txt Release/*.txt
%__chmod 644 *.html *.txt Release/*.txt

%build
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"

# build CLI
pushd Project/GNU/CLI
	%__chmod +x autogen
	./autogen
	%configure

	%__make %{?jobs:-j%{jobs}}
popd

# now build GUI
pushd Project/GNU/GUI
	%__chmod +x autogen
	./autogen
	%configure

	%__make %{?jobs:-j%{jobs}}
popd

%install
pushd Project/GNU/CLI
	%__make install-strip DESTDIR=%{buildroot}
popd

pushd Project/GNU/GUI
	%__make install-strip DESTDIR=%{buildroot}
popd

# icon
%__install -dm 755 %{buildroot}%{_datadir}/icons/hicolor/128x128/apps
%__install -m 644 Source/Resource/Image/FADGI/Logo128.png \
	%{buildroot}%{_datadir}/icons/hicolor/128x128/apps/%{name}.png
%__install -dm 755 %{buildroot}%{_datadir}/pixmaps
%__install -m 644 Source/Resource/Image/FADGI/Logo.png \
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
%if 0%{?suse_version}
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/bwfmetaedit-gui.desktop AudioVideo AudioVideoEditing
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

%changelog
* Tue Jan 01 2010 Jerome Martinez <info@mediaarea.net> - 1.2.0-0
- See History.txt for more info and real dates

