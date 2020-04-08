// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_About.h"
#include "Common/Core.h"
#include <QTextEdit>
#include <QTextBrowser>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QImage>
#include <QLabel>
#include <QDialogButtonBox>
#include "Common/Common_About.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
GUI_About::GUI_About(QWidget * parent)
: QDialog(parent)
{
    #ifdef __WINDOWS__
        resize(640, 440);
    #else
        resize(840, 540);
    #endif
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("About BWF MetaEdit");

    Close=new QPushButton("&Close");
    Close->setDefault(true);
    QDialogButtonBox* Dialog=new QDialogButtonBox();
    Dialog->addButton(Close, QDialogButtonBox::AcceptRole);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(close()));

    QGridLayout* L=new QGridLayout();
    QVBoxLayout* L00=new QVBoxLayout();
    Button_Description=new QPushButton("&Description", this);
    Button_AuthorLicense=new QPushButton("&Author and License", this);
    Button_Contact=new QPushButton("&Contact", this);
    L00->addWidget(Button_Description);
    L00->addWidget(Button_AuthorLicense);
    L00->addWidget(Button_Contact);
    QPixmap* A=new QPixmap(QPixmap(":/Image/Logo/Logo.png").scaledToWidth(160));
    QLabel* B=new QLabel();
    B->setPixmap(*A);
    L00->addWidget(B);
    L->addLayout(L00, 0, 0, 3, 1);
    NameVersion=new QTextBrowser();
    NameVersion->setReadOnly(true);
    NameVersion->setOpenExternalLinks(true);
    NameVersion->setHtml(NameVersion_HTML());
    L->addWidget(NameVersion, 0, 1, 1, 2);
    Text=new QTextBrowser();
    Text->setReadOnly(true);
    Text->setOpenExternalLinks(true);
    L->addWidget(Text, 1, 1, 1, 2);
    L->addWidget(Close, 2, 2);

    connect(Button_Description, SIGNAL(clicked()), this, SLOT(OnDescription()));
    connect(Button_AuthorLicense, SIGNAL(clicked()), this, SLOT(OnAuthorLicense()));
    connect(Button_Contact, SIGNAL(clicked()), this, SLOT(OnContact()));
    setLayout(L);

    OnDescription();
}

//---------------------------------------------------------------------------
GUI_About::~GUI_About()
{
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_About::OnDescription()
{
    Text->clear();
    Text->setHtml(Description_HTML());
    Button_Description->setEnabled(false);
    Button_AuthorLicense->setEnabled(true);
    Button_Contact->setEnabled(true);
}

//---------------------------------------------------------------------------
void GUI_About::OnAuthorLicense()
{
    Text->clear();
    Text->setSource(QUrl("qrc:/License.html"));
    Button_Description->setEnabled(true);
    Button_AuthorLicense->setEnabled(false);
    Button_Contact->setEnabled(true);
}

//---------------------------------------------------------------------------
void GUI_About::OnContact()
{
    Text->clear();
    Text->setHtml(Contact_HTML());
    Button_Description->setEnabled(true);
    Button_AuthorLicense->setEnabled(true);
    Button_Contact->setEnabled(false);
}

void GUI_About::showEvent(QShowEvent* Event)
{
    NameVersion->setBackgroundRole(QPalette::Window);
    resizeEvent(NULL);
}

void GUI_About::resizeEvent(QResizeEvent* Event)
{
    NameVersion->setMaximumHeight((int)NameVersion->document()->documentLayout()->documentSize().height()+6);
}
