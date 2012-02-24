// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Riff_HandlerH
#define Riff_HandlerH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/File.h"
#include "ZenLib/Ztring.h"
#include <string>
#include <sstream>
#include <bitset>
#include "Riff/Riff_Base.h"
using namespace ZenLib;
using namespace std;
class Riff;
//---------------------------------------------------------------------------

//***************************************************************************
// 
//***************************************************************************

class Riff_Handler
{
public:
    //---------------------------------------------------------------------------
    //Structures
    struct rules
    {
        bool                            Tech3285_Req;
        bool                            Tech3285_Rec;
        bool                            CodingHistory_Rec;
        bool                            OriginatorReference_Rec;
        bool                            INFO_Req;
        bool                            INFO_Rec;
        bool                            FADGI_Rec;

        rules()
        {
            Tech3285_Req=true;
            Tech3285_Rec=false;
            CodingHistory_Rec=false;
            OriginatorReference_Rec=false;
            INFO_Req=true;
            INFO_Rec=false;
            FADGI_Rec=false;
        }
    };

    //---------------------------------------------------------------------------
    //Constructor/Destructor
    Riff_Handler();
    ~Riff_Handler();
    
    //---------------------------------------------------------------------------
    //I/O
    bool            Open            (const string &FileName);
    bool            Save            ();
    bool            BackToLastSave  ();

    //---------------------------------------------------------------------------
    //Per Item
    string          Get                 (const string &Field);
    bool            Set                 (const string &Field, const string &Value, rules Rules);
    bool            Remove              (const string &Field);
    string          History             (const string &Field);
    bool            IsOriginal          (const string &Field, const string &Value);
    bool            IsValid             (const string &Field, const string &Value, rules Rules);
    string          IsValid_LastError   () {return IsValid_Errors.str();}
    bool            IsModified          (const string &Field);
    
    //---------------------------------------------------------------------------
    //Global
    static string   Core_Header();
    string          Core_Get(bool IsBackuping=false); //FromFile=before modifications
    static string   Technical_Header();
    string          Technical_Get();
    
    //---------------------------------------------------------------------------
    //Info
    string          Trace_Get();
    string          FileName_Get();
    string          FileDate_Get();
    float           Progress_Get();
    void            Progress_Clear();
    bool            Canceled_Get();
    void            Cancel();
    bool            IsValid_Get();
    bool            IsModified_Get();

    //---------------------------------------------------------------------------
    //Configuration
    bool            riff2rf64_Reject;
    bool            Overwrite_Reject;
    bool            NoPadding_Accept;
    bool            NewChunksAtTheEnd;
    bool            GenerateMD5;
    bool            VerifyMD5;
    bool            EmbedMD5;
    bool            EmbedMD5_AuthorizeOverWritting;
    unsigned short  Bext_DefaultVersion;
    unsigned short  Bext_MaxVersion;
    void            Options_Update();

    //---------------------------------------------------------------------------
    //Stdxxx
    ostringstream   Errors;
    ostringstream   Information;
    ostringstream   PerFile_Error;
    ostringstream   PerFile_Information;
    ostringstream   IsValid_Errors;

private:
    //---------------------------------------------------------------------------
    //Helpers - Per item
    string          Get             (const string &Field,                       Riff_Base::global::chunk_strings* &Chunk_Strings);
    bool            Set             (const string &Field, const string &Value,  Riff_Base::global::chunk_strings* &Chunk_Strings, int32u Chunk_Name2, int32u Chunk_Name3=0x00000000);
    bool            IsModified      (const string &Field,                       Riff_Base::global::chunk_strings* &Chunk_Strings);
    bool            IsOriginal      (const string &Field, const string &Value,  Riff_Base::global::chunk_strings* &Chunk_Strings);
    string          History         (const string &Field,                       Riff_Base::global::chunk_strings* &Chunk_Strings);
    
    //---------------------------------------------------------------------------
    //Helpers - Retrieval of chunks info
    Riff_Base::global::chunk_strings** chunk_strings_Get    (const string &Field);
    string Field_Get                                        (const string &Field);
    int32u Chunk_Name2_Get                                  (const string &Field);
    int32u Chunk_Name3_Get                                  (const string &Field);

    //---------------------------------------------------------------------------
    //Internal
    ZenLib::File    In;
    ZenLib::File    Out;
    Ztring          Core_FromFile;
    Riff*           Chunks;
    bool            File_IsValid;
    bool            File_IsCanceled;
};

#endif
