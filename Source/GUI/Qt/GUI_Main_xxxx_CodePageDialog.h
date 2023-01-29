// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_CodePageDialogH
#define GUI_Main_xxxx_CodePageDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include <QComboBox>
class QDialogButtonBox;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_ComboBox
//***************************************************************************

class GUI_Main_xxxx_CodePageDialog : public QDialog
{
    Q_OBJECT

public:   
    //Constructor/Destructor
    GUI_Main_xxxx_CodePageDialog(Core* C, const std::string &FileName_, QWidget* parent=NULL);

    //Widgets
    QComboBox*          CodePage;
    QDialogButtonBox*   Dialog;

private Q_SLOTS:
    //Actions
    void OnAccept         ();

private:
    //Internal
    Core* C;
    std::string FileName;
};

#endif
