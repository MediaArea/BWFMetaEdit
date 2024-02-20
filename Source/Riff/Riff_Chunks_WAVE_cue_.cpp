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

//***************************************************************************
// Const
//***************************************************************************

//---------------------------------------------------------------------------
const size_t cue__Point_Size=24;

//---------------------------------------------------------------------------

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_cue_::Read_Internal ()
{
    //Integrity
    if (Global->cue_)
        throw exception_valid("2 cue chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Integrity
    if (Chunk.Content.Size<4)
        throw exception_valid("wrong chunk size (cue)");

    //Parsing
    int32u Count=0;
    Get_L4(Count);

    //Integrity
    if (cue__Point_Size*Count > Chunk.Content.Size)
        throw exception_valid("wrong cue points count");

    Global->cue_=new Riff_Base::global::chunk_cue_();
    for (size_t Pos=0; Pos<Count; Pos++)
    {
        Riff_Base::global::chunk_cue_::point Item;
        Get_L4(Item.id);
        Get_L4(Item.position);
        Get_B4(Item.dataChunkId);
        Get_L4(Item.chunkStart);
        Get_L4(Item.blockStart);
        Get_L4(Item.sampleOffset);

        //Filling
        Global->cue_->points.push_back(Item);
    }
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_cue_::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (Global->cue_==NULL || Global->cue_->points.empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=4+cue__Point_Size*Global->cue_->points.size();
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(Global->cue_->points.size());
    for (size_t Pos=0; Pos<Global->cue_->points.size(); Pos++)
    {
        Put_L4(Global->cue_->points[Pos].id);
        Put_L4(Global->cue_->points[Pos].position);
        Put_B4(Global->cue_->points[Pos].dataChunkId);
        Put_L4(Global->cue_->points[Pos].chunkStart);
        Put_L4(Global->cue_->points[Pos].blockStart);
        Put_L4(Global->cue_->points[Pos].sampleOffset);
    }

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_cue_::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

