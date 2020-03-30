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
void Riff_WAVE_INFO::Read_Internal ()
{
    //Integrity
    if (Global->INFO)
        throw exception_valid("2 INFO chunks");

    //Filling
    Global->INFO=new Riff_Base::global::chunk_strings;

    //Subs
    SUBS_BEGIN();
        SUB_ELEMENT_DEFAULT(WAVE_INFO_xxxx);
    SUBS_END_DEFAULT();
}

//***************************************************************************
// Insert
//***************************************************************************

//---------------------------------------------------------------------------
size_t Riff_WAVE_INFO::Insert_Internal (int32u Chunk_Name_Insert)
{
    if (Global->INFO->Strings[Ztring().From_CC4(Chunk_Name_Insert).MakeUpperCase().To_UTF8()].empty())
        return Subs.size(); //No data to add    
    Riff_Base* NewChunk=new Riff_WAVE_INFO_xxxx(Global);
    NewChunk->Header_Name_Set(Chunk_Name_Insert);
    NewChunk->Modify();
    if (!NewChunk->IsRemovable())
    {
        Subs.push_back(NewChunk); //At the end
        return Subs.size()-1; //Indicationg the position
    }
    else
    {
        delete NewChunk; //NewChunk=NULL;
        return Subs.size(); //No data to add    
    }
}

