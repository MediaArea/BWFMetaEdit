// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Riff_BaseH
#define Riff_BaseH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/ZtringList.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include "ZenLib/CriticalSection.h"
#include <vector>
#include <map>
#include <sstream>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const int64u RIFF_Size_Limit=0xFFFFFFFF; //Limit about when we implement ds64
const int64u RIFF_WAVE_FLLR_DefaultSise=16*1024; //Default size of FLLR at the beginning of a file
const vector<wchar_t> ISO_8859_2=
{
    0x00A0,0x0104,0x02D8,0x0141,0x00A4,0x013D,
    0x015A,0x00A7,0x00A8,0x0160,0x015E,0x0164,
    0x0179,0x00AD,0x017D,0x017B,0x00B0,0x0105,
    0x02DB,0x0142,0x00B4,0x013E,0x015B,0x02C7,
    0x00B8,0x0161,0x015F,0x0165,0x017A,0x02DD,
    0x017E,0x017C,0x0154,0x00C1,0x00C2,0x0102,
    0x00C4,0x0139,0x0106,0x00C7,0x010C,0x00C9,
    0x0118,0x00CB,0x011A,0x00CD,0x00CE,0x010E,
    0x0110,0x0143,0x0147,0x00D3,0x00D4,0x0150,
    0x00D6,0x00D7,0x0158,0x016E,0x00DA,0x0170,
    0x00DC,0x00DD,0x0162,0x00DF,0x0155,0x00E1,
    0x00E2,0x0103,0x00E4,0x013A,0x0107,0x00E7,
    0x010D,0x00E9,0x0119,0x00EB,0x011B,0x00ED,
    0x00EE,0x010F,0x0111,0x0144,0x0148,0x00F3,
    0x00F4,0x0151,0x00F6,0x00F7,0x0159,0x016F,
    0x00FA,0x0171,0x00FC,0x00FD,0x0163,0x02D9,
};
const vector<wchar_t> CP437=
{
    0x00C7,0x00FC,0x00E9,0x00E2,0x00E4,0x00E0,0x00E5,0x00E7,
    0x00EA,0x00EB,0x00E8,0x00EF,0x00EE,0x00EC,0x00C4,0x00C5,
    0x00C9,0x00E6,0x00C6,0x00F4,0x00F6,0x00F2,0x00FB,0x00F9,
    0x00FF,0x00D6,0x00DC,0x00A2,0x00A3,0x00A5,0x20A7,0x0192,
    0x00E1,0x00ED,0x00F3,0x00FA,0x00F1,0x00D1,0x00AA,0x00BA,
    0x00BF,0x2310,0x00AC,0x00BD,0x00BC,0x00A1,0x00AB,0x00BB,
    0x2591,0x2592,0x2593,0x2502,0x2524,0x2561,0x2562,0x2556,
    0x2555,0x2563,0x2551,0x2557,0x255D,0x255C,0x255B,0x2510,
    0x2514,0x2534,0x252C,0x251C,0x2500,0x253C,0x255E,0x255F,
    0x255A,0x2554,0x2569,0x2566,0x2560,0x2550,0x256C,0x2567,
    0x2568,0x2564,0x2565,0x2559,0x2558,0x2552,0x2553,0x256B,
    0x256A,0x2518,0x250C,0x2588,0x2584,0x258C,0x2590,0x2580,
    0x03B1,0x00DF,0x0393,0x03C0,0x03A3,0x03C3,0x00B5,0x03C4,
    0x03A6,0x0398,0x03A9,0x03B4,0x221E,0x03C6,0x03B5,0x2229,
    0x2261,0x00B1,0x2265,0x2264,0x2320,0x2321,0x00F7,0x2248,
    0x00B0,0x2219,0x00B7,0x221A,0x207F,0x00B2,0x25A0,0x00A0,
};
const vector<wchar_t> CP850=
{
    0x00C7,0x00FC,0x00E9,0x00E2,0x00E4,0x00E0,0x00E5,0x00E7,
    0x00EA,0x00EB,0x00E8,0x00EF,0x00EE,0x00EC,0x00C4,0x00C5,
    0x00C9,0x00E6,0x00C6,0x00F4,0x00F6,0x00F2,0x00FB,0x00F9,
    0x00FF,0x00D6,0x00DC,0x00F8,0x00A3,0x00D8,0x00D7,0x0192,
    0x00E1,0x00ED,0x00F3,0x00FA,0x00F1,0x00D1,0x00AA,0x00BA,
    0x00BF,0x00AE,0x00AC,0x00BD,0x00BC,0x00A1,0x00AB,0x00BB,
    0x2591,0x2592,0x2593,0x2502,0x2524,0x00C1,0x00C2,0x00C0,
    0x00A9,0x2563,0x2551,0x2557,0x255D,0x00A2,0x00A5,0x2510,
    0x2514,0x2534,0x252C,0x251C,0x2500,0x253C,0x00E3,0x00C3,
    0x255A,0x2554,0x2569,0x2566,0x2560,0x2550,0x256C,0x00A4,
    0x00F0,0x00D0,0x00CA,0x00CB,0x00C8,0x0131,0x00CD,0x00CE,
    0x00CF,0x2518,0x250C,0x2588,0x2584,0x00A6,0x00CC,0x2580,
    0x00D3,0x00DF,0x00D4,0x00D2,0x00F5,0x00D5,0x00B5,0x00FE,
    0x00DE,0x00DA,0x00DB,0x00D9,0x00FD,0x00DD,0x00AF,0x00B4,
    0x00AD,0x00B1,0x2017,0x00BE,0x00B6,0x00A7,0x00F7,0x00B8,
    0x00B0,0x00A8,0x00B7,0x00B9,0x00B3,0x00B2,0x25A0,0x00A0,
};
const vector<wchar_t> CP1252=
{
    0x20AC,0x0000,0x201A,0x0192,0x201E,0x2026,0x2020,0x2021,
    0x02C6,0x2030,0x0160,0x2039,0x0152,0x0000,0x017D,0x0000,
    0x0000,0x2018,0x2019,0x201C,0x201D,0x2022,0x2013,0x2014,
    0x02DC,0x2122,0x0161,0x203A,0x0153,0x0000,0x017E,0x0178,
};

