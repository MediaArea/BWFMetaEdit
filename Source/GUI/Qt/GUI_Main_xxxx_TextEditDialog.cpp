// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_TextEditDialog::GUI_Main_xxxx_TextEditDialog(Core* _C, const std::string &FileName_, const std::string &Field_, const QString &Value, bool ReadOnly_, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;
    Field=Field_;
    ReadOnly=ReadOnly_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle(QString::fromUtf8(Field.c_str()));
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    QDialogButtonBox::StandardButtons Buttons=QDialogButtonBox::Ok;
    if (!ReadOnly)
        Buttons|=QDialogButtonBox::Cancel;

    Load=new QPushButton("&Import file...");
    Save=new QPushButton("&Export file...");
    Dialog=new QDialogButtonBox(Buttons, Qt::Horizontal, this);
    if (!ReadOnly)
        Dialog->addButton(Load, QDialogButtonBox::ResetRole);
    Dialog->addButton(Save, QDialogButtonBox::ResetRole);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    connect(Load, SIGNAL(clicked()), this, SLOT(OnMenu_Load()));
    connect(Save, SIGNAL(clicked()), this, SLOT(OnMenu_Save()));

    TextEdit=new QTextEdit(this);
    connect(TextEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));

    Label=new QLabel(this);
    Label->setOpenExternalLinks(true);

    BigMessage=new QLabel(this);

    Message=new QLabel(this);
    Message->setOpenExternalLinks(true);
    
    QVBoxLayout* L=new QVBoxLayout();
    L->addWidget(TextEdit);
    L->addWidget(Label);
    L->addWidget(BigMessage);
    L->addWidget(Message);
    L->addWidget(Dialog);

    setLayout(L);

    if (Field=="XMP")
        Message->setText("<html><body>This tool does not validate the contents of the XML chunks against the rules for XMP.<br />Edit at your own risk. For more information see the <a href=\"http://www.adobe.com/products/xmp/\">Adobe XMP website</a><br />Edits to this chunk can not be undone</body></html>");
    else if (Field=="aXML")
        Message->setText("<html><body>This tool does not validate the contents of the XML chunks against the rules for aXML.<br />Edit at your own risk. For more information see the <a href=\"http://tech.ebu.ch/docs/tech/tech3285s5.pdf\">BWF aXML chunk specification</a><br />Edits to this chunk can not be undone</body></html>");
    else if (Field=="iXML")
        Message->setText("<html><body>This tool does not validate the contents of the XML chunks  against the rules for iXML.<br />Edit at your own risk. For more information see the <a href=\"http://www.gallery.co.uk/ixml/\">iXML Specification</a><br />Edits to this chunk can not be undone</body></html>");

    Display(Value);
    TextEdit->setFocus();

}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_TextEditDialog::OnAccept ()
{
    std::string Value=(BigValue.isEmpty()?TextEdit->toPlainText():BigValue).toUtf8().data();
    if (!C->IsValid(FileName, Field, Value, true))
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Ok);
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        MessageBox.exec();
        return;
    }

    C->Set(FileName, Field, Value);

    accept();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_TextEditDialog::OnTextChanged ()
{
    std::string Value=(BigValue.isEmpty()?TextEdit->toPlainText():BigValue).toUtf8().data();
    if (!C->IsValid(FileName, Field, Value, true))
    {
        Label->setText(QString::fromUtf8(C->IsValid_LastError(FileName).c_str()));
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        Label->setWordWrap(true);
        Label->setText(C->IsValid_LastWarning(FileName).c_str());
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_TextEditDialog::OnMenu_Load()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getOpenFileName(  this,
                                                        tr("Import file..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        (Field=="XMP" || Field=="aXML" || Field=="iXML")?"XML files (*.xml);;All files (*.*)":"Text files (*.txt);;All files (*.*)");
    
    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Open(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;
    int64u F_Size=F.Size_Get();
    if (F_Size>((size_t)-1)-1)
        return;

    //Creating buffer
    int8u* Buffer=new int8u[(size_t)F_Size+1];
    size_t Buffer_Offset=0;

    //Reading the file
    while(Buffer_Offset<F_Size)
    {
        size_t BytesRead=F.Read(Buffer+Buffer_Offset, (size_t)F_Size-Buffer_Offset);
        if (BytesRead==0)
            break; //Read is finished
        Buffer_Offset+=BytesRead;
    }
    if (Buffer_Offset<F_Size)
        return;

    //Filling
    string ModifiedContent((const char*)Buffer, Buffer_Offset);
    delete[] Buffer;
    AdaptEOL(ModifiedContent, adapt_n);
    QString ModifiedContentQ=ModifiedContent.c_str();

    Display(ModifiedContentQ);
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_TextEditDialog::OnMenu_Save()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Export file..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        (Field=="XMP" || Field=="aXML" || Field=="iXML")?"XML files (*.xml);;All files (*.*)":"Text files (*.txt);;All files (*.*)");
    
    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Create(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;

    //Filling
    std::string Value=(BigValue.isEmpty()?TextEdit->toPlainText():BigValue).toUtf8().data();
    F.Write((int8u*)Value.c_str(), Value.size());
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_TextEditDialog::Display(const QString& Value)
{
    static const size_t MaxSize=0x100000;
    if (Value.size()<MaxSize)
    {
        BigValue.clear();
        BigMessage->setText(QString());
        TextEdit->setReadOnly(ReadOnly);
        TextEdit->setPlainText(Value);
        QTextCursor Cursor=TextEdit->textCursor(); Cursor.setPosition(Value.length());
        TextEdit->setTextCursor(Cursor);
    }
    else
    {
        BigValue=Value;
        BigMessage->setText("The content is too large to be displayed in full here and can not be edited here.");
        TextEdit->setReadOnly(true);
        TextEdit->setPlainText(Value.mid(0, MaxSize>>4));
    }
}
