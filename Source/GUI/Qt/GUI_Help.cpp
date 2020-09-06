// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Help.h"
#include "Common/Core.h"
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QDesktopServices>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
GUI_Help::GUI_Help(QWidget * parent)
: QDialog(parent)
{
    move(QApplication::desktop()->screenGeometry().width()/10, QApplication::desktop()->screenGeometry().height()/10);
    resize(QApplication::desktop()->screenGeometry().width()-QApplication::desktop()->screenGeometry().width()/10*6, QApplication::desktop()->screenGeometry().height()/2);

    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("BWF MetaEdit help");

    Close=new QPushButton("&Close");
    Close->setDefault(true);
    QDialogButtonBox* Dialog=new QDialogButtonBox();
    Dialog->addButton(Close, QDialogButtonBox::AcceptRole);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(close()));

    QVBoxLayout* L=new QVBoxLayout();
    Central=new QTabWidget(this);
    QTextBrowser* Text1=new QTextBrowser(this);
    Text1->setReadOnly(true);
    Text1->setOpenExternalLinks(true);
    Text1->setSource(QUrl("qrc:/Documentation/index.html"));
    #if (QT_VERSION >= 0x040300)
        Text1->setOpenLinks(false);
    #endif // (QT_VERSION >= 0x040300)
    connect(Text1, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text1, tr("Main"));
    QTextBrowser* Text2=new QTextBrowser(this);
    Text2->setReadOnly(true);
    Text2->setOpenExternalLinks(true);
    Text2->setSource(QUrl("qrc:/Documentation/tech_view_help.html"));
    connect(Text2, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text2, tr("TECH"));
    QTextBrowser* Text3=new QTextBrowser(this);
    Text3->setReadOnly(true);
    Text3->setOpenExternalLinks(true);
    Text3->setSource(QUrl("qrc:/Documentation/core_doc_help.html"));
    connect(Text3, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text3, tr("CORE"));
    QTextBrowser* Text4=new QTextBrowser(this);
    Text4->setReadOnly(true);
    Text4->setOpenExternalLinks(true);
    Text4->setSource(QUrl("qrc:/Documentation/trace.html"));
    connect(Text4, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text4, tr("Trace"));
    QTextBrowser* Text5=new QTextBrowser(this);
    Text5->setReadOnly(true);
    Text5->setOpenExternalLinks(true);
    Text5->setSource(QUrl("qrc:/Documentation/errors.html"));
    connect(Text5, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text5, tr("Errors"));
    QTextBrowser* Text6=new QTextBrowser(this);
    Text6->setReadOnly(true);
    Text6->setOpenExternalLinks(true);
    Text6->setSource(QUrl("qrc:/Documentation/bext.html"));
    connect(Text6, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text6, tr("BEXT metadata"));
    QTextBrowser* Text7=new QTextBrowser(this);
    Text7->setReadOnly(true);
    Text7->setOpenExternalLinks(true);
    Text7->setSource(QUrl("qrc:/Documentation/listinfo.html"));
    connect(Text7, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text7, tr("LIST-INFO metadata"));
    QTextBrowser* Text8=new QTextBrowser(this);
    Text8->setReadOnly(true);
    Text8->setOpenExternalLinks(true);
    Text8->setSource(QUrl("qrc:/Documentation/xml_chunks.html"));
    connect(Text8, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text8, tr("XML chunks"));
    QTextBrowser* Text9=new QTextBrowser(this);
    Text9->setReadOnly(true);
    Text9->setOpenExternalLinks(true);
    Text9->setSource(QUrl("qrc:/Documentation/options.html"));
    connect(Text9, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text9, tr("Options"));
    QTextBrowser* Text10=new QTextBrowser(this);
    Text10->setReadOnly(true);
    Text10->setOpenExternalLinks(true);
    Text10->setSource(QUrl("qrc:/Documentation/md5.html"));
    connect(Text10, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text10, tr("Checksums"));
    QTextBrowser* Text11=new QTextBrowser(this);
    Text11->setReadOnly(true);
    Text11->setOpenExternalLinks(true);
    Text11->setSource(QUrl("qrc:/Documentation/validation_rules_help.html"));
    connect(Text11, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text11, tr("Validation rules"));
    QTextBrowser* Text12=new QTextBrowser(this);
    Text12->setReadOnly(true);
    Text12->setOpenExternalLinks(true);
    Text12->setSource(QUrl("qrc:/Documentation/workflows.html"));
    connect(Text12, SIGNAL(anchorClicked (const QUrl &)), this, SLOT(OnAnchorClicked(const QUrl &)));
    Central->addTab(Text12, tr("Workflows"));
    L->addWidget(Central);
    L->addWidget(Close);

    setLayout(L);
}

void GUI_Help::OnAnchorClicked(const QUrl &link)
{
         if (link==QUrl("qrc:/Documentation/index.html"))
        Central->setCurrentIndex(0);
    else if (link==QUrl("qrc:/Documentation/tech_view_help.html"))
        Central->setCurrentIndex(1);
    else if (link==QUrl("qrc:/Documentation/core_doc_help.html"))
        Central->setCurrentIndex(2);
    else if (link==QUrl("qrc:/Documentation/trace.html"))
        Central->setCurrentIndex(3);
    else if (link==QUrl("qrc:/Documentation/errors.html"))
        Central->setCurrentIndex(4);
    else if (link==QUrl("qrc:/Documentation/bext.html"))
        Central->setCurrentIndex(5);
    else if (link==QUrl("qrc:/Documentation/listinfo.html"))
        Central->setCurrentIndex(6);
    else if (link==QUrl("qrc:/Documentation/xml_chunks.html"))
        Central->setCurrentIndex(7);
    else if (link==QUrl("qrc:/Documentation/options.html"))
        Central->setCurrentIndex(8);
    else if (link==QUrl("qrc:/Documentation/md5.html"))
        Central->setCurrentIndex(9);
    else if (link==QUrl("qrc:/Documentation/validation_rules_help.html"))
        Central->setCurrentIndex(10);
    else if (link==QUrl("qrc:/Documentation/workflows.html"))
        Central->setCurrentIndex(11);
    else
        QDesktopServices::openUrl(link);
}