//---------------------------------------------------------------------------

//***************************************************************************
// Enums
//***************************************************************************

enum Riff_Encoding
{
    Encoding_UTF8=0,
    Encoding_CP437,
    Encoding_CP850,
    Encoding_CP858,
    Encoding_CP1252,
    Encoding_8859_1,
    Encoding_8859_2,
    Encoding_Local,
    Encoding_Max,
};

//***************************************************************************
// Exceptions
//***************************************************************************

class exception_read : public exception
{
    virtual const char* what() const throw()
    {
        return "error during reading";
    }
};

class exception_canceled : public exception
{
    virtual const char* what() const throw()
    {
        return "canceled";
    }
};

class exception_valid : public exception
{
public:
    exception_valid(const string &text_in) throw () : exception() {text="invalid Wave: "+text_in;}
    ~exception_valid() throw () {};
    virtual const char* what() const throw()
    {
        return text.c_str();
    }

protected:
    string text;    
};

class exception_read_chunk : public exception
{
public:
    exception_read_chunk(const string &text_in) throw () : exception() {text=text_in;}
    ~exception_read_chunk() throw () {};
    virtual const char* what() const throw()
    {
        return text.c_str();
    }

protected:
    string text;    
};

class exception_write : public exception
{
public:
    exception_write(const string &text_in) throw () : exception() {text=text_in;}
    exception_write() throw () {text="error during writing";};
    ~exception_write() throw () {};
    virtual const char* what() const throw()
    {
        return text.c_str();
    }

protected:
    string text;    
};


//***************************************************************************
// Riff_Base
//***************************************************************************

class Riff_Base
{
public:
    //***************************************************************************
    // Structures
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Global structure for handling common data
    struct global
    {
        struct chunk_WAVE
        {
            int64u          File_Offset;
            int64u          Size_Original;

            chunk_WAVE()
            {
                File_Offset=(int64u)-1;
                Size_Original=(int64u)-1;
            }
        };
        struct chunk_data
        {
            int64u          File_Offset;
            int64u          Size;

            chunk_data()
            {
                File_Offset=(int64u)-1;
                Size=(int64u)-1;
            }
        };
        struct chunk_ds64
        {
            int64u          riffSize;
            int64u          dataSize;
            int64u          sampleCount;

