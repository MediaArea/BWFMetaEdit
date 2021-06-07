// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Core_TableH
#define GUI_Main_Core_TableH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx__Common.h"
#include "GUI/Qt/GUI_Main_xxxx_EditMenu.h"
#include <QTableWidget>
#include <QItemDelegate>
#include <string>
class QEvent;
class GUI_Main;
class Core;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Core_Table
//***************************************************************************

class GUI_Main_Core_Table : public GUI_Main_xxxx__Common  
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_Core_Table(Core* C, GUI_Main* parent);
    ~GUI_Main_Core_Table();

public Q_SLOTS:
    void onItemSelectionChanged();
    void onValuesChanged(bool onlySelected);

protected:
    //Events
    void contextMenuEvent   (QContextMenuEvent* Event);
    void keyPressEvent      (QKeyEvent* Event);
    bool edit               (const QModelIndex &index, EditTrigger trigger, QEvent *Event);

    //Helpers
    const string           &Fill_Content        ();
    group                   Fill_Group          ();
    bool                    Fill_Enabled        (const string &FileName, const string &Field, const string &Value);
    int                     Sort_Column_Get     () { return SortColumn; };
    void                    Sort_Column_Set     (int Column) { SortColumn=Column; };
    Qt::SortOrder           Sort_Order_Get      () { return SortOrder; };
    void                    Sort_Order_Set      (Qt::SortOrder Order) { Order=Order; };

private:
    GUI_Main_xxxx_EditMenu* MenuHandler;
    static int              SortColumn;
    static Qt::SortOrder    SortOrder;
};

#endif
