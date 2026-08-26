// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Colors.h"
#include <QPalette>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool GUI_Colors::IsDarkTheme()
{
    const QPalette DefaultPalette;
    return DefaultPalette.color(QPalette::WindowText).lightness()>DefaultPalette.color(QPalette::Window).lightness();
}

//---------------------------------------------------------------------------
QString GUI_Colors::Error()
{
    return IsDarkTheme()?"#ff7961":"#c0392b";
}

//---------------------------------------------------------------------------
QString GUI_Colors::Success()
{
    return IsDarkTheme()?"#66bb6a":"#1e7e34";
}

//---------------------------------------------------------------------------
QString GUI_Colors::Warning()
{
    return IsDarkTheme()?"#ffa726":"#a15c00";
}

//---------------------------------------------------------------------------
QString GUI_Colors::Info()
{
    return IsDarkTheme()?"#64b5f6":"#1565c0";
}

//---------------------------------------------------------------------------
QString GUI_Colors::Neutral()
{
    return IsDarkTheme()?"#b0b3b8":"#5f6368";
}

//---------------------------------------------------------------------------
QString GUI_Colors::Notice()
{
    return IsDarkTheme()?"#ce93d8":"#7b1fa2";
}
