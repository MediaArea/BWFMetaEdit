// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_UndoDialogH
#define GUI_Main_UndoDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <QDialog>
class QPushButton;
class QListWidget;
class QTextEdit;
class QListWidgetItem;
class Core;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_UndoDialog
//***************************************************************************

class GUI_Main_UndoDialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_UndoDialog(Core* C, QWidget* parent=NULL);

    //Widgets
    QListWidget*                        ListWidget;
    QTextEdit*                          ListInfo;
    QPushButton*                        Cancel;
    QPushButton*                        Valid;

private Q_SLOTS:
    //Actions
    void currentItemChanged         (QListWidgetItem* Current, QListWidgetItem* Previous);

private:
    //Internal
    Core*   C;
};

#endif
