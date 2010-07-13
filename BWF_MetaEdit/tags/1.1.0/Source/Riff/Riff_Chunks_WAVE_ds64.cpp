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
#include <cstring>
//---------------------------------------------------------------------------

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_ds64::Read_Internal ()
{
    //Integrity
    if (Global->aXML)
        throw exception_valid("2 ds64 chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    int64u riffSize, dataSize, sampleCount;
    int32u tableLength;
    Get_L8(riffSize);
    Get_L8(dataSize);
    Get_L8(sampleCount);
    Get_L4(tableLength);
    
    //Test
    if (riffSize<4)
        throw exception_valid("riffSize");
    if (tableLength)
        throw exception_valid("extended ds64 is not supported");
    
    //Filling
    Global->ds64=new Riff_Base::global::chunk_ds64;
    Global->ds64->riffSize=riffSize;
    Global->ds64->dataSize=dataSize;
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_ds64::Modify_Internal ()
{
    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=28;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[28];
    memset(Chunk.Content.Buffer, '\0', 28);
    Chunk.Content.Size=28;

    Put_L8(Global->ds64->riffSize);
    Put_L8(Global->ds64->dataSize);
    Put_L8(0);
    Put_L4(0); //tableLength

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_ds64::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}


