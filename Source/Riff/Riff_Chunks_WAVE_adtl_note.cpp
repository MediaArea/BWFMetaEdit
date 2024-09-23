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
void Riff_WAVE_adtl_note::Read_Internal ()
{
    //Reading
    Read_Internal_ReadAllInBuffer();

    //Integrity
    if (Chunk.Content.Size<4)
        throw exception_valid("wrong chunk size (note)");

    //Parsing
    Riff_Base::global::chunk_note Item;
    Get_L4(Item.cuePointId);
    if (Chunk.Content.Size>4)
        Get_String(Chunk.Content.Size-4, Item.note);

    //Filling
    Global->adtl->notes.push_back(Item);
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_note::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (!Global->adtl || Global->adtl->notes.empty() || Global->adtl->notesIndex>=Global->adtl->notes.size())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    Riff_Base::global::chunk_note Item=Global->adtl->notes[Global->adtl->notesIndex];

    //Calculating size
    if (Item.note.size()>=0xFFFFFFFA)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=4+Item.note.size()+1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(Item.cuePointId);
    Put_String(Item.note.size(), Item.note);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;

    Global->adtl->notesIndex++;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_note::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

