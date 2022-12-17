// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_CodePageDialog.h"
#include "Common/Core.h"
#include <QLabel>
#include <QEvent>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QIcon>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_CodePageDialog::GUI_Main_xxxx_CodePageDialog(Core* _C, const std::string &FileName_, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("File Encoding");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));

    //Extra - Bext
    CodePage=new QComboBox(this);
    QLabel* Label=new QLabel("Encoding:");

    CodePage->addItem("(65001) UTF-8", QString("UTF-8"));
    CodePage->addItem("(437) IBM CP437", QString("CP437"));
    CodePage->addItem("(850) IBM CP850", QString("CP850"));
    CodePage->addItem("(858) IBM CP858", QString("CP858"));
    CodePage->addItem("(1252) Windows 1252", QString("CP1252"));
    CodePage->addItem("(28591) ISO-8859-1", QString("8859-1"));
    CodePage->addItem("(28592) ISO-8859-2", QString("8859-2"));

    QGridLayout* L=new QGridLayout();
    L->addWidget(Label, 0, 0);
    L->addWidget(CodePage, 0, 1);
    L->addWidget(Dialog, 1, 0, 1, 2);

    setLayout(L);
    CodePage->setFocus();

    //Default settings
    std::string Value = C->Get(FileName, "Encoding");
    if (Value=="UTF-8")
        CodePage->setCurrentIndex(0);
    else if (Value=="CP437")
        CodePage->setCurrentIndex(1);
    else if (Value=="CP850")
        CodePage->setCurrentIndex(2);
    else if (Value=="CP858")
        CodePage->setCurrentIndex(3);
    else if (Value=="CP1252")
        CodePage->setCurrentIndex(4);
    else if (Value=="ISO-8859-1")
        CodePage->setCurrentIndex(5);
    else if (Value=="ISO-8859-2")
        CodePage->setCurrentIndex(6);
    else
        CodePage->setCurrentIndex(-1);

    //Loudness->setValue(Ztring().From_UTF8(C->Get(FileName, Field)).To_float32());
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodePageDialog::OnAccept ()
{
    std::string Value = CodePage->currentData().toString().toStdString();
    if (!Value.empty())
        C->Set(FileName, "Encoding", Value);

    accept();
}


