#! /bin/sh

#############################################################################
# Configure
Home=`pwd`

#############################################################################
# Setup for parallel builds
Parallel_Make () {
    local numprocs=1
    case $OS in
    'linux')
        numprocs=`grep -c ^processor /proc/cpuinfo 2>/dev/null`
        ;;
    'mac')
        if type sysctl &> /dev/null; then
            numprocs=`sysctl -n hw.ncpu`
        fi
        ;;
    *) ;;
    esac

    if [ "$numprocs" = "" ] || [ "$numprocs" = "0" ]; then
        numprocs=1
    fi  
    make -s -j$numprocs
}

#############################################################################
# BWF_MetaEdit
if test -e Project/QtCreator/bwfmetaedit-gui.pro; then
 pushd Project/QtCreator
 test -e Makefile && rm Makefile
 ./prepare
 if test -e Makefile; then
  make clean
  Parallel_Make
  if test -e bwfmetaedit-gui || test -e "BWF MetaEdit.app" ; then
   echo BWF_MetaEdit compiled
  else
   echo Problem while compiling BWF_MetaEdit
   exit
  fi
 else
  echo Problem while configuring BWF_MetaEdit
  exit
 fi
else
 echo BWF_MetaEdit directory is not found
 exit
fi
cd $Home

#############################################################################
# Going home
cd $Home
echo "BWF MetaEdit (GUI) executable is in Project/QtCreator"
