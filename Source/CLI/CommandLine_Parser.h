// BWF MetaEdit CLI - A Command Line Interface for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef CommandLine_ParserH
#define CommandLine_ParserH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
//#include <tchar.h>
//---------------------------------------------------------------------------

//***************************************************************************
// Command line parser
//***************************************************************************

int Parse(Core &C, string &Argument);

//***************************************************************************
// Per option
//***************************************************************************

//---------------------------------------------------------------------------
// Defines
#define CL_METHOD(_NAME) \
    int CL_##_NAME(Core &C, const string &Argument)

#define CL_OPTION(_NAME) \
    int CL_##_NAME(Core &C, const string &Argument)

//---------------------------------------------------------------------------
CL_OPTION(Help);
CL_OPTION(Version);

//---------------------------------------------------------------------------
CL_OPTION(riff2rf64_Reject);
CL_OPTION(Overwrite_Reject);
CL_OPTION(NoPadding_Accept);
CL_OPTION(Errors_Continue);
CL_OPTION(Append);
CL_OPTION(Log_cout);
CL_OPTION(Simulate);
CL_OPTION(SpecialChars);
CL_OPTION(Encoding);
CL_OPTION(Write_Encoding);
CL_OPTION(Fallback_Encoding);
CL_OPTION(Write_CodePage);
CL_OPTION(In_CSET_Remove);
CL_OPTION(Ignore_File_Encoding);
CL_OPTION(Chunks_Remove);

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
CL_OPTION(Out_XML_cout);
CL_OPTION(Out_XML_File);

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_cout);
CL_OPTION(Out_Tech_File);
CL_OPTION(Out_Tech_XML);
CL_OPTION(Out_Tech_XML_File);

//---------------------------------------------------------------------------
CL_OPTION(In_Core_File);
CL_OPTION(In_Core_Remove);
CL_OPTION(In_Core_XML);
CL_OPTION(Out_Core_cout);
CL_OPTION(Out_Core_File);
CL_OPTION(Out_Core_XML);
CL_OPTION(Out_Core_XML_File);

//---------------------------------------------------------------------------
CL_OPTION(In__PMX_File);
CL_OPTION(In__PMX_Remove);
CL_OPTION(In__PMX_XML);
CL_OPTION(Out__PMX_cout);
CL_OPTION(Out__PMX_File);
CL_OPTION(Out__PMX_XML);

//---------------------------------------------------------------------------
CL_OPTION(In_aXML_File);
CL_OPTION(In_aXML_Remove);
CL_OPTION(In_aXML_XML);
CL_OPTION(Out_aXML_cout);
CL_OPTION(Out_aXML_File);
CL_OPTION(Out_aXML_XML);

//---------------------------------------------------------------------------
CL_OPTION(In_iXML_File);
CL_OPTION(In_iXML_Remove);
CL_OPTION(In_iXML_XML);
CL_OPTION(Out_iXML_cout);
CL_OPTION(Out_iXML_File);
CL_OPTION(Out_iXML_XML);

//---------------------------------------------------------------------------
CL_OPTION(In_cue__File);
CL_OPTION(In_cue__Remove);
CL_OPTION(In_cue__XML);
CL_OPTION(Out_cue__cout);
CL_OPTION(Out_cue__File);
CL_OPTION(Out_cue__XML);

//---------------------------------------------------------------------------
CL_OPTION(MD5_Generate);
CL_OPTION(MD5_Verify);
CL_OPTION(MD5_Embed);
CL_OPTION(MD5_Embed_Overwrite);

//---------------------------------------------------------------------------
CL_OPTION(Default);

#endif
