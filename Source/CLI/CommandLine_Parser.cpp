// BWF MetaEdit CLI - A Command Line Interface for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "CLI/CommandLine_Parser.h"
#include "CLI/CLI_Help.h"
#include "Common/Common_About.h"
#include "ZenLib/ZtringList.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ZtringList In_Core_File_List;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//Parse Command Line
#define OPTION(_TEXT, _TOLAUNCH) \
    else if (Argument.find(_TEXT)==0)        return CL_##_TOLAUNCH(C, Argument); \
//---------------------------------------------------------------------------

//***************************************************************************
// Main
//***************************************************************************

int Parse(Core &C, string &Argument)
{
    if (0);
    OPTION("--help",                                        Help)
    OPTION("-h",                                            Help)
    OPTION("--version",                                     Version)

    OPTION("--reject-riff2rf64",                            riff2rf64_Reject)
    OPTION("--reject-overwrite",                            Overwrite_Reject)
    OPTION("--accept-nopadding",                            NoPadding_Accept)
    OPTION("--continue-errors",                             Errors_Continue)

    OPTION("--append",                                      Append)
    OPTION("-a",                                            Append)

    OPTION("--verbose",                                     Log_cout)
    OPTION("-v",                                            Log_cout)

    OPTION("--simulate",                                    Simulate)
    OPTION("-s",                                            Simulate)

    OPTION("--specialchars",                                SpecialChars)

    OPTION("--out-xml=",                                    Out_XML_File)
    OPTION("--out-xml",                                     Out_XML_cout)

    OPTION("--out-tech=",                                   Out_Tech_File)
    OPTION("--out-tech-xml=",                               Out_Tech_XML_File)
    OPTION("--out-tech-xml",                                Out_Tech_XML)
    OPTION("--out-tech",                                    Out_Tech_cout)

    OPTION("--in-core=",                                    In_Core_File)
    OPTION("--in-core-remove",                              In_Core_Remove)
//    OPTION("--in-core-xml",                                 In_Core_XML)
    OPTION("--out-core=",                                   Out_Core_File)
    OPTION("--out-core-xml=",                               Out_Core_XML_File)
    OPTION("--out-core-xml",                                Out_Core_XML)
    OPTION("--out-core",                                    Out_Core_cout)

    OPTION("--in-xmp=",                                     In__PMX_File)
    OPTION("--in-xmp-remove",                               In__PMX_Remove)
    OPTION("--in-xmp-xml",                                  In__PMX_XML)
    OPTION("--out-xmp-xml",                                 Out__PMX_XML)
    OPTION("--out-xmp=",                                    Out__PMX_File)
    OPTION("--out-xmp",                                     Out__PMX_cout)

    OPTION("--in-axml=",                                    In_aXML_File)
    OPTION("--in-axml-remove",                              In_aXML_Remove)
    OPTION("--in-axml-xml",                                 In_aXML_XML)
    OPTION("--out-axml-xml",                                Out_aXML_XML)
    OPTION("--out-axml=",                                   Out_aXML_File)
    OPTION("--out-axml",                                    Out_aXML_cout)

    OPTION("--in-ixml=",                                    In_iXML_File)
    OPTION("--in-ixml-remove",                              In_iXML_Remove)
    OPTION("--in-ixml-xml",                                 In_iXML_XML)
    OPTION("--out-ixml-xml",                                Out_iXML_XML)
    OPTION("--out-ixml=",                                   Out_iXML_File)
    OPTION("--out-ixml",                                    Out_iXML_cout)

    OPTION("--in-cue=",                                     In_cue__File)
    OPTION("--in-cue-remove",                               In_cue__Remove)
    OPTION("--in-cue-xml",                                  In_cue__XML)
    OPTION("--out-cue=",                                    Out_cue__File)
    OPTION("--out-cue",                                     Out_cue__cout)
    OPTION("--out-cue-xml",                                 Out_cue__XML)

    OPTION("--md5-generate",                                MD5_Generate)
    OPTION("--md5-verify",                                  MD5_Verify)
    OPTION("--md5-embed-overwrite",                         MD5_Embed_Overwrite)
    OPTION("--md5-embed",                                   MD5_Embed)
    
    //Default
    OPTION("--",                                            Default)
    OPTION("-",                                             Default)

    return -1; //This is a file
}

//***************************************************************************
// Options - General
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(Help)
{
    CL_Version(C, Argument);
    std::cout<<Help();

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Version)
{
    std::cout<<NameVersion_Text()<<std::endl;

    return 0;
}

