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
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl::Read_Internal ()
{
    //Integrity
    if (Global->adtl)
        throw exception_valid("2 adtl chunks");

    //Filling
    Global->adtl=new Riff_Base::global::chunk_adtl;

    //Subs
    SUBS_BEGIN();
        SUB_ELEMENT(WAVE_adtl_labl);
        SUB_ELEMENT(WAVE_adtl_note);
        SUB_ELEMENT(WAVE_adtl_ltxt);
    SUBS_END();
}

//***************************************************************************
// Insert
//***************************************************************************

//---------------------------------------------------------------------------
size_t Riff_WAVE_adtl::Insert_Internal (int32u Chunk_Name_Insert)
{

    Riff_Base* NewChunk=NULL;
    if (Chunk_Name_Insert==Elements::WAVE_adtl_labl)
        NewChunk=new Riff_WAVE_adtl_labl(Global);
    else if (Chunk_Name_Insert==Elements::WAVE_adtl_note)
        NewChunk=new Riff_WAVE_adtl_note(Global);
    else if (Chunk_Name_Insert==Elements::WAVE_adtl_ltxt)
        NewChunk=new Riff_WAVE_adtl_ltxt(Global);
    else
        return Subs.size(); //Wrong sub

    NewChunk->Header_Name_Set(Chunk_Name_Insert);
    NewChunk->Modify();
    if (NewChunk->IsRemovable())
    {
        delete NewChunk; //NewChunk=NULL;
        return Subs.size(); //No data to add
    }

    Subs.push_back(NewChunk); //At the end
    return Subs.size()-1; //Indicationg the position
}

