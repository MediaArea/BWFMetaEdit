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
void Riff_WAVE_CSET::Read_Internal ()
{
    //Integrity
    if (Global->CSET)
        throw exception_valid("2 CSET chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Parsing
    int16u codePage, countryCode, languageCode, dialectCode;
    Get_L2(codePage);
    Get_L2(countryCode);
    Get_L2(languageCode);
    Get_L2(dialectCode);

    //Filling
    Global->CSET=new Riff_Base::global::chunk_CSET;
    Global->CSET->codePage=codePage;
    Global->CSET->countryCode=countryCode;
    Global->CSET->languageCode=languageCode;
    Global->CSET->dialectCode=dialectCode;
    Global->CSET_Present=true;
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_CSET::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (!Global->CSET)
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    if (Chunk.Content.Size<8)
    {
        Chunk.Content.Size=8;
        delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[8];
    }
    memset(Chunk.Content.Buffer, '\0', 8);

    Put_L2(Global->CSET->codePage);
    Put_L2(Global->CSET->countryCode);
    Put_L2(Global->CSET->languageCode);
    Put_L2(Global->CSET->dialectCode);

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_CSET::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}