//***************************************************************************
// Options - Configuration
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(riff2rf64_Reject)
{
    C.riff2rf64_Reject=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Overwrite_Reject)
{
    C.Overwrite_Reject=true;
    
    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(NoPadding_Accept)
{
    C.NoPadding_Accept=true;
    
    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Errors_Continue)
{
    C.Errors_Continue=true;
    
    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Append)
{
    C.NewChunksAtTheEnd=true;
    
    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Simulate)
{
    C.Simulation_Enabled=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(SpecialChars)
{
    C.SpecialChars_Enabled=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Log_cout)
{
    C.Out_Log_cout=true;

    return -2; //Continue
}

//***************************************************************************
// Options - XML
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(Out_XML_File)
{
    //Form : --out-xml=(FileName)
    C.Out_XML_FileName.assign(Argument, 10, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_XML_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_XML;

    return -2; //Continue
}

//***************************************************************************
// Options - Technical
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_Tech;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_File)
{
    //Form : --out-Tech=(FileName)
    C.Out_Tech_CSV_FileName.assign(Argument, 11, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_XML)
{
    C.Out_Tech_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_XML_File)
{
    //Form : --out-Core-XML=(FileName)
    C.Out_Tech_XML_FileName.assign(Argument, 15, std::string::npos);

    return -2; //Continue
}

//***************************************************************************
// Options - Core
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In_Core_File)
{
    //Form : --in-Core=(FileName)
    #ifdef _WIN32
    In_Core_File_List.push_back(Ztring(Ztring().From_UTF8(Argument), 10, std::string::npos));
    #else
    In_Core_File_List.push_back(Ztring(Ztring().From_Local(Argument), 10, std::string::npos));
    #endif
    
    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_Core_Remove)
{
    C.In_Core_Remove=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_Core_XML)
{
    C.In_Core_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Core_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_Core;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Core_File)
{
    //Form : --out-Core=(FileName)
    C.Out_Core_CSV_FileName.assign(Argument, 11, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Core_XML)
{
    C.Out_Core_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Core_XML_File)
{
    //Form : --out-Core-XML=(FileName)
    C.Out_Core_XML_FileName.assign(Argument, 15, std::string::npos);

    return -2; //Continue
}

//***************************************************************************
// Options - XMP
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In__PMX_File)
{
    //Form : --in-XMP=(FileName)
    C.In__PMX_FileName.assign(Argument, 9, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In__PMX_Remove)
{
    C.In__PMX_Remove=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In__PMX_XML)
{
    C.In__PMX_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out__PMX_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout__PMX;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out__PMX_XML)
{
    C.Out__PMX_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out__PMX_File)
{
    C.Out__PMX_FileName.assign(Argument, 10, std::string::npos);

    return -3; //Continue, one file mode
}

//***************************************************************************
// Options - aXML
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In_aXML_File)
{
    //Form : --in-aXML=(FileName)
    C.In_aXML_FileName.assign(Argument, 10, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_aXML_Remove)
{
    C.In_aXML_Remove=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_aXML_XML)
{
    C.In_aXML_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_aXML_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_aXML;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_aXML_XML)
{
    C.Out_aXML_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_aXML_File)
{
    C.Out_aXML_FileName.assign(Argument, 11, std::string::npos);

    return -3; //Continue, one file mode
}

//***************************************************************************
// Options - iXML
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In_iXML_File)
{
    //Form : --in-iXML=(FileName)
    C.In_iXML_FileName.assign(Argument, 10, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_iXML_Remove)
{
    C.In_iXML_Remove=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_iXML_XML)
{
    C.In_iXML_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_iXML_cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_iXML;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_iXML_XML)
{
    C.Out_iXML_XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_iXML_File)
{
    C.Out_iXML_FileName.assign(Argument, 11, std::string::npos);

    return -3; //Continue, one file mode
}


//***************************************************************************
// Options - cue
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In_cue__File)
{
    //Form : --in-cue=(FileName)
    C.In_cue__FileName.assign(Argument, 9, std::string::npos);

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_cue__Remove)
{
    C.In_cue__Remove=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_cue__XML)
{
    C.In_cue__XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_cue__cout)
{
    if (C.Cout!=Core::Cout_None)
    {
        std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
        return 1;
    }

    C.Cout=Core::Cout_cue_;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_cue__XML)
{
    C.Out_cue__XML=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_cue__File)
{
    C.Out_cue__FileName.assign(Argument, 10, std::string::npos);

    return -3; //Continue, one file mode
}

//***************************************************************************
// Options - MD5
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(MD5_Generate)
{
    C.GenerateMD5=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(MD5_Verify)
{
    C.GenerateMD5=true;
    C.VerifyMD5=true;
    C.VerifyMD5_Force=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(MD5_Embed)
{
    C.GenerateMD5=true;
    C.EmbedMD5=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(MD5_Embed_Overwrite)
{
    C.GenerateMD5=true;
    C.EmbedMD5=true;
    C.EmbedMD5_AuthorizeOverWritting=true;

    return -2; //Continue
}

//***************************************************************************
// Options - Default
//***************************************************************************

CL_OPTION(Default)
{
    //Form : --(Field)=(Value)
    size_t Egal_Pos;
    if (Argument.size()==2)
        Egal_Pos=string::npos;
    else
        Egal_Pos=Argument.find('=');
    if (Egal_Pos==string::npos)
    {
        std::cerr<<Argument<<" is unknown"<<std::endl;
        return 1;
    }

    #ifdef _WIN32
    Ztring Field(Ztring().From_UTF8(Argument), 2, Egal_Pos-2); Field.MakeLowerCase();
    #else
    Ztring Field(Ztring().From_Local(Argument), 2, Egal_Pos-2); Field.MakeLowerCase();
    #endif
    if (Field!=__T("description")
     && Field!=__T("originator")
     && Field!=__T("originatorreference")
     && Field!=__T("originationdate")
     && Field!=__T("originationtime")
     && Field!=__T("timereference")
     && Field!=__T("bextversion")
     && Field!=__T("loudnessvalue")
     && Field!=__T("loudnessrange")
     && Field!=__T("maxtruepeaklevel")
     && Field!=__T("maxmomentaryloudness")
     && Field!=__T("maxshorttermloudness")
     && Field!=__T("umid")
     && Field!=__T("history")
     && Field!=__T("xmp")
     && Field!=__T("axml")
     && Field!=__T("ixml")
     && Field.size()!=4) //All INFO sub-chunks
    {
        std::cerr<<Argument<<" is unknown"<<std::endl;
        return 1;
    }

    string Value=string(Argument, Egal_Pos+1, std::string::npos);
    C.In_Core_Add(Field.To_UTF8(), Value);

    return -2; //Continue
}

