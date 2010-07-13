// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Chunks.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Const
//***************************************************************************

const size_t INFO_Strings_Size=17;
const char* INFO_Strings[17]=
{
    //Note: there is a duplicate in Riff_Chunks_INFO_xxxx
    "IARL", //Archival Location
    "IART", //Artist
    "ICMS", //Commissioned
    "ICMT", //Comment
    "ICOP", //Copyright
    "ICRD", //Date Created
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
    if (Global->INFO->Strings.find(Ztring().From_CC4(Chunk.Header.Name))!=Global->INFO->Strings.end())
        throw exception_valid("2 "+Ztring().From_CC4(Chunk.Header.Name)+" chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    Ztring Value;    
    Get_String(Chunk.Content.Size, Value);
        
    //Filling
    Ztring Field=Ztring().From_CC4(Chunk.Header.Name);
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
    string Field=Ztring().From_CC4(Chunk.Header.Name).MakeUpperCase();
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
    Chunk.Content.Size=Global->INFO->Strings[Field].size();
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Global->INFO->Strings[Field].size()];

    Put_String(Global->INFO->Strings[Field].size(), Global->INFO->Strings[Field]);

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

