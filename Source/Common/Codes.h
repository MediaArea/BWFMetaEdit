// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Common_CodesH
#define Common_CodesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/Utils.h"
#include <utility>
#include <string>
#include <map>

//---------------------------------------------------------------------------
typedef std::pair<std::string, bool> codes_entries;
typedef std::map<ZenLib::int16u, codes_entries> codes;

bool IsCSETCountry(ZenLib::int16u Code);
bool IsCSETLanguage(ZenLib::int16u Code);
bool IsCSETDialect(ZenLib::int16u Language, ZenLib::int16u Code);
bool IsISOCountry(ZenLib::int16u Code);
bool IsISOLanguage(ZenLib::int16u Code);

std::string Country_Get(ZenLib::int16u Code);
std::string Language_Get(ZenLib::int16u Code);
std::string Dialect_Get(ZenLib::int16u Language, ZenLib::int16u Code);

const codes* CSETCountries_Get();
const codes* CSETLanguages_Get();
const codes* CSETDialects_Get();
const codes* ISOCountries_Get();
const codes* ISOLanguages_Get();

#endif
