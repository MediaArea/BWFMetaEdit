// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_Bext.h"
#include "Riff/Riff_Handler.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QtGui/QLabel>
#include <QtCore/QEvent>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_Bext::GUI_Main_xxxx_Bext(Core* _C, const std::string &FileName_, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("bext version");
    setWindowIcon (QIcon(":/Image/FADGI/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    
    //Extra - Bext
    Extra_Bext_DefaultVersion=new QComboBox();
    Extra_Bext_DefaultVersion->addItem("0");
    Extra_Bext_DefaultVersion->addItem("1");
    QLabel* Extra_Bext_DefaultVersion_Label=new QLabel("Default bext version:");

    QGridLayout* L=new QGridLayout();
    L->addWidget(Extra_Bext_DefaultVersion_Label, 0, 0);
    L->addWidget(Extra_Bext_DefaultVersion, 0, 1);
    L->addWidget(Dialog, 1, 0, 1, 2);

    setLayout(L);
    Extra_Bext_DefaultVersion->setFocus();

    //Default settings
    Extra_Bext_DefaultVersion->setCurrentIndex(Ztring(C->Get(FileName, "BextVersion")).To_int8u());
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_Bext::OnAccept ()
{
    std::string Value=Extra_Bext_DefaultVersion->currentText().toLocal8Bit().data();
    if (!C->IsValid(FileName, "BextVersion", Value))
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Ok);
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
        MessageBox.exec();
        return;
    }

    C->Set(FileName, "BextVersion", Value);

    accept();
}
