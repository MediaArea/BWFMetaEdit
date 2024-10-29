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
#include <cstring>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
struct umid
{
    int128u UniversalLabel;
    int128u MaterialNumber;
    int32u  Time;
    int32u  Date;
    int32u  Spacial_Altitude;
    int32u  Spacial_Longitudinal;
    int32u  Spacial_Latitude;
    int32u  Country;
    int32u  Org;
    int32u  User;

    umid()
    {
        UniversalLabel.hi=0;
        UniversalLabel.lo=0;
        MaterialNumber.hi=0;
        MaterialNumber.lo=0;
        Time=0;
        Date=0;
        Spacial_Altitude=0;
        Spacial_Longitudinal=0;
        Spacial_Latitude=0;
        Country=0;
        Org=0;
        User=0;
    }

    Ztring ToString_Temp(int32u Value)
    {
        Ztring Temp;
        Temp.append(Ztring::ToZtring( Value>>28));
        Temp.append(Ztring::ToZtring((Value>>24)&0xF));
        Temp.append(Ztring::ToZtring((Value>>20)&0xF));
        Temp.append(Ztring::ToZtring((Value>>16)&0xF));
        Temp.append(Ztring::ToZtring((Value>>12)&0xF));
        Temp.append(Ztring::ToZtring((Value>> 8)&0xF));
        Temp.append(Ztring::ToZtring((Value>> 4)&0xF));
        Temp.append(Ztring::ToZtring((Value    )&0xF));

        return Temp;
    }

    Ztring ToString()
    {
        Ztring Temp;
        if ((UniversalLabel.lo&0x00000000FF000000LL)>=0x0000000013000000LL)
        {
            Temp.append(ToString_Temp((int32u)(UniversalLabel.hi>>24)));
            Temp.append(ToString_Temp((int32u)(UniversalLabel.hi>>16)));
            Temp.append(ToString_Temp((int32u)(UniversalLabel.hi>> 8)));
            Temp.append(ToString_Temp((int32u)(UniversalLabel.hi    )));
            Temp.append(ToString_Temp((int32u)(MaterialNumber.hi>>24)));
            Temp.append(ToString_Temp((int32u)(MaterialNumber.hi>>16)));
            Temp.append(ToString_Temp((int32u)(MaterialNumber.hi>> 8)));
            Temp.append(ToString_Temp((int32u)(MaterialNumber.hi    )));
            if ((UniversalLabel.lo&0x00000000FF000000LL)>=0x0000000033000000LL)
            {
                Temp.append(ToString_Temp(Time));
                Temp.append(ToString_Temp(Date));
                Temp.append(ToString_Temp(Spacial_Altitude));
                Temp.append(ToString_Temp(Spacial_Longitudinal));
                Temp.append(ToString_Temp(Spacial_Latitude));
                Temp.append(ToString_Temp(Country));
                Temp.append(ToString_Temp(Org));
                Temp.append(ToString_Temp(User));
            }
        }
        return Temp;
    }

    static void BufferToString(string &Value, int8u* Buffer, size_t &Buffer_Pos)
    {
        size_t Buffer_Pos_Max=Buffer_Pos+64;
        for (; Buffer_Pos<Buffer_Pos_Max; Buffer_Pos++)
        {
            int8u Char1=Buffer[Buffer_Pos]>>4;
            int8u Char2=Buffer[Buffer_Pos]&0xF;
            if (Char1<10)
                Value.push_back(__T('0')+Char1);
            else
                Value.push_back(__T('A')+Char1-10);
            if (Char2<10)
                Value.push_back(__T('0')+Char2);
            else
                Value.push_back(__T('A')+Char2-10);
        }

        bool Is128=false;
        bool Is64=false;
        for (size_t Pos=0; Pos<Value.size(); Pos++)
            if (Value[Pos]!=__T('0'))
            {
                Is64=true;
                if (Pos>=64)
                    Is128=true;
            }
        if (!Is64)
            Value.clear();
        else if (!Is128)
            Value.resize(64);
    }