            chunk_ds64()
            {
                riffSize=(int64u)-1;
                dataSize=(int64u)-1;
                sampleCount=(int64u)-1;
            }
        };
        struct chunk_fmt_
        {
            int16u          formatType;
            int128u         extFormatType;
            int16u          channelCount;
            int32u          sampleRate;
            int32u          bytesPerSecond;
            int16u          blockAlignment;
            int16u          bitsPerSample;
 
            chunk_fmt_()
            {
                formatType=0;
                extFormatType=0;
                channelCount=0;
                sampleRate=0;
                bytesPerSecond=0;
                blockAlignment=0;
                bitsPerSample=0;
            }
        };
        struct chunk_cue_
        {

            struct point
            {
                int32u          id;
                int32u          position;
                int32u          dataChunkId;
                int32u          chunkStart;
                int32u          blockStart;
                int32u          sampleOffset;

                point()
                {
                    id=0;
                    position=0;
                    dataChunkId=0;
                    chunkStart=0;
                    blockStart=0;
                    sampleOffset=0;
                }
            };

            std::vector<point> points;
        };
        struct chunk_labl
        {
            int32u          cuePointId;
            string          label;

            chunk_labl()
            {
                cuePointId=0;
            }
        };
        struct chunk_note
        {
            int32u          cuePointId;
            string          note;

            chunk_note()
            {
                cuePointId=0;
            }
        };
        struct chunk_ltxt
        {
            int32u          cuePointId;
            int32u          sampleLength;
            int32u          purposeId;
            int16u          country;
            int16u          language;
            int16u          dialect;
            int16u          codePage;
            string          text;

            chunk_ltxt()
            {
                cuePointId=0;
                sampleLength=0;
                purposeId=0;
                country=0;
                language=0;
                dialect=0;
                codePage=0;
            }
        };
        struct chunk_adtl
        {
            std::vector<chunk_labl> labels;
            size_t                  labelsIndex;
            std::vector<chunk_note> notes;
            size_t                  notesIndex;
            std::vector<chunk_ltxt> texts;
            size_t                  textsIndex;

            chunk_adtl()
            {
                labelsIndex=0;
                notesIndex=0;
                textsIndex=0;
            }
        };

        struct chunk_CSET
        {
            int16u          codePage;
            int16u          countryCode;
            int16u          languageCode;
            int16u          dialectCode;

            chunk_CSET()
            {
                codePage=0;
                countryCode=0;
                languageCode=0;
                dialectCode=0;
            }
        };
        struct chunk_strings
        {
            map<string, string> Strings;
            map<string, ZtringList> Histories;
        };
        struct buffer
        {
            int8u*  Data;
            size_t  Size;
            size_t  Size_Maximum;

            buffer()
            {
                Data=NULL;
                Size=0;
                Size_Maximum=0;
            }
        };

        File                In;
        File                Out;
        buffer              Out_Buffer_Begin;
        buffer              Out_Buffer_End;
        bool                Out_Buffer_WriteAtEnd;

        #ifdef MACSTORE
        Ztring              Temp_Path;
        Ztring              Temp_Name;
        #endif
        Ztring              File_Name;
        int64u              File_Size;
        string              File_Date_Created;
        string              File_Date_Modified;
        string              UnsupportedChunks;
        ostringstream       TruncatedChunks;
        ostringstream       Trace;
        chunk_WAVE         *WAVE;
        chunk_ds64         *ds64;
        chunk_fmt_         *fmt_;
        chunk_data         *data;
        chunk_CSET         *CSET;
        chunk_strings      *bext;
        chunk_strings      *INFO;
        chunk_strings      * XMP;
        chunk_strings      *aXML;
        chunk_strings      *iXML;
        chunk_strings      *cuexml;
        chunk_strings      *MD5Stored;
        chunk_strings      *MD5Generated;
        chunk_cue_         *cue_;
        chunk_adtl         *adtl;
        bool                CSET_Present;
        bool                NoPadding_Accept;
        bool                NoPadding_IsCorrected;
        bool                RF64DataSize_IsCorrected;
        bool                NewChunksAtTheEnd;
        bool                GenerateMD5;
        bool                VerifyMD5;
        bool                VerifyMD5_Force;
        bool                EmbedMD5;
        bool                EmbedMD5_AuthorizeOverWritting;
        bool                Out_Buffer_File_TryModification;
        bool                Out_Buffer_File_IsModified;
        bool                IsRF64;
        bool                Trace_UseDec;
        bool                Read_Only;

