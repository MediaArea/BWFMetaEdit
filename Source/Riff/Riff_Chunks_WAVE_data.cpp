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
#include "ZenLib/Utils.h"
//---------------------------------------------------------------------------

//***************************************************************************
// WAVE data
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Read_Internal ()
{
    //Integrity
    if (Global->data)
        throw exception_valid("2 data chunks");

    //Real size
    if (Global->ds64 && Global->ds64->dataSize!=(int64u)-1)
        Chunk.Content.Size=Global->ds64->dataSize;

    //Integrity
    if (Chunk.File_In_Position+Chunk.Header.Size+Chunk.Content.Size>Global->In.Size_Get())
        throw exception_valid(!Global->TruncatedChunks.str().empty()?"truncated ("+Global->TruncatedChunks.str()+")":"truncated");

    //Filling
    Global->data=new Riff_Base::global::chunk_data;
    Global->data->File_Offset=Global->In.Position_Get();
    Global->data->Size=Chunk.Content.Size;
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Modify_Internal ()
{
    //No modification is possible
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Write_Internal ()
{
    if (Global->Out_Buffer_File_TryModification)
        Global->Out_Buffer_WriteAtEnd=true;
    else
        Riff_Base::Write_Internal();
}

