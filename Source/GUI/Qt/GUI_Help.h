// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_HelpH
#define GUI_HelpH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <QDialog>
class QTabWidget;
class QPushButton;
class QUrl;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main
//***************************************************************************

class GUI_Help : public QDialog
{
    Q_OBJECT

public:
    GUI_Help (QWidget * parent);

private:
    //GUI
    QTabWidget*     Central;
    QPushButton*    Close;

 private Q_SLOTS:
    void OnAnchorClicked(const QUrl &link);
};

#endif