        CriticalSection     CS;
        float               Progress;
        bool                Canceling;

        global()
        {
            File_Size=0;
            WAVE=NULL;
            ds64=NULL;
            fmt_=NULL;
            data=NULL;
            CSET=NULL;
            bext=NULL;
            INFO=NULL;
             XMP=NULL;
            aXML=NULL;
            iXML=NULL;
            cue_=NULL;
            adtl=NULL;
            cuexml=NULL;
            MD5Stored=NULL;
            MD5Generated=NULL;
            CSET_Present=false;
            NoPadding_Accept=false;
            NoPadding_IsCorrected=false;
            RF64DataSize_IsCorrected=false;
            NewChunksAtTheEnd=false;
            GenerateMD5=false;
            VerifyMD5=false;
            VerifyMD5_Force=false;
            EmbedMD5=false;
            EmbedMD5_AuthorizeOverWritting=false;
            Out_Buffer_WriteAtEnd=false;
            Out_Buffer_File_TryModification=true;
            Out_Buffer_File_IsModified=false;
            IsRF64=false;
            Trace_UseDec=false;
            Read_Only=false;
            Progress=0;
            Canceling=false;
        }

        ~global()
        {
            delete ds64; //ds64=NULL;
            delete fmt_; //fmt_=NULL;
            delete bext; //bext=NULL;
            delete CSET; //CSET=NULL;
            delete INFO; //INFO=NULL;
            delete  XMP; // XMP=NULL;
            delete aXML; //aXML=NULL;
            delete iXML; //iXML=NULL;
            delete cue_; //cue_=NULL;
            delete adtl; //adtl=NULL;
            delete cuexml; //cuexml=NULL;
        }
    };

    //---------------------------------------------------------------------------
    //Chunk specific
    struct chunk
    {
        struct header
        {
            size_t  Level;
            int8u   Size;
            int32u  List; //If header is 12 byte long
            int32u  Name;
            
            header()
            {
                Level=0;
                Size=0;
                List=0x00000000;
                Name=0x00000000;
            }
        };
        struct content
        {
            int8u*  Buffer;
            size_t  Buffer_Offset; //Internal use
            int64u  Size; //Header excluded
            bool    IsModified;
            bool    IsRemovable;
            bool    Size_IsModified;
            
            content()
            {
                Buffer=NULL;
                Buffer_Offset=0;
                Size=0;
                IsModified=false;
                IsRemovable=false;
                Size_IsModified=false;
            }

            ~content()
            {
                delete[] Buffer; //Buffer=NULL;
            }
        };

        int64u  File_In_Position;
        header  Header;
        content Content;

        chunk()
        {
            File_In_Position=(int64u)-1;
        }
    };

    //***************************************************************************
    // Interface
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Constructor/Destructor
             Riff_Base          (global* Global, size_t Level);
    virtual ~Riff_Base          ();
    
    //---------------------------------------------------------------------------
    //Read/Write
    void Read                   (chunk &Chunk_In);
    void Modify                 (int32u Chunk_Name_1, int32u Chunk_Name_2, int32u Chunk_Name_3);
    void Modify                 ()                                              {Modify_Internal();};
    void Write                  ();

    //---------------------------------------------------------------------------
    //Data
    int64u Block_Size_Get       ();
    int64u Block_Size_Get       (int32u Element);
    size_t Subs_Pos_Get         (int32u Element);
    bool   Read_Header          (chunk &NewChunk);
    bool   IsModified           ()                                              {return Chunk.Content.IsModified;};
    void   IsModified_Clear     ()                                              {Chunk.Content.IsModified=false;};
    bool   IsRemovable          ()                                              {return Chunk.Content.IsRemovable;};
    int32u Header_Name_Get      ()                                              {return Chunk.Header.Name;};
    void   Header_Name_Set      (int32u Name)                                   {Chunk.Header.Name=Name;};

protected :
    //***************************************************************************
    // Buffer handling (virtual)
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Read/Write
    virtual void    Read_Internal       ();
    virtual void    Modify_Internal     ()                                      {};
    virtual size_t  Insert_Internal     (int32u)                                {return Subs.size();}
    virtual void    Write_Internal      ()                                      ;
    void            Write_Internal      (const int8u* Buffer, size_t Buffer_Size);

