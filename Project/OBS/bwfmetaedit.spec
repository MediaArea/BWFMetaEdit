# Copyright (c) 2010 info@mediaarea.net
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

# norootforbuild

%global bwfmetaedit_version		26.01

# The rust version packaged with these distributions versions is too old to build the C2PA library
%if (!0%{?sle_version} || 0%{?sle_version} >= 150700) && (!0%{?mageia} || 0%{?mageia} >= 10) && (!0%{?rhel} || 0%{?rhel} >= 9)
%global build_c2pa_plugin 1
%else
%global build_c2pa_plugin 0
%endif

Name:			bwfmetaedit
Version:		%bwfmetaedit_version
Release:		1
Summary:		Supplies technical and tag information about a video or audio file (CLI)
Group:			Productivity/Multimedia/Other
License:		0BSD
URL:			https://mediaarea.net/BWFMetaEdit
Source0:		bwfmetaedit_%{version}.tar.xz
Source1:		bwfmetaedit-vendor_%{version}.tar.xz
Prefix:		%{_prefix}
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires:	pkgconfig
BuildRequires:	automake
BuildRequires:	autoconf
BuildRequires:	gcc-c++
%if %{build_c2pa_plugin}
BuildRequires:	cargo
BuildRequires:	rust
%endif

%if 0%{?rhel} >= 8
BuildRequires:  alternatives
%endif

%description
bwfmetaedit CLI (Command Line Interface)

BWF MetaEdit provides this service:

(To be filled)

%if %{build_c2pa_plugin}
%package plugin-c2pa
Summary:	C2PA (Content Credentials) support library for BWF MetaEdit
Group:		Productivity/Multimedia/Other

%description plugin-c2pa
C2PA Signature, validation and export support.
%endif

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

%__tar -xf %{SOURCE1} -C Source/ThirdParty/c2pa-rs
mkdir -p Source/ThirdParty/c2pa-rs/.cargo
cat > Source/ThirdParty/c2pa-rs/.cargo/config.toml <<'EOF'
[source.crates-io]
replace-with = "vendored-sources"

[source.vendored-sources]
directory = "vendor"
EOF

%build
export CFLAGS="-g $RPM_OPT_FLAGS"
export CXXFLAGS="-g $RPM_OPT_FLAGS"
export QMAKEOPTS="CONFIG+=force_debug_info"

%if %{build_c2pa_plugin}
# build c2pa-rs (offline, from the vendored dependencies unpacked during prep)
export CARGO_NET_OFFLINE=true
export CARGO_HOME="$(pwd)/.cargo_home"
pushd Source/ThirdParty/c2pa-rs
	cargo build --release --offline --locked -p c2pa-c-ffi \
		--no-default-features --features "rust_native_crypto, http, file_io"
	strip target/release/libc2pa_c.so
popd
%endif

# build CLI
pushd Project/GNU/CLI
	%__chmod +x autogen
	./autogen
	%if %{build_c2pa_plugin}
	LDFLAGS="$LDFLAGS -Wl,-rpath,%{_libdir}/%{name}" %configure --enable-c2pa=dynamic
	%else
	%configure
	%endif
	%__make %{?jobs:-j%{jobs}}
popd

# now build GUI
pushd Project/QtCreator
	%__chmod +x prepare
	%if %{build_c2pa_plugin}
	./prepare $QMAKEOPTS ENABLE_C2PA=dynamic QMAKE_RPATHDIR+=%{_libdir}/%{name} BINDIR=%{_bindir}
	%else
	./prepare BINDIR=%{_bindir}
	%endif
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

%if %{build_c2pa_plugin}
%__install -dm 755 %{buildroot}%{_libdir}/%{name}
%__install -m 755 Source/ThirdParty/c2pa-rs/target/release/libc2pa_c.so \
	%{buildroot}%{_libdir}/%{name}/libc2pa_c.so
%endif

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

%if %{build_c2pa_plugin}
%files plugin-c2pa
%defattr(-,root,root,-)
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/libc2pa_c.so
%endif

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
