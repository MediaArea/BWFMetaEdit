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
#include <cstring>
//---------------------------------------------------------------------------

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_MD5_::Read_Internal ()
{
    //Integrity
    if (Global->MD5Stored && !Global->MD5Stored->Strings["md5stored"].empty())
        throw exception_valid("2 MD5 chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    int128u MD5Stored;
    Get_L16   (     MD5Stored);

    Global->MD5Stored=new Riff_Base::global::chunk_strings;
    Global->MD5Stored->Strings["md5stored"]=Ztring().From_Number(MD5Stored, 16).To_UTF8();
    while (Global->MD5Stored->Strings["md5stored"].size()<32)
        Global->MD5Stored->Strings["md5stored"].insert(Global->MD5Stored->Strings["md5stored"].begin(), '0'); //Padding with 0, this must be a 32-byte string    
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_MD5_::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (!Global->EmbedMD5_AuthorizeOverWritting && !(Global->MD5Stored && !Global->MD5Stored->Strings["md5stored"].empty()))
        return; //Should never happen (test in Riff_Handler), but in case of.

    if (!(Global->MD5Stored && !Global->MD5Stored->Strings["md5stored"].empty()))
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=16;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[16];
    memset(Chunk.Content.Buffer, '\0', 16);

    //Parsing
    Put_L16   (     Ztring().From_UTF8(Global->MD5Stored->Strings["md5stored"]).To_int128u());

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_MD5_::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

