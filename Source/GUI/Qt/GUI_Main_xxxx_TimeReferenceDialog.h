// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_TimeReferenceDialogH
#define GUI_Main_xxxx_TimeReferenceDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QtGui/QDialog>
#include <string>
#include <ZenLib/Conf.h>
class QTimeEdit;
class QLabel;
class QLineEdit;
class QTime;
class QEvent;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_TimeReferenceDialog
//***************************************************************************

class GUI_Main_xxxx_TimeReferenceDialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_TimeReferenceDialog(Core* C, const std::string &FileName_, const std::string &Field_, QWidget* parent=NULL);

    //Widgets
    QTimeEdit*          TimeEdit;
    QLabel*             TimeEdit_Label;
    QLabel*             Label;
    QLineEdit*          LineEdit;
    QLabel*             LimeEdit_Label;
    QPushButton*        Cancel;
    QPushButton*        Valid;

private slots:
    //Actions
    void OnAccept                   ();
    void OnTimeChanged              (const QTime &Time);
    void OnValueEdited              (const QString &Value);

private:
    //Internal
    Core* C;
    std::string FileName;
    std::string Field;
    ZenLib::int64u  SampleRate;
    bool            IsChanging;
};

#endif
