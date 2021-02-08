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
#include "ZenLib/Ztring.h"

using namespace ZenLib;

//---------------------------------------------------------------------------
Ztring makeUniqueFileName();
Ztring makeTemporaryDirectoryForFile(const Ztring& path);
bool deleteTemporaryDirectory(const Ztring& path);
void clearNSMenu(void* menu);
#endif
