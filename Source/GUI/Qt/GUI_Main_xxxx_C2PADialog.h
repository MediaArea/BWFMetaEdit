// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_C2PADialogH
#define GUI_Main_xxxx_C2PADialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include <string>
class QWidget;
class QLabel;
class QTabWidget;
class QTextEdit;
class QPushButton;
class QDialogButtonBox;
struct json_value_s;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_C2PADialog
//***************************************************************************

class GUI_Main_xxxx_C2PADialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_C2PADialog(Core* C, const std::string &FileName_, QWidget* parent=NULL, bool Writable=false);

private Q_SLOTS:
    //Actions
    void OnMenu_Export              ();
    void OnMenu_Import              ();
    void OnMenu_ClearImport         ();

private:
    //Internal
    Core*           C;
    std::string     FileName;
    std::string     Status;
    std::string     Manifest;
    bool            Writable;
    std::string     SignManifestPath;

    //Widgets
    QTabWidget*         Central;
    QDialogButtonBox*   Dialog;
    QPushButton*        Export;
    QPushButton*        Import;
    QPushButton*        ClearImport;
    QLabel*             PendingBanner;

    //Helpers
    QWidget* Build_Summary             ();
    QLabel*  Build_StatusBanner        ();
    QWidget* Build_Manifest            (json_value_s* ManifestValue, int Index, int Count);
    QWidget* Build_ValidationResults   (json_value_s* ValidationResults);
    QLabel*  Build_ValidationItem      (json_value_s* Item, const QString &Color, const QString &Glyph);
    void     Update_PendingBanner      ();
};

#endif
