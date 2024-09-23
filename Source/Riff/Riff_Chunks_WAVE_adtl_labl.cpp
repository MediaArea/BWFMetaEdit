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
#include "ZenLib/ZtringList.h"
#include "ZenLib/ZtringListList.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_labl::Read_Internal ()
{

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Integrity
    if (Chunk.Content.Size<4)
        throw exception_valid("wrong chunk size (labl)");

    //Parsing
    Riff_Base::global::chunk_labl Item;
    Get_L4(Item.cuePointId);
    if (Chunk.Content.Size>4)
        Get_String(Chunk.Content.Size-4, Item.label);

    //Filling
    Global->adtl->labels.push_back(Item);
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_labl::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (!Global->adtl || Global->adtl->labels.empty() || Global->adtl->labelsIndex>=Global->adtl->labels.size())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    Riff_Base::global::chunk_labl Item=Global->adtl->labels[Global->adtl->labelsIndex];

    //Calculating size
    if (Item.label.size()>=0xFFFFFFFA)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=4+Item.label.size()+1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(Item.cuePointId);
    Put_String(Item.label.size(), Item.label);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;

    Global->adtl->labelsIndex++;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_labl::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

