// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Output_LogH
#define GUI_Main_Output_LogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QTextEdit>
class QEvent;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Output_Log
//***************************************************************************

class GUI_Main_Output_Log : public QTextEdit
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_Output_Log(Core* C, int KindOfLog, QWidget* parent);

    //Events
    bool event (QEvent *Event); 

private:
    //Internal
    Core*   C;
    int   KindOfLog;
};

#endif
