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
//---------------------------------------------------------------------------

//***************************************************************************
// WAVE fmt
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_fmt_::Read_Internal ()
{
    //Integrity
    if (Global->fmt_)
        throw exception_valid("2 fmt_ chunks");

    //Reading
    Read_Internal_ReadAllInBuffer();
    
    //Parsing
    int32u sampleRate, bytesPerSecond;
    int16u formatType, channelCount, blockAlignment, bitsPerSample;
    Get_L2    (formatType);
    Get_L2    (channelCount);
    Get_L4    (sampleRate);
    Get_L4    (bytesPerSecond);
    Get_L2    (blockAlignment);
    Get_L2    (bitsPerSample);

    //Integrity
    if (formatType==1 && bitsPerSample!=0 && channelCount*sampleRate*bitsPerSample!=bytesPerSecond*8) //if PCM
        throw exception_valid("fmt_");
    
    //Filling
    Global->fmt_=new Riff_Base::global::chunk_fmt_;
    Global->fmt_->formatType=formatType;
    Global->fmt_->channelCount=channelCount;
    Global->fmt_->sampleRate=sampleRate;
    Global->fmt_->bytesPerSecond=bytesPerSecond;
    Global->fmt_->blockAlignment=blockAlignment;
    Global->fmt_->bitsPerSample=bitsPerSample;
}

