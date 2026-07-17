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
// C2PA Chunk
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_C2PA::Read_Internal ()
{
    //Integrity
    if (Global->C2PA)
        throw exception_valid("2 C2PA chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Filling
    Global->C2PA=new Riff_Base::global::chunk_C2PA;
    Global->C2PA->present=true;
}