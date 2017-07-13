// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_DateDialogH
#define GUI_Main_xxxx_DateDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
class QTabWidget;
class QCalendarWidget;
class QTextEdit;
class QTimeEdit;
class QCheckBox;
class QGridLayout;
class QLabel;
class QDialogButtonBox;
class QEvent;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_DateDialog
//***************************************************************************

class GUI_Main_xxxx_DateDialog : public QDialog
{
    Q_OBJECT

public:   
    //Constructor/Destructor
    GUI_Main_xxxx_DateDialog(Core* C, const std::string &FileName_, const std::string &Field_, const QString &Value, QWidget* parent=NULL);

    //Widgets
    QTabWidget*         Central;
    QWidget*            Central_Calendar;
    QGridLayout*        Central_Calendar_Layout;
    QCalendarWidget*    Calendar;
    QCheckBox*          TimeEdit_Activated;
    QTimeEdit*          TimeEdit;
    QTextEdit*          TextEdit;
    QLabel*             Label;
    QDialogButtonBox*   Dialog;
    QPushButton*        Date;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();
    void OnCalendar_Changed         ();
    void OnMenu_Date                ();
    void OnMenu_Calendar            ();
    void OnMenu_Text                ();
    void OnTextChanged              ();
    void OnCurrentChanged           (int Index);
    void OnTimeActivatedChanged     (int State);

private:
    //Internal
    Core* C;
    std::string FileName;
    std::string Field;
};

#endif
