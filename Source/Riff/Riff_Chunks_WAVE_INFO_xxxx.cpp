// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Chunks.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Const
//***************************************************************************

const size_t INFO_Strings_Size=18;
const char* INFO_Strings[18]=
{
    //Note: there is a duplicate in Riff_Chunks_INFO_xxxx
    "IARL", //Archival Location
    "IART", //Artist
    "ICMS", //Commissioned
    "ICMT", //Comment
    "ICOP", //Copyright
    "ICRD", //Date Created
    "IDIT", //Digitization Date
    "IENG", //Engineer
    "IGNR", //Genre
    "IKEY", //Keywords
    "IMED", //Medium
    "INAM", //Title
    "IPRD", //Product
    "ISBJ", //Subject
    "ISFT", //Software
    "ISRC", //Source
    "ISRF", //Source Form
    "ITCH", //Technician
};

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_INFO_xxxx::Read_Internal ()
{
    //Junk
    switch (Chunk.Header.Name)
    {
        case Elements::WAVE_filr :
        case Elements::WAVE_FILR :
        case Elements::WAVE_fllr :
        case Elements::WAVE_FLLR :
        case Elements::WAVE_junk :
        case Elements::WAVE_JUNK :
        case Elements::WAVE_junq :
        case Elements::WAVE_JUNQ :
        case Elements::WAVE_pad_ :
        case Elements::WAVE_PAD_ :
                                    return;
        default                  :  break;
    }

    //Integrity
    if (Global->INFO->Strings.find(Ztring().From_CC4(Chunk.Header.Name).To_UTF8())!=Global->INFO->Strings.end())
        throw exception_valid("2 "+Ztring().From_CC4(Chunk.Header.Name).To_UTF8()+" chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    string Value;
    Get_String(Chunk.Content.Size, Value);

    //Filling
    string Field=Ztring().From_CC4(Chunk.Header.Name).To_UTF8();

    if (Field=="IDIT" && Value.size()==25 && Value[24]=='\n')
        Value.resize(24);

    Global->INFO->Strings[Field]=Value;

    //Details
    bool Unsupported=true;
    for (size_t Pos=0; Pos<INFO_Strings_Size; Pos++)
         if (INFO_Strings[Pos]==Field)
             Unsupported=false;
    if (Unsupported)
    {
        if (!Global->UnsupportedChunks.empty())
            Global->UnsupportedChunks+=" ";
        Global->UnsupportedChunks+=Field;
    }
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_INFO_xxxx::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    string Field=Ztring().From_CC4(Chunk.Header.Name).MakeUpperCase().To_UTF8();
    if (Global->INFO->Strings[Field].empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    //Calculating size
    if (Global->INFO->Strings[Field].size()>=0xFFFFFFFF)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=Global->INFO->Strings[Field].size()+1;
    if (Field=="IDIT" && Global->INFO->Strings[Field].size()==24)
        Chunk.Content.Size+=1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    string Temp=Global->INFO->Strings[Field];
    if (Field=="IDIT" && Global->INFO->Strings[Field].size()==24)
        Temp+="\n";
    Put_String(Temp.size(), Temp);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_INFO_xxxx::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

