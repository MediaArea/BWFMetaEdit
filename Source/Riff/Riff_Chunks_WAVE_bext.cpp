// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Chunks.h"
#include <cstring>
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
    string Description, Originator, OriginatorReference, OriginationDate, OriginationTime, CodingHistory;
    int128u UMID=0;
    int64u TimeReference;
    int16u Version, LoudnessValue=0, LoudnessRange=0, MaxTruePeakLevel=0, MaxMomentaryLoudness=0, MaxShortTermLoudness=0;
    Get_String(256, Description);
    Get_String( 32, Originator);
    Get_String( 32, OriginatorReference);
    Get_String( 10, OriginationDate);
    Get_String(  8, OriginationTime);
    Get_L8    (     TimeReference); //To be divided by SamplesPerSec
    Get_L2    (     Version);
    if (Version>=1)
        Get_UUID(UMID);
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
        Global->bext->Strings["timereference"].From_Number(TimeReference);
    Global->bext->Strings["bextversion"]=Ztring().From_Number(Version);
    if (UMID!=0)
        Global->bext->Strings["umid"].From_UUID(UMID);
    if (Version>=2)
    {
        if (LoudnessValue)
            Global->bext->Strings["loudnessvalue"].From_Number((float)((int16s)LoudnessValue)/100, 2);
        if (LoudnessRange)
            Global->bext->Strings["loudnessrange"].From_Number((float)((int16s)LoudnessRange)/100, 2);
        if (MaxTruePeakLevel)
            Global->bext->Strings["maxtruepeaklevel"].From_Number((float)((int16s)MaxTruePeakLevel)/100, 2);
        if (MaxMomentaryLoudness)
            Global->bext->Strings["maxmomentaryloudness"].From_Number((float)((int16s)MaxMomentaryLoudness)/100, 2);
        if (MaxShortTermLoudness)
            Global->bext->Strings["maxshorttermloudness"].From_Number((float)((int16s)MaxShortTermLoudness)/100, 2);
    }
    if (!CodingHistory.empty())
        Global->bext->Strings["codinghistory"]=CodingHistory;
    for (std::map<string, Ztring>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
        String->second.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
    for (std::map<string, Ztring>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
        String->second.FindAndReplace("\r", "\n", 0, Ztring_Recursive);
    for (std::map<string, Ztring>::iterator String=Global->bext->Strings.begin(); String!=Global->bext->Strings.end(); String++)
        String->second.FindAndReplace("\n", EOL, 0, Ztring_Recursive);
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_bext::Modify_Internal ()
{
    if (Global->bext==NULL
     || (Global->bext->Strings["description"].empty()
      && Global->bext->Strings["originator"].empty()
      && Global->bext->Strings["originatorreference"].empty()
      && Global->bext->Strings["originationdate"].empty()
      && Global->bext->Strings["originationtime"].empty()
      && Global->bext->Strings["timereference (translated)"].empty()
      && Global->bext->Strings["timereference"].empty()
      && Global->bext->Strings["umid"].empty()
      && Global->bext->Strings["codinghistory"].empty()))
    {
        Chunk.Content.IsRemovable=true;
        return;
    }

    //Calculating size
    int64u TargetedSize=858;
    if (Chunk.Content.Size)
        TargetedSize=Chunk.Content.Size;
    if (TargetedSize<=602+Global->bext->Strings["codinghistory"].size())
        TargetedSize=602+Global->bext->Strings["codinghistory"].size();
    if (TargetedSize>=0x100000000LL)
        return; //TODO: error

    //Preparing
    int64u TimeReference=Global->bext->Strings["timereference"].To_int64u();
    /*
    Ztring TimeReferenceS=Global->bext->Strings["timereference"];
    if (TimeReferenceS.size()>=12)
    {
        int64u HH=Ztring(TimeReferenceS.substr(0, TimeReferenceS.size()-10)).To_int64u(); 
        int64u MM=Ztring(TimeReferenceS.substr(TimeReferenceS.size()-9, 2 )).To_int64u(); 
        int64u SS=Ztring(TimeReferenceS.substr(TimeReferenceS.size()-6, 2 )).To_int64u(); 
        int64u MS=Ztring(TimeReferenceS.substr(TimeReferenceS.size()-3, 3 )).To_int64u();
        TimeReference=HH*60*60*1000
                    + MM*   60*1000
                    + SS*      1000
                    + MS;
        TimeReference*=Global->fmt_->sampleRate;
        TimeReference/=1000;
    }
    */
    
    //Creating buffer
    Chunk.Content.Buffer_Offset=0;
    Chunk.Content.Size=TargetedSize;
    delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=new int8u[(size_t)TargetedSize];
    memset(Chunk.Content.Buffer, '\0', (size_t)TargetedSize);

    int128u UIMD=Ztring(Global->bext->Strings["umid"]).To_UUID();
    int16s  LoudnessValue=(int16u)float32_int32s(Ztring(Global->bext->Strings["loudnessvalue"]).To_float32()*100);
    int16s  LoudnessRange=(int16u)float32_int32s(Ztring(Global->bext->Strings["loudnessrange"]).To_float32()*100);
    int16s  MaxTruePeakLevel=(int16u)float32_int32s(Ztring(Global->bext->Strings["maxtruepeaklevel"]).To_float32()*100);
    int16s  MaxMomentaryLoudness=(int16u)float32_int32s(Ztring(Global->bext->Strings["maxmomentaryloudness"]).To_float32()*100);
    int16s  MaxShortTermLoudness=(int16u)float32_int32s(Ztring(Global->bext->Strings["maxshorttermloudness"]).To_float32()*100);
    int16u  BextVersion=Global->bext->Strings["bextversion"].To_int16u();
    if (BextVersion<1 && UIMD)
        BextVersion=1;
    if (BextVersion<2 && (LoudnessValue || LoudnessRange || MaxTruePeakLevel || MaxMomentaryLoudness || MaxShortTermLoudness))
        BextVersion=2;
    Put_String(256, Global->bext->Strings["description"]);
    Put_String( 32, Global->bext->Strings["originator"]);
    Put_String( 32, Global->bext->Strings["originatorreference"]);
    Put_String( 10, Global->bext->Strings["originationdate"]);
    Put_String(  8, Global->bext->Strings["originationtime"]);
    Put_L8    (     TimeReference);
    Put_L2    (     BextVersion);
    Put_UUID  (UIMD);
    Put_L2    (     LoudnessValue);
    Put_L2    (     LoudnessRange);
    Put_L2    (     MaxTruePeakLevel);
    Put_L2    (     MaxMomentaryLoudness);
    Put_L2    (     MaxShortTermLoudness);
    Skip_XX   (602-Chunk.Content.Buffer_Offset);
    if (Chunk.Content.Buffer_Offset<Chunk.Content.Size)
        Put_String(Global->bext->Strings["codinghistory"].size(), Global->bext->Strings["codinghistory"]);

    if (Chunk.Content.Buffer_Offset<858)
        Chunk.Content.Size=858; //We free the no more needed space

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

