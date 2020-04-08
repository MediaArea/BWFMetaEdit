// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_Bext.h"
#include "Riff/Riff_Handler.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QIcon>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_Bext::GUI_Main_xxxx_Bext(Core* _C, const std::string &FileName_, int Maximum, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;
    int8u BextVersion=Ztring().From_UTF8(C->Get(FileName, "BextVersion")).To_int8u();

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("bext version");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    
    //Extra - Bext
    Version=new QDoubleSpinBox(this);
    Version->setMaximum(Maximum);
    Version->setDecimals(0);
    bool HasV1=C->Get(FileName, "LoudnessValue").empty() && C->Get(FileName, "LoudnessRange").empty() && C->Get(FileName, "MaxTruePeakLevel").empty() && C->Get(FileName, "MaxMomentaryLoudness").empty() && C->Get(FileName, "MaxShortTermLoudness").empty();
    bool HasV0=HasV1 && C->Get(FileName, "UMID").empty();
    if (HasV0)
        Version->setMinimum(0);
    else if (HasV1)
        Version->setMinimum(1);
    else
        Version->setMinimum(2);
    QLabel* Version_Label=new QLabel("bext version:");

    QGridLayout* L=new QGridLayout();
    L->addWidget(Version_Label, 0, 0);
    L->addWidget(Version, 0, 1);
    L->addWidget(Dialog, 1, 0, 1, 2);

    setLayout(L);
    Version->setFocus();

    //Default settings
    Version->setValue(BextVersion);
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_Bext::OnAccept ()
{
    std::string Value=Ztring::ToZtring(Version->value(), 0).To_UTF8();
    if (!C->IsValid(FileName, "BextVersion", Value))
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Ok);
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        MessageBox.exec();
        return;
    }

    C->Set(FileName, "BextVersion", Value);

    accept();
}
