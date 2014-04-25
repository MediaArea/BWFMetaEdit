// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Technical_TableH
#define GUI_Main_Technical_TableH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx__Common.h"
#include <QTableWidget>
#include <string>
class QEvent;
class GUI_Main;
class Core;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Technical_Table
//***************************************************************************

class GUI_Main_Technical_Table : public GUI_Main_xxxx__Common
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_Technical_Table(Core* C, GUI_Main* parent);

protected:
    //Events
    void                    contextMenuEvent   (QContextMenuEvent* Event);
    bool                    edit               (const QModelIndex &index, EditTrigger trigger, QEvent *Event); 

    //Helpers
    const string           &Fill_Content        ();
    group                   Fill_Group          ();
    bool                    Fill_Enabled        (const string &FileName, const string &Field, const string &Value);
};

#endif
