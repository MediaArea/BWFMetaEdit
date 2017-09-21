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
// Top level element
//***************************************************************************

//---------------------------------------------------------------------------
Riff::Riff ()
:Riff_Base(NULL, 0)
{
    Global=new global();
}

//---------------------------------------------------------------------------
Riff::~Riff ()
{
    delete Global; //Global=NULL
}

//---------------------------------------------------------------------------
void Riff::Read_Internal ()
{
    SUBS_BEGIN();
        SUB_ELEMENT(WAVE);
    SUBS_END();
}

