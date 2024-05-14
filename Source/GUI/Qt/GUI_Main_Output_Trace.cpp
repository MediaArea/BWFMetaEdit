// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_Output_Trace.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Trace::GUI_Main_Trace(Core* _C, QWidget* parent)
: QTextEdit(parent)
{
    //Internal
    C=_C;

    //Configuration
    setLineWrapMode(QTextEdit::NoWrap);
    setReadOnly(true);

    //Settings
    QFont Font;
    Font.setFamily("Courier");
    Font.setStyleHint(QFont::TypeWriter);
    Font.setFixedPitch(true);
    setCurrentFont(Font);
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main_Trace::event (QEvent *Event) 
{
    if (Event->type()==QEvent::User)
    {
        //Showing
        clear();
        setPlainText(QString::fromUtf8(C->Output_Trace_Get().c_str()));

        Event->accept();
        return true;
    }

    return QTextEdit::event(Event);
}
