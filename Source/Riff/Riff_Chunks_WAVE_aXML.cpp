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
// WAVE aXML
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_axml::Read_Internal ()
{
    //Integrity
    if (Global->aXML)
        throw exception_valid("2 aXML chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Filling
    Global->aXML=new Riff_Base::global::chunk_strings;
    string Temp;
    Get_String(Chunk.Content.Size, Temp);
    Global->aXML->Strings["axml"]=Temp;
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_axml::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (Global->aXML==NULL
     || Global->aXML->Strings["axml"].empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    //Calculating size
    string Value=Global->aXML->Strings["axml"];
    if (Value.size()>=0xFFFFFFFF)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=Value.size();
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Value.size()];

    Put_String(Value.size(), Value);

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_axml::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

