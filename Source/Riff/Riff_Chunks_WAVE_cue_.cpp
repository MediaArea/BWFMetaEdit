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
    ZtringListList Cues;
    int32u Count=0;
    Get_L4(Count);

    //Integrity
    if (cue__Point_Size*Count > Chunk.Content.Size)
        throw exception_valid("wrong cue points count");

    for (size_t Pos=0; Pos<Count; Pos++)
    {
        int32u Id, Position, DataChunkId, ChunkStart, BlockStart, SampleOffset;
        ZtringList Cue;
        Get_L4(Id);
        Cue.push_back(Ztring().From_Number(Id));
        Get_L4(Position);
        Cue.push_back(Ztring().From_Number(Position));
        Get_B4(DataChunkId);
        Cue.push_back(__T("0x")+Ztring().From_Number(DataChunkId, 16));
        Get_L4(ChunkStart);
        Cue.push_back(Ztring().From_Number(ChunkStart));
        Get_L4(BlockStart);
        Cue.push_back(Ztring().From_Number(BlockStart));
        Get_L4(SampleOffset);
        Cue.push_back(Ztring().From_Number(SampleOffset));
        Cues.push_back(Cue);
    }

    //Filling
    Global->cue_=new Riff_Base::global::chunk_strings;
    Global->cue_->Strings["cue"]=Cues.Read().To_UTF8();
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_cue_::Modify_Internal ()
{
    if (Global->cue_==NULL || Global->cue_->Strings["cue"].empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    ZtringListList Cues(Ztring().From_UTF8(Global->cue_->Strings["cue"]));
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=4+cue__Point_Size*Cues.size();
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(Cues.size());
    for (size_t Pos=0; Pos<Cues.size(); Pos++)
    {
        Put_L4(Cues[Pos](0).To_int32u());
        Put_L4(Cues[Pos](1).To_int32u());
        Put_B4(Cues[Pos](2).To_int32u(16));
        Put_L4(Cues[Pos](3).To_int32u());
        Put_L4(Cues[Pos](4).To_int32u());
        Put_L4(Cues[Pos](5).To_int32u());
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

