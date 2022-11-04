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
    int32u CuePointId;
    string Value;
    Get_L4(CuePointId);
    if (Chunk.Content.Size>4)
        Get_String(Chunk.Content.Size-4, Value);

    //Filling
    ZtringList Item;
    Ztring Note;

    Note.From_UTF8(Value);
    Note.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Note.FindAndReplace(__T("\n"), __T("\\n"), 0, Ztring_Recursive);
    Item.push_back(Ztring().From_Number(CuePointId));
    Item.push_back(Note);

    ZtringListList Items(Ztring().From_UTF8(Global->adtl->Strings["note"].c_str()));
    Items.push_back(Item);
    Global->adtl->Strings["note"]=Items.Read().To_UTF8();
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_note::Modify_Internal ()
{
    ZtringListList Notes(Ztring().From_UTF8(Global->adtl?Global->adtl->Strings["note"]:string()));
    if (Notes.empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    int32u PointId=Notes[0](0).To_int32u();
    Ztring Note=Notes[0](1);
    Note.FindAndReplace(__T("\\n"), __T("\n"), 0, Ztring_Recursive);
    Note.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Note.FindAndReplace(__T("\n"), __T("\r\n"), 0, Ztring_Recursive);
    string Value=Note.To_UTF8();

    //Calculating size
    if (Value.size()>=0xFFFFFFFA)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=4+Value.size()+1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(PointId);
    Put_String(Value.size(), Value);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;

    Notes.erase(Notes.begin());
    Global->adtl->Strings["note"]=Notes.Read().To_UTF8();
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_note::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

