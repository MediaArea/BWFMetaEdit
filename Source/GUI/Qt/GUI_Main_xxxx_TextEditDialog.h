// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_TextEditDialogH
#define GUI_Main_xxxx_TextEditDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include <string>
class QTextEdit;
class QLabel;
class QDialogButtonBox;
class QEvent;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_TextEditDialog
//***************************************************************************

class GUI_Main_xxxx_TextEditDialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_TextEditDialog(Core* C, const std::string &FileName, const std::string &Field, const QString &Value, bool ReadOnly=false, QWidget* parent=NULL);

    //Widgets
    QTextEdit*          TextEdit;
    QLabel*             Label;
    QLabel*             BigMessage;
    QLabel*             Message;
    QDialogButtonBox*   Dialog;
    QPushButton*        Load;
    QPushButton*        Save;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();
    void OnTextChanged              ();
    void OnMenu_Load                ();
    void OnMenu_Save                ();

private:
    //Internal
    Core* C;
    std::string FileName;
    std::string Field;
    QString BigValue;
    bool ReadOnly;

    void Display(const QString& Value);
};

#endif
