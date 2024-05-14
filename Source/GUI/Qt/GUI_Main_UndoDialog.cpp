// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_UndoDialog.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QMessageBox>
#include <QDesktopWidget>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_UndoDialog::GUI_Main_UndoDialog(Core* _C, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("Undo last save oprerations");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    QDialogButtonBox* Dialog=new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));

    //Central
    ListWidget=new QListWidget(this);
    connect(ListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
    ListInfo=new QTextEdit(this);
    ListInfo->setReadOnly(true);

    QGridLayout* L=new QGridLayout();
    L->addWidget(ListWidget, 0, 0);
    L->addWidget(ListInfo, 0, 1);
    L->addWidget(Dialog, 1, 0, 1, 2);

    setLayout(L);

    resize(QApplication::desktop()->screenGeometry().width()/2, QApplication::desktop()->screenGeometry().height()/3);

    //Fill
    ZtringList List=C->Menu_File_Undo_ListBackupFiles();
    for (size_t Pos=0; Pos<List.size(); Pos++)
        ListWidget->addItem(QString::fromUtf8(List[Pos].To_UTF8().c_str()));
}

//***************************************************************************
// Menu actions
//***************************************************************************

void GUI_Main_UndoDialog::currentItemChanged (QListWidgetItem* Current, QListWidgetItem* Previous)
{
    string ToDisplay=C->Menu_File_Undo_ListModifiedFiles(ListWidget->currentRow());

    ListInfo->setText(QString::fromUtf8(ToDisplay.c_str()));
}

