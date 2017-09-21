// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_LoudnessH
#define GUI_Main_xxxx_LoudnessH
//---------------------------------------------------------------------------

#include <iostream>

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include <QDoubleSpinBox>
class QDoubleSpinBox;
class QDialogButtonBox;
//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
class Loudness_SpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    Loudness_SpinBox(QWidget *parent = 0);
    void fixup(QString &input) const;
};

//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_Loudness
//***************************************************************************

class GUI_Main_xxxx_Loudness : public QDialog
{
    Q_OBJECT

public:   
    //Constructor/Destructor
    GUI_Main_xxxx_Loudness(Core* C, const std::string &FileName_, const std::string &Field_, const QString &Value, bool Rules_Requirements_, QWidget* parent=NULL);

    //Widgets
    Loudness_SpinBox*   Loudness;
    QDialogButtonBox*   Dialog;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();

private:
    //Internal
    Core* C;
    std::string FileName;
    std::string Field;
};

#endif
