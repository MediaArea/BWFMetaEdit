// BWF MetaEdit CLI - A Command Line Interface for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
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
    else if (Argument.find(_T(_TEXT))==0)        return CL_##_TOLAUNCH(C, Argument); \
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

    OPTION("--out-tech=",                                   Out_Tech_File)
    OPTION("--out-tech",                                    Out_Tech_cout)

    OPTION("--in-core=",                                    In_Core_File)
    OPTION("--in-core-remove",                              In_Core_Remove)
//    OPTION("--in-core-xml",                                 In_Core_XML)
    OPTION("--out-core=",                                   Out_Core_File)
    OPTION("--out-core-xml",                                Out_Core_XML)
    OPTION("--out-core",                                    Out_Core_cout)

    OPTION("--in-xmp=",                                     In__PMX_File)
    OPTION("--in-xmp-remove",                               In__PMX_Remove)
    OPTION("--in-xmp-xml",                                  In__PMX_XML)
    OPTION("--out-xmp-xml",                                 Out__PMX_XML)

    OPTION("--in-axml=",                                    In_aXML_File)
    OPTION("--in-axml-remove",                              In_aXML_Remove)
    OPTION("--in-axml-xml",                                 In_aXML_XML)
    OPTION("--out-axml-xml",                                Out_aXML_XML)

    OPTION("--in-ixml=",                                    In_iXML_File)
    OPTION("--in-ixml-remove",                              In_iXML_Remove)
    OPTION("--in-ixml-xml",                                 In_iXML_XML)
    OPTION("--out-ixml-xml",                                Out_iXML_XML)

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
CL_OPTION(Log_cout)
{
    C.Out_Log_cout=true;

    return -2; //Continue
}

//***************************************************************************
// Options - Technical
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_cout)
{
    C.Out_Tech_cout=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Out_Tech_File)
{
    //Form : --out-Tech=(FileName)
    C.Out_Tech_CSV_FileName.assign(Argument, 11, std::string::npos);

    return -2; //Continue
}

//***************************************************************************
// Options - Core
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(In_Core_File)
{
    //Form : --in-Core=(FileName)
    In_Core_File_List.push_back(Ztring(Argument, 10, std::string::npos));
    
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
    C.Out_Core_cout=true;

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
CL_OPTION(Out__PMX_XML)
{
    C.Out__PMX_XML=true;

    return -2; //Continue
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
CL_OPTION(Out_aXML_XML)
{
    C.Out_aXML_XML=true;

    return -2; //Continue
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
CL_OPTION(Out_iXML_XML)
{
    C.Out_iXML_XML=true;

    return -2; //Continue
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
        std::cout<<Argument<<" is unknown"<<std::endl;
        return 0;
    }
        
    Ztring Field(Argument, 2, Egal_Pos-2); Field.MakeLowerCase();
    if (Field!="description"
     && Field!="originator"
     && Field!="originatorreference"
     && Field!="originationdate"
     && Field!="originationtime"
     && Field!="timereference"
     && Field!="umid"
     && Field!="history"
     && Field!="xmp"
     && Field!="axml"
     && Field!="ixml"
     && Field.size()!=4) //All INFO sub-chunks
    {
        std::cout<<Argument<<" is unknown"<<std::endl;
        return 0;
    }

    string Value=string(Argument, Egal_Pos+1, std::string::npos);
    C.In_Core_Add(Field, Value);

    return -2; //Continue
}