    static void StringToBuffer(string Value, int8u* Buffer, size_t &Buffer_Pos)
    {
        Ztring temp=Ztring().From_UTF8(Value);
        temp.MakeUpperCase();
        Value=temp.To_UTF8();
        size_t Buffer_Pos_End=Buffer_Pos+64;

        for (size_t Pos=0; Pos<Value.size(); Pos+=2)
        {
            Buffer[Buffer_Pos]=0x00;
            if (Value[Pos]<='9')
                Buffer[Buffer_Pos]|=(Value[Pos]-'0')<<4;
            else
                Buffer[Buffer_Pos]|=(Value[Pos]-'A'+10)<<4;
            if (Value[Pos+1]<='9')
                Buffer[Buffer_Pos]|=(Value[Pos+1]-'0');
            else
                Buffer[Buffer_Pos]|=(Value[Pos+1]-'A'+10);
            Buffer_Pos++;
        }
        for (; Buffer_Pos<Buffer_Pos_End; Buffer_Pos++)
            Buffer[Buffer_Pos]=0x00;
    }
};
//---------------------------------------------------------------------------

//***************************************************************************
// Read
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_bext::Read_Internal ()
{
    //Integrity
    if (Global->bext)
        throw exception_valid("2 bext chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    string Description, Originator, OriginatorReference, OriginationDate, OriginationTime, UMID, CodingHistory;
    int64u TimeReference;
    int16u Version, LoudnessValue=0, LoudnessRange=0, MaxTruePeakLevel=0, MaxMomentaryLoudness=0, MaxShortTermLoudness=0;
    Get_String(256, Description);
    Get_String( 32, Originator);
    Get_String( 32, OriginatorReference);
    Get_String( 10, OriginationDate);
    Get_String(  8, OriginationTime);;
    Get_L8    (     TimeReference); //To be divided by SamplesPerSec
    Get_L2    (     Version);
    if (Version>=1)
         umid::BufferToString(UMID, Chunk.Content.Buffer, Chunk.Content.Buffer_Offset);
    if (Version>=2)
    {
        Get_L2(     LoudnessValue);
        Get_L2(     LoudnessRange);
        Get_L2(     MaxTruePeakLevel);
        Get_L2(     MaxMomentaryLoudness);
        Get_L2(     MaxShortTermLoudness);
    }
    Skip_XX   (602-Chunk.Content.Buffer_Offset);
    if (Chunk.Content.Buffer_Offset<Chunk.Content.Size)
        Get_String(Chunk.Content.Size-Chunk.Content.Buffer_Offset, CodingHistory);

    //Filling
    Global->bext=new Riff_Base::global::chunk_strings;
    if (!Description.empty())
        Global->bext->Strings["description"]=Description;
    if (!Originator.empty())
        Global->bext->Strings["originator"]=Originator;
    if (!OriginatorReference.empty())
        Global->bext->Strings["originatorreference"].assign(OriginatorReference);
    if (!OriginationDate.empty())
        Global->bext->Strings["originationdate"]=OriginationDate;
    if (!OriginationTime.empty())
        Global->bext->Strings["originationtime"]=OriginationTime;
    if (TimeReference)
        Global->bext->Strings["timereference"]=Ztring().From_Number(TimeReference).To_UTF8();
    Global->bext->Strings["bextversion"]=Ztring().From_Number(Version).To_UTF8();
    Global->bext->Strings["umid"]=UMID;
    if (Version>=2)
    {
        if (LoudnessValue!=0x7FFF)
            Global->bext->Strings["loudnessvalue"]=Ztring().From_Number((float)((int16s)LoudnessValue)/100, 2).To_UTF8();
        if (LoudnessRange!=0x7FFF)
            Global->bext->Strings["loudnessrange"]=Ztring().From_Number((float)((int16s)LoudnessRange)/100, 2).To_UTF8();
        if (MaxTruePeakLevel!=0x7FFF)
            Global->bext->Strings["maxtruepeaklevel"]=Ztring().From_Number((float)((int16s)MaxTruePeakLevel)/100, 2).To_UTF8();
        if (MaxMomentaryLoudness!=0x7FFF)
            Global->bext->Strings["maxmomentaryloudness"]=Ztring().From_Number((float)((int16s)MaxMomentaryLoudness)/100, 2).To_UTF8();
        if (MaxShortTermLoudness!=0x7FFF)
            Global->bext->Strings["maxshorttermloudness"]=Ztring().From_Number((float)((int16s)MaxShortTermLoudness)/100, 2).To_UTF8();
    }
    if (!CodingHistory.empty())
        Global->bext->Strings["codinghistory"]=CodingHistory;
    for (std::map<string, string>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
    {
        for (size_t Pos=String->second.find("\r\n"); Pos!=std::string::npos;  Pos=String->second.find("\r\n", Pos+1))
            String->second.replace(Pos, 2, "\n");
    }
    for (std::map<string, string>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
    {
        for (size_t Pos=String->second.find("\r"); Pos!=std::string::npos; Pos=String->second.find("\r", Pos+1))
            String->second.replace(Pos, 1, "\n");
    }
    for (std::map<string, string>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
    {
        for (size_t Pos=String->second.find("\n"); Pos!=std::string::npos; Pos=String->second.find("\n", Pos+2))
            String->second.replace(Pos, 1, "\r\n");
    }
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_bext::Modify_Internal ()
{
    if (Chunk.Content.IsRemovable)
        return;

    if (Global->bext==NULL
     || (Global->bext->Strings["description"].empty()
      && Global->bext->Strings["originator"].empty()
      && Global->bext->Strings["originatorreference"].empty()
      && Global->bext->Strings["originationdate"].empty()
      && Global->bext->Strings["originationtime"].empty()
      && Global->bext->Strings["timereference (translated)"].empty()
      && Global->bext->Strings["timereference"].empty()
      && Global->bext->Strings["bextversion"].empty()
      && Global->bext->Strings["umid"].empty()
      && Global->bext->Strings["loudnessvalue"].empty()
      && Global->bext->Strings["loudnessrange"].empty()
      && Global->bext->Strings["maxtruepeaklevel"].empty()
      && Global->bext->Strings["maxmomentaryloudness"].empty()
      && Global->bext->Strings["maxshorttermloudness"].empty()
      && Global->bext->Strings["codinghistory"].empty()))
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    string Description=Global->bext->Strings["description"];
    string Originator=Global->bext->Strings["originator"];
    string OriginatorReference=Global->bext->Strings["originatorreference"];
    string OriginationDate=Global->bext->Strings["originationdate"];
    string OriginationTime=Global->bext->Strings["originationtime"];
    string CodingHistory=Global->bext->Strings["codinghistory"];

    //Calculating size
    int64u TargetedSize=858;
    if (Chunk.Content.Size)
        TargetedSize=Chunk.Content.Size;
    if (TargetedSize<=602+CodingHistory.size())
        TargetedSize=602+CodingHistory.size();
    if (TargetedSize>=0x100000000LL)
        return; //TODO: error

    //Preparing
    int64u TimeReference=Ztring().From_UTF8(Global->bext->Strings["timereference"]).To_int64u();
    
    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    if (TargetedSize>Chunk.Content.Size)
    {
        int8u* BufferTemp=Chunk.Content.Buffer;
        Chunk.Content.Buffer=new int8u[(size_t)TargetedSize];
        memcpy(Chunk.Content.Buffer, BufferTemp, (size_t)Chunk.Content.Size);
        memset(Chunk.Content.Buffer+Chunk.Content.Size, '\0', (size_t)(TargetedSize-Chunk.Content.Size));
        delete[] BufferTemp;
    }
    Chunk.Content.Size=TargetedSize;

    int16s LoudnessValue=0x7FFF;
    if(!Global->bext->Strings["loudnessvalue"].empty())
        LoudnessValue=(int16u)float32_int32s(Ztring().From_UTF8(Global->bext->Strings["loudnessvalue"]).To_float32()*100);
    int16s LoudnessRange=0x7FFF;
    if(!Global->bext->Strings["loudnessrange"].empty())
        LoudnessRange=(int16u)float32_int32s(Ztring().From_UTF8(Global->bext->Strings["loudnessrange"]).To_float32()*100);
    int16s MaxTruePeakLevel=0x7FFF;
    if(!Global->bext->Strings["maxtruepeaklevel"].empty())
        MaxTruePeakLevel=(int16u)float32_int32s(Ztring().From_UTF8(Global->bext->Strings["maxtruepeaklevel"]).To_float32()*100);
    int16s MaxMomentaryLoudness=0x7FFF;
    if(!Global->bext->Strings["maxmomentaryloudness"].empty())
        MaxMomentaryLoudness=(int16u)float32_int32s(Ztring().From_UTF8(Global->bext->Strings["maxmomentaryloudness"]).To_float32()*100);
    int16s MaxShortTermLoudness=0x7FFF;
    if(!Global->bext->Strings["maxshorttermloudness"].empty())
        MaxShortTermLoudness=(int16u)float32_int32s(Ztring().From_UTF8(Global->bext->Strings["maxshorttermloudness"]).To_float32()*100);

    int16u  BextVersion=Ztring().From_UTF8(Global->bext->Strings["bextversion"]).To_int16u();
    if (BextVersion<1 && !Global->bext->Strings["umid"].empty())
        BextVersion=1;
    if (BextVersion<2 && (LoudnessValue!=0x7FFF || LoudnessRange!=0x7FFF || MaxTruePeakLevel!=0x7FFF || MaxMomentaryLoudness!=0x7FFF || MaxShortTermLoudness!=0x7FFF))
        BextVersion=2;


    Put_String(256, Description);
    Put_String( 32, Originator);
    Put_String( 32, OriginatorReference);
    Put_String( 10, OriginationDate);
    Put_String(  8, OriginationTime);
    Put_L8    (     TimeReference);
    Put_L2    (     BextVersion);
    if (Global->bext->Strings["umid"].find(__T('-'))!=string::npos)
    {
        int128u UIMD; UIMD.hi=0x060A2B3401010101LL; UIMD.lo=0x0101021013000000LL;
        Put_UUID(UIMD);
        UIMD=Ztring().From_UTF8(Global->bext->Strings["umid"]).To_UUID();
        Put_UUID(UIMD);
        UIMD.hi=UIMD.lo=0;
        Put_UUID(UIMD);
        Put_UUID(UIMD);
    }
    if (!Global->bext->Strings["umid"].empty())
        umid::StringToBuffer(Global->bext->Strings["umid"], Chunk.Content.Buffer, Chunk.Content.Buffer_Offset);
    else
    {
        Put_UUID(int128u(0));
        Put_UUID(int128u(0));
        Put_UUID(int128u(0));
        Put_UUID(int128u(0));
    }
    if (BextVersion>=2)
    {
        Put_L2    (     LoudnessValue);
        Put_L2    (     LoudnessRange);
        Put_L2    (     MaxTruePeakLevel);
        Put_L2    (     MaxMomentaryLoudness);
        Put_L2    (     MaxShortTermLoudness);
    }
    if (BextVersion<=2)
        while (Chunk.Content.Buffer_Offset<602)
            Put_L1(0); //Erasing old data
    else
        Skip_XX   (602-Chunk.Content.Buffer_Offset); //Keeping old data
    if (Chunk.Content.Buffer_Offset<Chunk.Content.Size)
        Put_String(Chunk.Content.Size-Chunk.Content.Buffer_Offset, CodingHistory);

    Chunk.Content.IsModified=true;
    Chunk.Content.Size_IsModified=true;
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_bext::Write_Internal ()
{
    Riff_Base::Write_Internal(Chunk.Content.Buffer, (size_t)Chunk.Content.Size);
}

