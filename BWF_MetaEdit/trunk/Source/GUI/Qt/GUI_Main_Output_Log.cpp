// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_Output_Log.h"
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
GUI_Main_Output_Log::GUI_Main_Output_Log(Core* _C, int KindOfLog_, QWidget* parent)
: QTextEdit(parent)
{
    //Internal
    C=_C;
    KindOfLog=KindOfLog_;

    //Configuration
    setLineWrapMode(QTextEdit::NoWrap);
    setReadOnly(true);
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main_Output_Log::event (QEvent *Event) 
{
    if (Event->type()==QEvent::User)
    {
        //Showing
        clear();
        switch (KindOfLog)
        {
            case 1 : setPlainText(QString().fromUtf8(ZenLib::Ztring(C->Text_stdout.str()).To_Local().c_str())); break;
            case 2 : setPlainText(QString().fromUtf8(ZenLib::Ztring(C->Text_stderr.str()).To_Local().c_str())); break;
            case 3 : setPlainText(QString().fromUtf8(ZenLib::Ztring(C->Text_stdall.str()).To_Local().c_str())); break;
            default: ;
        }

        Event->accept();
        return true;
    }

    return QTextEdit::event(Event);
}
