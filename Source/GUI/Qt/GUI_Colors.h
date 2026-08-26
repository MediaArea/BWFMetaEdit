// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_ColorsH
#define GUI_ColorsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <QString>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Colors
//***************************************************************************
namespace GUI_Colors
{
    bool    IsDarkTheme();

    QString Error();
    QString Success();
    QString Warning();
    QString Info();
    QString Neutral();
    QString Notice();
}

//---------------------------------------------------------------------------
#endif
