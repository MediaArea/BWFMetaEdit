// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Riff_C2PA_HelpersH
#define Riff_C2PA_HelpersH
#if defined(ENABLE_C2PA)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Riff/Riff_Base.h"
#include "Riff/Riff_Handler.h"

//---------------------------------------------------------------------------
void C2PA_Validate(Riff_Handler* Handler, Riff_Base::global* Global);
void C2PA_Sign(Riff_Handler* Handler, Riff_Base::global* Global);

//---------------------------------------------------------------------------
#endif // defined(ENABLE_C2PA)
#endif // Riff_C2PA_HelpersH