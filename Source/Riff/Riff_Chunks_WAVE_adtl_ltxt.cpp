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
    int32u CuePointId, SampleLength, PurposeId;
    int16u Country, Language, Dialect, CodePage;
    string Value;
    Get_L4(CuePointId);
    Get_L4(SampleLength);
    Get_B4(PurposeId);
    Get_L2(Country);
    Get_L2(Language);
    Get_L2(Dialect);
    Get_L2(CodePage);
    if (Chunk.Content.Size>20)
    {
        Get_String(Chunk.Content.Size-20, Value);
        Decode(Value);
    }

    //Filling
    ZtringList Item;
    Ztring Text;
    Text.From_UTF8(Value);
    Text.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Text.FindAndReplace(__T("\n"), __T("\\n"), 0, Ztring_Recursive);
    Item.push_back(Ztring().From_Number(CuePointId));
    Item.push_back(Ztring().From_Number(SampleLength));
    Item.push_back(__T("0x")+Ztring().From_Number(PurposeId, 16));
    Item.push_back(Ztring().From_Number(Country));
    Item.push_back(Ztring().From_Number(Language));
    Item.push_back(Ztring().From_Number(Dialect));
    Item.push_back(Ztring().From_Number(CodePage));
    Item.push_back(Text);

    ZtringListList Items(Ztring().From_UTF8(Global->adtl->Strings["ltxt"].c_str()));
    Items.push_back(Item);
    Global->adtl->Strings["ltxt"]=Items.Read().To_UTF8();
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_ltxt::Modify_Internal ()
{
    ZtringListList Texts(Ztring().From_UTF8(Global->adtl?Global->adtl->Strings["ltxt"]:string()));
    if (Texts.empty())
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    int32u PointId=Texts[0](0).To_int32u();
    int32u SampleLength=Texts[0](1).To_int32u();
    int32u PurposeId=Texts[0](2).To_int32u(16);
    int16u Country=Texts[0](3).To_int16u();
    int16u Language=Texts[0](4).To_int16u();
    int16u Dialect=Texts[0](5).To_int16u();
    int16u CodePage=Texts[0](6).To_int16u();
    Ztring Text=Texts[0](7);
    Text.FindAndReplace(__T("\\n"), __T("\n"), 0, Ztring_Recursive);
    Text.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    Text.FindAndReplace(__T("\n"), __T("\r\n"), 0, Ztring_Recursive);
    string Value=Text.To_UTF8();
    Encode(Value);

    //Calculating size
    if (Value.size()>=0xFFFFFFEA)
        return; //TODO: error

    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=20+Value.size()+1;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[Chunk.Content.Size];

    Put_L4(PointId);
    Put_L4(SampleLength);
    Put_B4(PurposeId);
    Put_L2(Country);
    Put_L2(Language);
    Put_L2(Dialect);
    Put_L2(CodePage);
    Put_String(Value.size(), Value);
    Put_L1(0x00); //ZSTR i.e. null terminated text string

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;

    Texts.erase(Texts.begin());
    Global->adtl->Strings["ltxt"]=Texts.Read().To_UTF8();
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_adtl_ltxt::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

