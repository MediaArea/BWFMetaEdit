// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
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

//---------------------------------------------------------------------------
enum adapteol
{
    adapt_platform,
    adapt_n,
    adapt_rn,
};
void AdaptEOL(string& Value, string& Value2, adapteol Adapt);
void AdaptEOL(string& Value, adapteol Adapt);

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
        bool                            CodingHistory_Rec_Ex_Comma;
        bool                            CodingHistory_Rec_Ex_Analog;
        bool                            CodingHistory_Rec_Ex_Frequency;
        bool                            CodingHistory_Rec_Ex_WordLength;
        bool                            OriginatorReference_Rec;
        bool                            INFO_Req;
        bool                            INFO_Rec;
        bool                            FADGI_Rec;
        bool                            EBU_ISRC_Rec;

        rules()
        {
            Tech3285_Req=true;
            Tech3285_Rec=false;
            CodingHistory_Rec=false;
            CodingHistory_Rec_Ex_Comma=false;
            CodingHistory_Rec_Ex_Analog=false;
            CodingHistory_Rec_Ex_Frequency=false;
            CodingHistory_Rec_Ex_WordLength=false;
            OriginatorReference_Rec=false;
            INFO_Req=true;
            INFO_Rec=false;
            FADGI_Rec=false;
            EBU_ISRC_Rec=false;
        }
    };

    //---------------------------------------------------------------------------
    //Constructor/Destructor
    Riff_Handler();
    ~Riff_Handler();

    //---------------------------------------------------------------------------
    //Encoding
    std::string     Encode          (const std::string& Str);
    std::string     Decode          (const std::string& Str);

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
    bool            Remove_Chunk        (const string &Field);
    string          History             (const string &Field);
    bool            IsOriginal          (const string &Field, const string &Value);
    bool            IsValid             (const string &Field, const string &Value, rules Rules, bool IgnoreCoherency=false);
    string          IsValid_LastError   () {CriticalSectionLocker(this->CS); return IsValid_Errors.str();}
    string          IsValid_LastWarning () {CriticalSectionLocker(this->CS); return IsValid_Warnings.str();}
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
    bool            IsReadOnly_Get();


    //---------------------------------------------------------------------------
    //Configuration
    bool            riff2rf64_Reject;
    bool            Overwrite_Reject;
    bool            NoPadding_Accept;
    bool            NewChunksAtTheEnd;
    bool            GenerateMD5;
    bool            VerifyMD5;
    bool            VerifyMD5_Force;
    bool            EmbedMD5;
    bool            EmbedMD5_AuthorizeOverWritting;
    bool            Trace_UseDec;
    Riff_Encoding   Encoding;
    Riff_Encoding   Fallback_Encoding;
    Riff_Encoding   Write_Encoding;
    bool            Write_CodePage;
    bool            Ignore_File_Encoding;
    unsigned short  Bext_DefaultVersion;
    unsigned short  Bext_MaxVersion;
    void            Options_Update();

    //---------------------------------------------------------------------------
    //Stdxxx
    ostringstream   Errors;
    ostringstream   Warnings;
    ostringstream   Information;
    ostringstream   PerFile_Error;
    ostringstream   PerFile_Warning;
    ostringstream   PerFile_Information;
    ostringstream   IsValid_Errors;
    ostringstream   IsValid_Warnings;

private:
    //---------------------------------------------------------------------------
    //Helpers - Internal
    bool      Open_Internal              (const string &FileName);
    string    Get_Internal               (const string &Field);
    bool      Set_Internal               (const string &Field, const string &Value, rules Rules);
    bool      Remove_Internal            (const string &Field);
    bool      Remove_Chunk_Helper        (Riff_Base *Parent, const string &Path, vector<int32u>& ParentsCC4);
    bool      Remove_Chunk_Internal      (const string &Field);
    bool      IsValid_Internal           (const string &Field, const string &Value, rules Rules, bool IgnoreCoherency=false);
    bool      IsOriginal_Internal        (const string &Field, const string &Value);
    bool      IsModified_Internal        (const string &Field);
    string    Core_Get_Internal          (bool IsBackuping=false);
    bool      IsModified_Get_Internal    ();
    bool      IsReadOnly_Get_Internal    ();
    void      Options_Update_Internal    (bool Update=true);
    string    Cue_Xml_Get                ();
    bool      Cue_Xml_Set                (const string& Xml, rules Rules);
    bool      Cue_Xml_To_Fields          (const string& Xml, std::vector<Riff_Base::global::chunk_cue_::point>& Points,
                                                             std::vector<Riff_Base::global::chunk_labl>& Labels,
                                                             std::vector<Riff_Base::global::chunk_note>& Notes,
                                                             std::vector<Riff_Base::global::chunk_ltxt>& Texts);

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
    CriticalSection CS;
};

#endif
