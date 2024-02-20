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
void Riff_WAVE_adtl_ltxt::Read_Internal ()
{

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Integrity
    if (Chunk.Content.Size<20)
        throw exception_valid("wrong chunk size (ltxt)");

    //Parsing
    Riff_Base::global::chunk_ltxt Item;
    Get_L4(Item.cuePointId);
    Get_L4(Item.sampleLength);
    Get_B4(Item.purposeId);
    Get_L2(Item.country);
    Get_L2(Item.language);
    Get_L2(Item.dialect);
    Get_L2(Item.codePage);
    if (Chunk.Content.Size>20)
        Get_String(Chunk.Content.Size-20, Item.text);

    //Filling
    Global->adtl->texts.push_back(Item);
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_ltxt::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (!Global->adtl || Global->adtl->texts.empty() || Global->adtl->textsIndex>=Global->adtl->texts.size())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    Riff_Base::global::chunk_ltxt Item=Global->adtl->texts[Global->adtl->textsIndex];

    //Calculating size
    if (Item.text.size()>=0xFFFFFFEA)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=20+Item.text.size()+1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(Item.cuePointId);
    Put_L4(Item.sampleLength);
    Put_B4(Item.purposeId);
    Put_L2(Item.country);
    Put_L2(Item.language);
    Put_L2(Item.dialect);
    Put_L2(Item.codePage);
    Put_String(Item.text.size(), Item.text);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;

    Global->adtl->textsIndex++;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_ltxt::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

