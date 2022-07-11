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
void Riff_WAVE_adtl_labl::Read_Internal ()
{

    //Reading
    Read_Internal_ReadAllInBuffer();

    //Integrity
    if (Chunk.Content.Size<4)
        throw exception_valid("wrong chunk size (labl)");

    //Parsing
    int32u CuePointId;
    string Value;
    Get_L4(CuePointId);
    if (Chunk.Content.Size>4)
    Get_String(Chunk.Content.Size-4, Value);
    Decode(Value);

    //Filling
    ZtringList Item;
    Ztring Label;

    Label.From_UTF8(Value);
    Label.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Label.FindAndReplace(__T("\n"), __T("\\n"), 0, Ztring_Recursive);
    Item.push_back(Ztring().From_Number(CuePointId));
    Item.push_back(Label);

    ZtringListList Items(Ztring().From_UTF8(Global->adtl->Strings["labl"].c_str()));
    Items.push_back(Item);
    Global->adtl->Strings["labl"]=Items.Read().To_UTF8();
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_labl::Modify_Internal ()
{
    ZtringListList Labels(Ztring().From_UTF8(Global->adtl?Global->adtl->Strings["labl"]:string()));
    if (Labels.empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    int32u PointId=Labels[0](0).To_int32u();
    Ztring Label=Labels[0](1);
    Label.FindAndReplace(__T("\\n"), __T("\n"), 0, Ztring_Recursive);
    Label.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Label.FindAndReplace(__T("\n"), __T("\r\n"), 0, Ztring_Recursive);
    string Value=Label.To_UTF8();
    Encode(Value);

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

    Labels.erase(Labels.begin());
    Global->adtl->Strings["labl"]=Labels.Read().To_UTF8();
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_labl::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