    //***************************************************************************
    // Buffer handling (buffer read/write)
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Buffer handling - Big endian values
    void Get_B1     (int8u  &Value);
    void Put_B1     (int8u   Value);
    void Get_B2     (int16u &Value);
    void Put_B2     (int16u  Value);
    void Get_B3     (int32u &Value);
    void Put_B3     (int32u  Value);
    void Get_B4     (int32u &Value);
    void Put_B4     (int32u  Value);
    void Get_B8     (int64u &Value);
    void Put_B8     (int64u  Value);
    void Get_B16    (int128u &Value);
    void Put_B16    (int128u  Value);

    //---------------------------------------------------------------------------
    //Buffer handling - Little endian values
    void Get_L1     (int8u  &Value);
    void Put_L1     (int8u   Value);
    void Get_L2     (int16u &Value);
    void Put_L2     (int16u  Value);
    void Get_L3     (int32u &Value);
    void Put_L3     (int32u  Value);
    void Get_L4     (int32u &Value);
    void Put_L4     (int32u  Value);
    void Get_L8     (int64u &Value);
    void Put_L8     (int64u  Value);
    void Get_L16    (int128u &Value);
    void Put_L16    (int128u  Value);

    //---------------------------------------------------------------------------
    //Buffer handling - Character codes
    void Get_C4     (int32u &Value);
    void Put_C4     (int32u  Value);

    //---------------------------------------------------------------------------
    //Buffer handling - UUID
    void Get_UUID   (int128u &Value);
    void Put_UUID   (int128u  Value);
    void Skip_UUID  ();

    //---------------------------------------------------------------------------
    //Buffer handling - Stirngs
    void Get_String (int64u Value_Size, string &Value);
    void Put_String (int64u Value_Size, string Value);

    //---------------------------------------------------------------------------
    //Buffer handling - Other
    void Skip_XX    (int64u Value_Size);

    //***************************************************************************
    // Buffer handling (internal)
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Read/Write helpers
    void Read_Internal_ReadAllInBuffer  ();
    void Modify_Internal_Subs           (int32u Chunk_Name_0, int32u Chunk_Name_1, int32u Chunk_Name_2);
    void Write_Internal_Subs            ();

    //***************************************************************************
    // Data
    //***************************************************************************

    //---------------------------------------------------------------------------
    //Subs
    std::vector<Riff_Base*> Subs;

    //---------------------------------------------------------------------------
    //Global
    global *Global;

    //---------------------------------------------------------------------------
    //Chunk
    chunk Chunk;

    //---------------------------------------------------------------------------
    //Friends
    friend class Riff_Handler;
};

//***************************************************************************
// Defines
//***************************************************************************

#define SUBS_BEGIN() \
    while (Global->In.Position_Get()<Chunk.File_In_Position+Chunk.Header.Size+Chunk.Content.Size) \
    { \
        chunk NewChunk; \
        NewChunk.Header.Level=Chunk.Header.Level+1; \
        if (!Read_Header(NewChunk)) \
            return; \
        \
        Riff_Base* Sub=NULL; \
        switch (NewChunk.Header.Name) \
        { \

#define SUB_ELEMENT(_Name) \
            case Elements::_Name : Sub=new Riff_##_Name(Global); break; \

#define SUB_ELEMENT_DEFAULT(_Name) \
            default : Sub=new Riff_##_Name(Global); break; \

#define SUBS_END() \
            default             : Sub=new Riff_Base(Global, Chunk.Header.Level+1); \
        SUBS_END_DEFAULT();

#define SUBS_END_DEFAULT() \
        } \
         \
        if (Sub) \
        { \
            Sub->Read(NewChunk); \
            Subs.push_back(Sub); \
            if (NewChunk.Content.IsModified) \
                Chunk.Content.IsModified=true; \
            if (NewChunk.Content.Size_IsModified) \
                Chunk.Content.Size_IsModified=true; \
            if (NewChunk.Header.Name==Elements::WAVE_ds64) \
                Global->WAVE->Size_Original=Chunk.Content.Size=Global->ds64->riffSize-4; \
        } \
    } \

#endif
