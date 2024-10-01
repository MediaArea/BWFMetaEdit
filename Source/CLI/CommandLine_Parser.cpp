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
#include <cctype>
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

    OPTION("--in-cset-remove",                              In_CSET_Remove)
    OPTION("--specialchars",                                SpecialChars)
    OPTION("--ignore-file-encoding",                        Ignore_File_Encoding)
    OPTION("--encoding=",                                   Encoding)
    OPTION("--fallback-encoding=",                          Fallback_Encoding)
    OPTION("--write-encoding=",                             Write_Encoding)
    OPTION("--write-encoding",                              Write_CodePage)

    OPTION("--out-xml=",                                    Out_XML_File)
    OPTION("--out-xml",                                     Out_XML_cout)

    OPTION("--out-tech-xml=",                               Out_Tech_XML_File)
    OPTION("--out-tech-xml",                                Out_Tech_XML)
    OPTION("--out-tech=",                                   Out_Tech_File)
    OPTION("--out-tech",                                    Out_Tech_cout)

    OPTION("--in-core=",                                    In_Core_File)
    OPTION("--in-core-remove",                              In_Core_Remove)
//    OPTION("--in-core-xml",                                 In_Core_XML)
    OPTION("--out-core-xml=",                               Out_Core_XML_File)
    OPTION("--out-core-xml",                                Out_Core_XML)
    OPTION("--out-core=",                                   Out_Core_File)
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
    OPTION("--out-cue-xml",                                 Out_cue__XML)
    OPTION("--out-cue",                                     Out_cue__cout)

    OPTION("--md5-generate",                                MD5_Generate)
    OPTION("--md5-verify",                                  MD5_Verify)
    OPTION("--md5-embed-overwrite",                         MD5_Embed_Overwrite)
    OPTION("--md5-embed",                                   MD5_Embed)

    OPTION("--remove-chunks=",                              Chunks_Remove)

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
CL_OPTION(Encoding)
{
    std::string Value=Argument.substr(11);
    if (Value.size()==5 && std::tolower(Value[0])=='u' && std::tolower(Value[1])=='t' && std::tolower(Value[2])=='f' && Value[3]=='-' && Value[4]=='8')
        C.Encoding=Encoding_UTF8;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='4' && Value[3]=='3' && Value[4]=='7')
        C.Encoding=Encoding_CP437;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='0')
        C.Encoding=Encoding_CP850;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='8')
        C.Encoding=Encoding_CP858;
    else if (Value.size()==6 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='1' && Value[3]=='2' && Value[4]=='5' && Value[5]=='2')
        C.Encoding=Encoding_CP1252;
    else if (Value=="8859-1")
        C.Encoding=Encoding_8859_1;
    else if (Value=="8859-2")
        C.Encoding=Encoding_8859_2;
    else if (Value.size()==5 && std::tolower(Value[0])=='l' && std::tolower(Value[1])=='o' && std::tolower(Value[2])=='c' && std::tolower(Value[3])=='a' && std::tolower(Value[4])=='l')
        C.Encoding=Encoding_Local;
    else
    {
        std::cout<<Value<<" unknown encoding"<<std::endl;
        return 0;
    }

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Fallback_Encoding)
{
    std::string Value=Argument.substr(20);
    if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='4' && Value[3]=='3' && Value[4]=='7')
        C.Fallback_Encoding=Encoding_CP437;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='0')
        C.Fallback_Encoding=Encoding_CP850;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='8')
        C.Fallback_Encoding=Encoding_CP858;
    else if (Value.size()==6 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='1' && Value[3]=='2' && Value[4]=='5' && Value[5]=='2')
        C.Fallback_Encoding=Encoding_CP1252;
    else if (Value=="8859-1")
        C.Fallback_Encoding=Encoding_8859_1;
    else if (Value=="8859-2")
        C.Fallback_Encoding=Encoding_8859_2;
    else
    {
        std::cout<<Value<<" unknown encoding"<<std::endl;
        return 0;
    }

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Write_Encoding)
{
    std::string Value=Argument.substr(17);
    if (Value.size()==5 && std::tolower(Value[0])=='u' && std::tolower(Value[1])=='t' && std::tolower(Value[2])=='f' && Value[3]=='-' && Value[4]=='8')
        C.Write_Encoding=Encoding_UTF8;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='4' && Value[3]=='3' && Value[4]=='7')
        C.Write_Encoding=Encoding_CP437;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='0')
        C.Write_Encoding=Encoding_CP850;
    else if (Value.size()==5 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='8' && Value[3]=='5' && Value[4]=='8')
        C.Write_Encoding=Encoding_CP858;
    else if (Value.size()==6 && std::tolower(Value[0])=='c' && std::tolower(Value[1])=='p' && Value[2]=='1' && Value[3]=='2' && Value[4]=='5' && Value[5]=='2')
        C.Write_Encoding=Encoding_CP1252;
    else if (Value=="8859-1")
        C.Write_Encoding=Encoding_8859_1;
    else if (Value=="8859-2")
        C.Write_Encoding=Encoding_8859_2;
    else
    {
        std::cout<<Value<<" unknown encoding"<<std::endl;
        return 0;
    }

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Write_CodePage)
{
    C.Write_CodePage=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(Ignore_File_Encoding)
{
    C.Ignore_File_Encoding=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(SpecialChars)
{
    C.SpecialChars_Enabled=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(In_CSET_Remove)
{
    C.In_CSET_Remove=true;

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

//---------------------------------------------------------------------------
CL_OPTION(Out_XML_File)
{
    //Form : --out-xml=(FileName)
    string FileName=string().assign(Argument, 10, std::string::npos);

    if (FileName=="-")
        return CL_Out_XML_cout(C, Argument);
    else
        C.Out_XML_FileName.assign(FileName);

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
    string FileName=string().assign(Argument, 11, std::string::npos);

    if (FileName=="-")
        return CL_Out_Tech_cout(C, Argument);
    else
        C.Out_Tech_CSV_FileName.assign(FileName);

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
    string FileName=string().assign(Argument, 15, std::string::npos);

    if (FileName=="-")
    {
        if (C.Cout!=Core::Cout_None)
        {
            std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
            return 1;
        }
        C.Cout=Core::Cout_Tech_XML;
    }
    else
        C.Out_Tech_XML_FileName.assign(FileName);

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
    //Form : --out-Tech=(FileName)
    string FileName=string().assign(Argument, 11, std::string::npos);

    if (FileName=="-")
        return CL_Out_Core_cout(C, Argument);
    else
        C.Out_Core_CSV_FileName.assign(FileName);

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
    string FileName=string().assign(Argument, 15, std::string::npos);

    if (FileName=="-")
    {
        if (C.Cout!=Core::Cout_None)
        {
            std::cerr<<"Cannot combine "<<Argument<<" with others display output"<<std::endl;
            return 1;
        }
        C.Cout=Core::Cout_Core_XML;
    }
    else
        C.Out_Core_XML_FileName.assign(FileName);

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

    return -3; //Continue, one file mode
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
    string FileName=string().assign(Argument, 10, std::string::npos);

    if (FileName=="-")
        return CL_Out__PMX_cout(C, Argument);
    else
        C.Out__PMX_FileName.assign(FileName);

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

    return -3; //Continue, one file mode
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
    string FileName=string().assign(Argument, 11, std::string::npos);

    if (FileName=="-")
        return CL_Out_aXML_cout(C, Argument);
    else
        C.Out_aXML_FileName.assign(FileName);

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

    return -3; //Continue, one file mode
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
    string FileName=string().assign(Argument, 11, std::string::npos);

    if (FileName=="-")
        return CL_Out_iXML_cout(C, Argument);
    else
        C.Out_iXML_FileName.assign(FileName);

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

    return -3; //Continue, one file mode
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
    string FileName=string().assign(Argument, 10, std::string::npos);

    if (FileName=="-")
        return CL_Out_cue__cout(C, Argument);
    else
        C.Out_cue__FileName.assign(FileName);

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
    C.VerifyMD5=true;
    C.VerifyMD5_Force=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(MD5_Embed)
{
    C.EmbedMD5=true;

    return -2; //Continue
}

//---------------------------------------------------------------------------
CL_OPTION(MD5_Embed_Overwrite)
{
    C.EmbedMD5=true;
    C.EmbedMD5_AuthorizeOverWritting=true;

    return -2; //Continue
}

//***************************************************************************
// Options - Others
//***************************************************************************

//---------------------------------------------------------------------------
CL_OPTION(Chunks_Remove)
{
    ZtringList Fields;
    Fields.Separator_Set(0, __T(","));
    Fields.Write(Ztring().From_Local(string().assign(Argument, 16, std::string::npos)));

    for (auto Field : Fields)
    {
        ZtringList Chunks;
        Chunks.Separator_Set(0, __T("/"));
        Chunks.Write(Field);

        size_t Level=0;
        for (auto Chunk :Chunks)
        {
            if (Chunk.empty()) // handle duplicate slashes
                continue;

            Level++;
            if (Chunk.size()!=4)
            {
                std::cerr<<"Invalid chunk identifier: "<<Chunk.To_UTF8()<<std::endl;
                return 1;
            }
            else if (!Level && (Chunk==__T("data") || Chunk==__T("ds64") || Chunk==__T("fmt ")))
            {
                std::cerr<<"Unable to remove mandatory chunk: "<<Chunk.To_UTF8()<<std::endl;
                return 1;
            }
        }

        C.In_Chunk_Remove(Field.To_UTF8());
    }

    return -2;
}

//***************************************************************************
// Options - Default
//***************************************************************************

CL_OPTION(Default)
{
    //Form : --(Field)=(Value)
    bool Append=false;
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

    if (Egal_Pos && Argument[Egal_Pos-1]=='+')
        Append=true;

    #ifdef _WIN32
    Ztring Field(Ztring().From_UTF8(Argument), 2, Egal_Pos-(Append?3:2)); Field.MakeLowerCase();
    #else
    Ztring Field(Ztring().From_Local(Argument), 2, Egal_Pos-(Append?3:2)); Field.MakeLowerCase();
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
    if (Append)
        C.In_Core_Append(Field.To_UTF8(), Value);
    else
        C.In_Core_Add(Field.To_UTF8(), Value);

    return -2; //Continue
}

