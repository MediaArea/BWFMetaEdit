// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_LoudnessH
#define GUI_Main_xxxx_LoudnessH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
class QDoubleSpinBox;
class QDialogButtonBox;
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
    QDoubleSpinBox*     Loudness;
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
