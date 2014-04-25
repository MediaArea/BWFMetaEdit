// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
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
//---------------------------------------------------------------------------

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
            int16u          channelCount;
            int32u          sampleRate;
            int32u          bytesPerSecond;
            int16u          blockAlignment;
            int16u          bitsPerSample;
 
            chunk_fmt_()
            {
                formatType=0;
                channelCount=0;
                sampleRate=0;
                bytesPerSecond=0;
                blockAlignment=0;
                bitsPerSample=0;
            }
        };
        struct chunk_strings
        {
            map<string, Ztring> Strings;
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

        string              File_Name;
        int64u              File_Size;
        string              File_Date;
        string              UnsupportedChunks;
        ostringstream       Trace;
        chunk_WAVE         *WAVE;
        chunk_ds64         *ds64;
        chunk_fmt_         *fmt_;
        chunk_data         *data;
        chunk_strings      *bext;
        chunk_strings      *INFO;
        chunk_strings      * XMP;
        chunk_strings      *aXML;
        chunk_strings      *iXML;
        chunk_strings      *MD5Stored;
        chunk_strings      *MD5Generated;
        bool                NoPadding_Accept;
        bool                NoPadding_IsCorrected;
        bool                NewChunksAtTheEnd;
        bool                GenerateMD5;
        bool                VerifyMD5;
        bool                EmbedMD5;
        bool                EmbedMD5_AuthorizeOverWritting;
        bool                Out_Buffer_File_TryModification;
        bool                Out_Buffer_File_IsModified;
        bool                IsRF64;

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
            bext=NULL;
            INFO=NULL;
             XMP=NULL;
            aXML=NULL;
            iXML=NULL;
            MD5Stored=NULL;
            MD5Generated=NULL;
            NoPadding_Accept=false;
            NoPadding_IsCorrected=false;
            NewChunksAtTheEnd=false;
            GenerateMD5=false;
            VerifyMD5=false;
            EmbedMD5=false;
            EmbedMD5_AuthorizeOverWritting=false;
            Out_Buffer_WriteAtEnd=false;
            Out_Buffer_File_TryModification=true;
            Out_Buffer_File_IsModified=false;
            IsRF64=false;
            Progress=0;
            Canceling=false;
        }

        ~global()
        {
            delete ds64; //ds64=NULL;
            delete fmt_; //fmt_=NULL;
            delete bext; //bext=NULL;
            delete INFO; //INFO=NULL;
            delete  XMP; // XMP=NULL;
            delete aXML; //aXML=NULL;
            delete iXML; //iXML=NULL;
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
    virtual void    Modify_Internal     ()                                      {}
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
