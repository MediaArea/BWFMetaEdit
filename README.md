# BWFMetaEdit README [![Build Status](https://travis-ci.org/MediaArea/BWFMetaEdit.svg?branch=master)](https://travis-ci.org/MediaArea/BWFMetaEdit)

BWF MetaEdit is a tool that supports embedding, validating, and exporting of metadata in Broadcast WAVE Format (BWF) files. It supports the [FADGI Broadcast WAVE Metadata Embedding Guidelines](http://www.digitizationguidelines.gov/guidelines/digitize-embedding.html).

# Download
[DownLoad BWF MetaEdit](https://mediaarea.net/BWFMetaEdit/Download)

# BWF MetaEdit features

* Import, edit, embed, and export specified metadata elements in WAVE audio files
* Export technical metadata from Format Chunks and minimal metadata from bext and INFO chunks as comma-separated values and/or XML, across a set of files or from individual files
* Evaluate, verify and embed MD5 checksums, as applied to the WAVE file’s data chunk (audio bitstream only)
* Enforce the guideline (above) developed by the Federal Agencies Audio-Visual Working Group, as well as specifications from the European Broadcasting Union (EBU), Microsoft, and IBM
* Generate reports that show errors in the construction of WAVE files

# Explanation of BWF MetaEdit Views and Documents

* [TECH](https://mediaarea.net/BWFMetaEdit/tech_view_help)
* [CORE](https://mediaarea.net/BWFMetaEdit/core_doc_help)
* [TRACE](https://mediaarea.net/BWFMetaEdit/trace)
* [Explanation of Errors and Warnings](https://mediaarea.net/BWFMetaEdit/errors)

# Metadata Standards

* [Definitions and Use of BEXT metadata](https://mediaarea.net/BWFMetaEdit/bext)
* [Definitions and Use of LIST-INFO metadata](https://mediaarea.net/BWFMetaEdit/listinfo)
* [XML chunks](https://mediaarea.net/BWFMetaEdit/xml_chunks)

# Use

* [Preferences](https://mediaarea.net/BWFMetaEdit/options)
* [Use of Audio Only Checksums](https://mediaarea.net/BWFMetaEdit/md5)
* [Application of Validation Rules](https://mediaarea.net/BWFMetaEdit/validation_rules_help)
* [Suggested Workflows](https://mediaarea.net/BWFMetaEdit/workflows)

# How to build BWFMetaEdit

## Build under Unix (macOS and Linux)

### Dependencies under macOS


*CLI and GUI dependencies*

* Xcode

*GUI only dependencies*

* Qt (in the PATH)


### Dependencies under Linux

*Build tools*

* git
* automake
* autoconf
* libtool
* pkgconfig
* make
* g++

*CLI and GUI dependencies*

* zlib

*GUI only dependencies*

* qt

#### Ubuntu

*Build tools and CLI/GUI dependencies*

```sh
apt-get install git automake autoconf libtool pkg-config make g++ zlib1g-dev
```

*GUI only dependencies*

```sh
apt-get install qtbase5-dev qtdeclarative5-dev qtquickcontrols2-5-dev libqt5svg5-dev
```

#### Fedora

*Build tools and CLI/GUI dependencies*

```sh
sudo dnf install git automake autoconf libtool pkgconfig make gcc-c++ zlib-devel
```

*GUI only dependencies*

```sh
sudo dnf install qt5-qtbase-devel qt5-qtquickcontrols2-devel qt5-qtsvg-devel
```

### Build BWF MetaEdit CLI


When you have done all the prerequisite for your configuration, then build BWF MetaEdit. We start with the CLI.

```sh
git clone https://github.com/MediaArea/BWFMetaEdit.git
cd BWFMetaEdit/Project/GNU/CLI
./autogen.sh
./configure
make
```

### Build BWF MetaEdit GUI

If you have already build the CLI, no need to run git twice. In fact, if you re-run git with an existing BWF MetaEdit directory, git will complain and exit.

To compile BWF MetaEdit GUI under macOS and Linux:

```sh
git clone https://github.com/MediaArea/BWFMetaEdit.git
cd BWFMetaEdit/Project/QtCreator
./prepare
make
```

## Build under Windows

*CLI and GUI dependencies*

* Microsoft Visual Studio with the Windows 8.1 SDK and the Git plugin (in the PATH)

*GUI only dependencies*

* Qt (in The PATH)

### Build BWF MetaEdit CLI


When you have done all the prerequisite for your configuration, then build BWF MetaEdit. We start with the CLI.

```bat
git clone https://github.com/MediaArea/BWFMetaEdit.git
devenv BWFMetaEdit\Project\MSVC2017\CLI\BWF_MetaEdit_CLI.vcxproj /upgrade
MSBuild BWFMetaEdit\Project\MSVC2017\CLI\BWF_MetaEdit_CLI.vcxproj
```

### Build BWF MetaEdit GUI

If you have already build the CLI, no need to run git twice. In fact, if you re-run git with an existing BWF MetaEdit directory, git will complain and exit.

To compile BWF MetaEdit GUI under macOS and Linux:

```bat
git clone https://github.com/MediaArea/BWFMetaEdit.git
cd BWFMetaEdit\Project\QtCreator
qmake
nmake
```

# License
It is [Open-Source](https://en.wikipedia.org/wiki/Open_source) software which means that end users and developers have freedom to study, to improve and to redistribute the program ([Public domain](https://mediaarea.net/BWFMetaEdit/License) for the code developed by us).
