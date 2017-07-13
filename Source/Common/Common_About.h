// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Common_AboutH
#define Common_AboutH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <string>

//***************************************************************************
//
//***************************************************************************

const char* NameVersion_HTML();
const char* NameVersion_Text();

const char* Description_HTML();
const char* Description_Text();

const char* Contact_HTML();
const char* Contact_Text();

const char* AuthorLicense_HTML();
const char* AuthorLicense_Text();

const char* Websites_HTML();
const char* Websites_Text();

const char* Columns_ToolTip(std::string Name);

#endif
