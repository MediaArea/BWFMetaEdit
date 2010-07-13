#! /bin/sh

#############################################################################
# Configure
Home=`pwd`

#############################################################################
# Setup for parallel builds
Zen_Make()
{
 if test -e /proc/stat; then
  numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
  if [ "$numprocs" = "0" ]; then
   numprocs=1
  fi
  make -s -j$numprocs
 else
  make
 fi
}

#############################################################################
# BWF_MetaEdit
if test -e Project/GNU/CLI/configure; then
 cd Project/GNU/CLI/
 test -e Makefile && rm Makefile
 chmod u+x configure
 ./configure $*
 if test -e Makefile; then
  make clean
  Zen_Make
  if test -e bwfmetaedit; then
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
echo "BWF MetaEdit (CLI) executable is in Project/GNU/CLI"
echo "For installing, cd Project/GNU/CLI && make install"