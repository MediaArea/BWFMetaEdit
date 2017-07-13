// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_AboutH
#define GUI_AboutH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <QDialog>
class QTextBrowser;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main
//***************************************************************************

class GUI_About : public QDialog
{
     Q_OBJECT

public:
    GUI_About (QWidget * parent);
    ~GUI_About ();

private Q_SLOTS:
    //Menu - Actions
    void OnDescription                  ();
    void OnAuthorLicense                ();
    void OnContact                      ();

private:
    //GUI
    QTextBrowser* NameVersion;
    QTextBrowser* Text;
    QTextBrowser* Websites;
    QPushButton*  Button_Description;
    QPushButton*  Button_AuthorLicense;
    QPushButton*  Button_Contact;
    QPushButton*  Close;

    void showEvent(QShowEvent* Event);
    void resizeEvent(QResizeEvent* Event);
};

#endif
