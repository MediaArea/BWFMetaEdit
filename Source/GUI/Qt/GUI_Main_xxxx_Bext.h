// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_BextH
#define GUI_Main_xxxx_BextH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
class QDialogButtonBox;
class QDoubleSpinBox;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_Bext
//***************************************************************************

class GUI_Main_xxxx_Bext : public QDialog
{
    Q_OBJECT

public:   
    //Constructor/Destructor
    GUI_Main_xxxx_Bext(Core* C, const std::string &FileName_, int Maximum, QWidget* parent=NULL);

    //Widgets
    QDoubleSpinBox*     Version;
    QDialogButtonBox*   Dialog;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();

private:
    //Internal
    Core* C;
    std::string FileName;
};

#endif
