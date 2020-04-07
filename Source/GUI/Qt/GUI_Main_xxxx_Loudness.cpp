// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_Loudness.h"
#include "Riff/Riff_Handler.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QIcon>
//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
Loudness_SpinBox::Loudness_SpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
}

//---------------------------------------------------------------------------
void Loudness_SpinBox::fixup(QString &input) const
{
        if(input == suffix() || input == QString(specialValueText().append(suffix())))
            input = QString("%1%2").arg(minimum()).arg(suffix());
        else
            QDoubleSpinBox::fixup(input);
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_Loudness::GUI_Main_xxxx_Loudness(Core* _C, const std::string &FileName_, const std::string &Field_, const QString &Value, bool Rules_Requirements_, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;
    Field=Field_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle(Field.c_str());
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));

    //Extra - Bext
    Loudness=new Loudness_SpinBox();
    Loudness->setSpecialValueText("None");
    Loudness->setDecimals(2);

    // Ranges are from min -1 (for None value)
    if (Rules_Requirements_)
    {
        if (Field=="LoudnessValue" || Field=="MaxTruePeakLevel" || Field=="MaxMomentaryLoudness" || Field=="MaxShortTermLoudness")
            Loudness->setRange(-100.00, 99.99);
        if (Field=="LoudnessRange")
            Loudness->setRange(-1, 99.99);
    }
    else
        Loudness->setRange(-655.37, 655.35);
    if (Field=="LoudnessValue" || Field=="MaxMomentaryLoudness" || Field=="MaxShortTermLoudness")
        Loudness->setSuffix(" LUFS");
    if (Field=="LoudnessRange")
        Loudness->setSuffix(" LU");
    if (Field=="MaxTruePeakLevel")
        Loudness->setSuffix(" dBTP");
    QLabel* Loudness_Label=new QLabel(QString(Field.c_str())+" value:");

    QGridLayout* L=new QGridLayout();
    L->addWidget(Loudness_Label, 0, 0);
    L->addWidget(Loudness, 0, 1);
    L->addWidget(Dialog, 1, 0, 1, 2);

    setLayout(L);
    Loudness->setFocus();

    //Default settings
    Loudness->setValue(Ztring().From_UTF8(C->Get(FileName, Field)).To_float32());
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_Loudness::OnAccept ()
{
    std::string Value = "";
    if(Loudness->value() != Loudness->minimum())
    {
        Value=Ztring().From_Number(Loudness->value(), 2).To_UTF8();
        if (!C->IsValid(FileName, Field, Value))
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
    }

    C->Set(FileName, Field, Value);

    accept();
}


