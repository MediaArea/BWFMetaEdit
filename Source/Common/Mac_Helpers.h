// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Common_MacHelpersH
#define Common_MacHelpersH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <string>

using namespace std;

//---------------------------------------------------------------------------
string makeUniqueFileName();
string makeTemporaryDirectoryForFile(const char *path);
bool deleteTemporaryDirectory(const char *path);
#endif
