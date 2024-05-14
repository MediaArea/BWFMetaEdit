// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_CueDialog.h"
#include "Common/Codes.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "TinyXml2/tinyxml2.h"
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include <QLabel>
#include <QApplication>
#include <QMessageBox>
#include <QFontMetrics>
#include <QDesktopWidget>
//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
enum columns
{
    Column_Close=0,
    Column_Id,
    Column_Start,
    Column_Length,
    Column_Label,
    Column_Note,
    Column_PurposeId,
    Column_Country,
    Column_Language,
    Column_Dialect,
    Column_CodePage,
    Column_LabeledText,
    Column_Max
};

//---------------------------------------------------------------------------
string CodeToCountry(int16u Code)
{
    string ToReturn;
    if (IsISOCountry(Code))
    {
        ToReturn+=(char)((Code&0xFF00)>>8);
        ToReturn+=(char)((Code&0x00FF));
    }
    else
       ToReturn+=QString().number(Code).toStdString();
    ToReturn+=" - ";
    ToReturn+=Country_Get(Code);

    return ToReturn;
}

//---------------------------------------------------------------------------
string CodeToLanguage(int16u Code)
{
    string ToReturn;
    if (IsISOLanguage(Code))
    {
        ToReturn+=(char)((Code&0xFF00)>>8);
        ToReturn+=(char)((Code&0x00FF));
    }
    else
       ToReturn+=QString().number(Code).toStdString();
    ToReturn+=" - ";
    ToReturn+=Language_Get(Code);

    return ToReturn;
}

//---------------------------------------------------------------------------
string CodeToDialect(int16u Language, int16u Code)
{
    string ToReturn;
    ToReturn+=QString().number(Code).toStdString();
    ToReturn+=" - ";
    ToReturn+=Dialect_Get(Language, Code);

    return ToReturn;
}

//---------------------------------------------------------------------------
string SamplesToTimeStamp(int64u Samples, int32u Rate)
{
    string ToReturn;
    ToReturn+=QString().number(Samples).toStdString();

    if (!Rate)
        return ToReturn;

    ToReturn+=" (";
    ToReturn+=Ztring().Duration_From_Milliseconds((int64u)((float64)Samples*1000/Rate)).To_UTF8();
    ToReturn+=")";

    return ToReturn;
}

//---------------------------------------------------------------------------
string HexToCC4(string Id)
{
    Ztring Value=Ztring().From_UTF8(Id);
    if (Value.size()>2 && Value.find(__T("0x"))==0)
        Value.erase(0, 2);

    if (Value.To_int32u(16)==0)
        return "";

    return Ztring().From_CC4(Value.To_int32u(16)).To_UTF8();
}

//---------------------------------------------------------------------------
string CC4ToHex(string Id)
{
    if (Id.empty())
        return "0x00000000";
    if (Id.size()!=4)
        return Id;

    int32u Value=Ztring().From_UTF8(Id).To_CC4();
    return Ztring(__T("0x")+Ztring().From_Number(Value, 16)).To_UTF8();
}


//***************************************************************************
// CueDialog_SampleRateDialog
//***************************************************************************

//---------------------------------------------------------------------------
CueDialog_SampleRateDialog::CueDialog_SampleRateDialog(int32u Value, int32u SampleRate, QWidget* parent)
: QDialog(parent), SampleRate(SampleRate)
{
    //Internal
    IsChanging=false;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("Edit");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    QDialogButtonBox* Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));

    TimeEdit=new QTimeEdit(this);
    TimeEdit->setDisplayFormat("hh:mm:ss.zzz");
    connect(TimeEdit, SIGNAL(timeChanged (const QTime&)), this, SLOT(OnTimeChanged (const QTime&)));
    QLabel* TimeEdit_Label=new QLabel(this);
    TimeEdit_Label->setText(tr("(HH:MM:SS.mmm)"));
    QLabel* Label=new QLabel(this);
    Label->setText(tr("or"));
    LineEdit=new QDoubleSpinBox(this);
    LineEdit->setMinimum(0);
    LineEdit->setMaximum(0xFFFFFFFF);
    LineEdit->setDecimals(0);
    connect(LineEdit, SIGNAL(valueChanged (const QString& )), this, SLOT(OnValueEdited (const QString&)));
    QLabel* LineEdit_Label=new QLabel(this);
    LineEdit_Label->setText(tr("(in samples)"));

    QGridLayout* L=new QGridLayout();
    L->addWidget(TimeEdit, 0, 0);
    L->addWidget(TimeEdit_Label, 0, 1);
    L->addWidget(Label, 1, 0, 1, 2);
    L->addWidget(LineEdit, 2, 0);
    L->addWidget(LineEdit_Label, 2, 1);
    L->addWidget(Dialog, 3, 0, 1, 2);

    setLayout(L);

    //Filling
    LineEdit->setValue(Value);
}

//---------------------------------------------------------------------------
void CueDialog_SampleRateDialog::OnTimeChanged(const QTime& Time)
{
    if (IsChanging || SampleRate==0)
        return;

    LineEdit->setValue(float64_int64s(((double)QTime(0, 0, 0, 0).msecsTo(Time))*SampleRate/1000));
}

//---------------------------------------------------------------------------
void CueDialog_SampleRateDialog::OnValueEdited(const QString&)
{
    if (IsChanging || SampleRate==0)
        return;

    int64s TimeReference=(int64s)LineEdit->value();
    TimeReference=float64_int64s(((float64)TimeReference)/(((float64)SampleRate)/1000));
    IsChanging=true;
    if (TimeReference>23*60*60*1000+59*60*1000+59*1000+999)
    {
        TimeEdit->setEnabled(false);
        TimeEdit->setTime(QTime(0, 0, 0, 0).addMSecs(23*60*60*1000+59*60*1000+59*1000+999));
    }
    else
    {
        TimeEdit->setEnabled(true);
        TimeEdit->setTime(QTime(0, 0, 0, 0).addMSecs((int)TimeReference));
    }
    IsChanging=false;
}

//---------------------------------------------------------------------------
int32u CueDialog_SampleRateDialog::GetValue()
{
    return (int32u)LineEdit->value();
}

//***************************************************************************
// Validators
//***************************************************************************

//---------------------------------------------------------------------------
CC4Validator::CC4Validator(QObject* parent) : QValidator(parent)
{
}

//---------------------------------------------------------------------------
QValidator::State CC4Validator::validate(QString& input, int&) const
{
    if (input.isEmpty())
        return QValidator::Acceptable;

    if (input.size()>4)
        return QValidator::Invalid;

    string value=input.toStdString();
    for (size_t pos=0; pos<value.size(); pos++)
        if (value.at(pos)<0x20)
            return QValidator::Invalid;

    if (input.size()==4)
        return QValidator::Acceptable;

    return QValidator::Intermediate;
}

//---------------------------------------------------------------------------
IDValidator::IDValidator(QModelIndex index, QObject* parent) : QValidator(parent), index(index)
{
}

//---------------------------------------------------------------------------
QValidator::State IDValidator::validate(QString& input, int&) const
{
    if (input.isEmpty())
        return QValidator::Acceptable;

    long value;
    bool ok;
    value=input.toLong(&ok);

    if (!ok || value>0xFFFFFFFF)
        return QValidator::Invalid;

    int pos=0;
    QModelIndex sibling;
    while ((sibling=index.sibling(pos, Column_Id)).isValid())
    {
        pos++;
        if (sibling.row()==index.row())
            continue;

        long siblingValue=sibling.data().toString().toLong(&ok);
        if (ok && siblingValue==value)
            return QValidator::Intermediate;
    }

    return QValidator::Acceptable;
}

//---------------------------------------------------------------------------
CountryValidator::CountryValidator(QObject* parent) : QValidator(parent)
{
}

//---------------------------------------------------------------------------
QValidator::State CountryValidator::validate(QString& input, int& pos) const
{
    if (input.isEmpty())
        return QValidator::Acceptable;

    if (QIntValidator(0, 0xFFFF).validate(input, pos)==QValidator::Acceptable)
        return QValidator::Acceptable;

    if (input.size()==2)
    {
        input=input.toUpper();
        if (IsISOCountry(CC2(input.toStdString().c_str())))
            return QValidator::QValidator::Acceptable;
        else
            return QValidator::Invalid;
    }
    else if (input.size()>2)
        return QValidator::Invalid;

    return QValidator::Intermediate;
}

//---------------------------------------------------------------------------
LanguageValidator::LanguageValidator(QObject* parent) : QValidator(parent)
{
}

//---------------------------------------------------------------------------
QValidator::State LanguageValidator::validate(QString& input, int& pos) const
{
    if (input.isEmpty())
        return QValidator::Acceptable;

    if (QIntValidator(0, 0xFFFF).validate(input, pos)==QValidator::Acceptable)
        return QValidator::Acceptable;

    if (input.size()==2)
    {
        input=input.toLower();
        if (IsISOLanguage(CC2(input.toStdString().c_str())))
            return QValidator::QValidator::Acceptable;
        else
            return QValidator::Invalid;
    }
    else if (input.size()>2)
        return QValidator::Invalid;

    return QValidator::Intermediate;
}

//***************************************************************************
// CueDialogEdit_Delegate
//***************************************************************************

//---------------------------------------------------------------------------
QWidget* CueDialogEdit_Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* Editor=new QLineEdit(parent);
    Editor->installEventFilter(const_cast<CueDialogEdit_Delegate*>(this));

    return Editor;
}

//---------------------------------------------------------------------------
void CueDialogEdit_Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString Value=index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* Editor=static_cast<QLineEdit*>(editor);

    if (index.column()==Column_Id)
        Editor->setValidator(new IDValidator(index));
    else if (index.column()==Column_CodePage)
        Editor->setValidator(new QIntValidator(0, 0xFFFF));

    Editor->setText(Value);
}

//---------------------------------------------------------------------------
void CueDialogEdit_Delegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* Editor=static_cast<QLineEdit*>(editor);
    QString Value=Editor->text();

    model->setData(index, Value, Qt::DisplayRole);
}

//---------------------------------------------------------------------------
void CueDialogEdit_Delegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

//***************************************************************************
// CueDialogCombo_Delegate
//***************************************************************************

//---------------------------------------------------------------------------
QWidget* CueDialogCombo_Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QComboBox* Editor=new QComboBox(parent);
    Editor->setEditable(true);
    Editor->installEventFilter(const_cast<CueDialogCombo_Delegate*>(this));

    return Editor;
}

//---------------------------------------------------------------------------
void CueDialogCombo_Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString Value=index.model()->data(index, Qt::EditRole).toString();
    QComboBox* Editor=static_cast<QComboBox*>(editor);

    int MinWidth=0;
    if (index.column()==Column_PurposeId)
    {
        Editor->addItem("");
        Editor->addItem("spea", QString("spea"));
        Editor->addItem("envi", QString("envi"));
        Editor->addItem("note", QString("note"));
        Editor->addItem("tran", QString("tran"));
        Editor->addItem("othr", QString("othr"));
        MinWidth=Editor->fontMetrics().maxWidth()*4;
        Editor->setValidator(new CC4Validator());
    }
    if (index.column()==Column_Country)
    {
        Value=index.model()->data(index, Qt::UserRole+1).toString();

        Editor->addItem("");
        int Width=Editor->fontMetrics().maxWidth()*4;
        const codes* CSETCountries=CSETCountries_Get();
        for (codes::const_iterator It=CSETCountries->begin(); It!=CSETCountries->end(); It++)
        {
            if(!It->second.second)
                continue;
            QString Entry=QString().fromStdString(CodeToCountry(It->first));
            MinWidth=max(MinWidth, Editor->fontMetrics().size(Qt::TextSingleLine, Entry).width()+20);
            Editor->addItem(Entry, QString::number(It->first));
        }

        Editor->insertSeparator(Editor->count()-1);

        const codes* ISOCountries=ISOCountries_Get();
        for (codes::const_iterator It=ISOCountries->begin(); It!=ISOCountries->end(); It++)
        {
            if(!It->second.second)
                continue;
            QString Entry=QString().fromStdString(CodeToCountry(It->first));
            MinWidth=max(MinWidth, Editor->fontMetrics().size(Qt::TextSingleLine, Entry).width()+20);

            string Alpha2;
            Alpha2+=(char)((It->first&0xFF00)>>8);
            Alpha2+=(char)((It->first&0x00FF));

            Editor->addItem(Entry, QString().fromStdString(Alpha2));
        }

        int16u Code=Value.toUShort();
        if (IsISOCountry(Code))
        {
          Value=QString();
          Value.append(QChar((char)((Code&0xFF00)>>8)));
          Value.append(QChar((char)((Code&0x00FF))));
        }

        Editor->setValidator(new CountryValidator());
    }
    else if (index.column()==Column_Language)
    {
        Value=index.model()->data(index, Qt::UserRole+1).toString();

        Editor->addItem("");
        const codes* CSETLanguages=CSETLanguages_Get();
        for (codes::const_iterator It=CSETLanguages->begin(); It!=CSETLanguages->end(); It++)
        {
            QString Entry=QString().fromStdString(CodeToLanguage(It->first));
            MinWidth=max(MinWidth, Editor->fontMetrics().size(Qt::TextSingleLine, Entry).width()+20);
            Editor->addItem(Entry, QString::number(It->first));
        }

        Editor->insertSeparator(Editor->count()-1);

        const codes* ISOLanguages=ISOLanguages_Get();
        for (codes::const_iterator It=ISOLanguages->begin(); It!=ISOLanguages->end(); It++)
        {
            QString Entry=QString().fromStdString(CodeToLanguage(It->first));
            MinWidth=max(MinWidth, Editor->fontMetrics().size(Qt::TextSingleLine, Entry).width()+20);

            string Alpha2;
            Alpha2+=(char)((It->first&0xFF00)>>8);
            Alpha2+=(char)((It->first&0x00FF));

            Editor->addItem(Entry, QString().fromStdString(Alpha2));
        }

        int16u Code=Value.toUShort();
        if (IsISOLanguage(Code))
        {
          Value=QString();
          Value.append(QChar((char)((Code&0xFF00)>>8)));
          Value.append(QChar((char)((Code&0x00FF))));
        }
        Editor->setValidator(new LanguageValidator());
    }
    else if (index.column()==Column_Dialect)
    {
        Value=index.model()->data(index, Qt::UserRole+1).toString();
        int16u Language=index.model()->data(index.sibling(index.row(), Column_Language), Qt::UserRole+1).toInt();

        Editor->addItem("");
        const codes* Dialects=CSETDialects_Get();

        for (codes::const_iterator It=Dialects->begin(); It!=Dialects->end(); It++)
        {
            if (It->first && It->first/10==Language)
            {
                QString Entry=QString().fromStdString(CodeToDialect(Language, It->first%10));
                MinWidth=max(MinWidth, Editor->fontMetrics().size(Qt::TextSingleLine, Entry).width()+20);
                Editor->addItem(Entry, QString::number(It->first%10));
            }
        }
        Editor->setValidator(new QIntValidator(0, 0xFFFF));
    }
    Editor->setStyleSheet(QString("QComboBox QAbstractItemView {min-width: %1;}").arg(MinWidth));

    bool Found=false;
    for (int Pos=0; Pos<Editor->count(); Pos++)
        if (Value==Editor->itemData(Pos).toString())
         {
            Found=true;
            Editor->setCurrentIndex(Pos);
         }

    if (!Value.isEmpty() && !Found)
        Editor->setCurrentText(Value);
}

//---------------------------------------------------------------------------
void CueDialogCombo_Delegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* Editor=static_cast<QComboBox*>(editor);

    QString Value;
    if (Editor->currentData().isValid())
        Value=Editor->currentData().toString();
    else
        Value=Editor->currentText();

    if (index.column()==Column_PurposeId)
    {
        model->setData(index, QString().fromStdString(CC4ToHex(Value.toStdString())), Qt::UserRole+1);
        model->setData(index, QString().fromStdString(Value.toStdString()), Qt::DisplayRole);
    }
    else if (index.column()==Column_Country)
    {
        if (Value.isEmpty())
            Value="0";

        bool Ok=false;
        int16u Code=Value.toUShort(&Ok, 10);
        if (!Ok)
        {
            Code=CC2(Value.toStdString().c_str());
            Value=QString::number(Code, 10);
        }
        model->setData(index, Value, Qt::UserRole+1);
        model->setData(index, QString().fromStdString(CodeToCountry(Code)), Qt::DisplayRole);
    }
    else if (index.column()==Column_Language)
    {
        if (Value.isEmpty())
            Value="0";

        bool Ok=false;
        int16u Code=Value.toUShort(&Ok, 10);
        if (!Ok)
        {
            Code=CC2(Value.toStdString().c_str());
            Value=QString::number(Code, 10);
        }
        model->setData(index, Value, Qt::UserRole+1);
        model->setData(index, QString().fromStdString(CodeToLanguage(Code)), Qt::DisplayRole);
        if (IsCSETLanguage(Code))
        {
            model->setData(index.sibling(index.row(), Column_Dialect), QString("1"), Qt::UserRole+1);
            model->setData(index.sibling(index.row(), Column_Dialect), QString(CodeToDialect(Code, 1).c_str()), Qt::DisplayRole);
        }
        else if (IsISOLanguage(Code))
        {
            model->setData(index.sibling(index.row(), Column_Dialect), QString("0"), Qt::UserRole+1);
            model->setData(index.sibling(index.row(), Column_Dialect), QString(CodeToDialect(Code, 0).c_str()), Qt::DisplayRole);
        }
    }
    else if (index.column()==Column_Dialect)
    {
        if (Value.isEmpty())
            Value="0";

        int16u Language=model->data(index.sibling(index.row(), Column_Language), Qt::UserRole+1).toInt();
        int16u Code=Value.toUShort();
        model->setData(index, Value, Qt::UserRole+1);
        model->setData(index, QString().fromStdString(CodeToDialect(Language, Code)), Qt::DisplayRole);
    }
}

//---------------------------------------------------------------------------
void CueDialogCombo_Delegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

//***************************************************************************
// CueDialog_TableWidget
//***************************************************************************

//---------------------------------------------------------------------------
CueDialog_TableWidget::CueDialog_TableWidget(Core* C, const string& FileName, bool ReadOnly, QWidget* parent)
: QTableWidget(parent), C(C), FileName(FileName), ReadOnly(ReadOnly)
{
    SampleRate=Ztring().From_UTF8(C->Get(FileName, "SampleRate")).To_int32u();
}

//---------------------------------------------------------------------------
bool CueDialog_TableWidget::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event)
{
    if (index.column()==Column_Close)
        return false;

    if ((trigger==DoubleClicked || trigger==AnyKeyPressed) && (index.column()==Column_Start || index.column()==Column_Length) && !ReadOnly)
    {
        //User interaction
        CueDialog_SampleRateDialog* Edit=new CueDialog_SampleRateDialog(index.model()->data(index, Qt::UserRole+1).toUInt(), SampleRate);
        if (Edit->exec()==QDialog::Accepted)
        {
            int32u NewValue=(int32u)Edit->GetValue();
            model()->setData(index, NewValue, Qt::UserRole+1);
            model()->setData(index, QString().fromStdString(SamplesToTimeStamp(NewValue, SampleRate)), Qt::DisplayRole);
        }
        delete Edit; //Edit=NULL;
        return false;
    }
    return QTableWidget::edit(index, trigger, event);
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_CueDialog::GUI_Main_xxxx_CueDialog(Core* C, const string& FileName, bool ReadOnly_, QWidget* parent)
: QDialog(parent), C(C), FileName(FileName), ReadOnly(ReadOnly_)
{
    //Internal
    IsAccepted=false;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("Cue editor");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Messages
    Message=new QLabel(this);

    // DAVID/DigaSystem checkbox
    DavidMode=new QCheckBox("Use &DAVID/DigaSystem CUE point format.");

    //Central
    Table=new CueDialog_TableWidget(C, FileName, ReadOnly_, this);
    Table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    SampleRate=Ztring().From_UTF8(C->Get(FileName, "SampleRate")).To_int32u();
    BitsPerSample=Ztring().From_UTF8(C->Get(FileName, "BitsPerSample")).To_int16u();
    Xml=C->Get(FileName, "cuexml");
    Xml2List();

    //Buttons
    QDialogButtonBox::StandardButtons Buttons=QDialogButtonBox::Ok;
    if (!ReadOnly)
        Buttons|=QDialogButtonBox::Cancel;

    Dialog=new QDialogButtonBox(Buttons);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    QPushButton* Load=new QPushButton("&Load file...");
    QPushButton* Save=new QPushButton("&Save file...");
    if (!ReadOnly)
        Dialog->addButton(Load, QDialogButtonBox::ResetRole);
    Dialog->addButton(Save, QDialogButtonBox::ResetRole);
    connect(Save, SIGNAL(clicked()), this, SLOT(OnMenu_Save()));
    connect(Load, SIGNAL(clicked()), this, SLOT(OnMenu_Load()));

    connect(Table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(OnEdited(QTableWidgetItem*)));

    QGridLayout* L=new QGridLayout();
    L->addWidget(Table, 0, 0, 1, 2);
    if (!ReadOnly)
        L->addWidget(DavidMode, 1, 0, 1, 2);
    L->addWidget(Message, 2, 0, 1, 2);
    L->addWidget(Dialog, 3, 0, 1, 2);

    setLayout(L);

    Dialog->button(QDialogButtonBox::Ok)->setEnabled(ReadOnly || C->IsValid(FileName, "cuexml", Xml));
    resize(QApplication::desktop()->screenGeometry().width()/2, QApplication::desktop()->screenGeometry().height()/3);
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::OnCloseClicked ()
{
    QObject* Sender=sender();
    if (!Sender)
        return;

    for (int Row=0; Row<Table->rowCount(); Row++)
    {
        if (Table->cellWidget(Row, Column_Close)==Sender)
        {
            Table->removeRow(Row);
            break;
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::OnMenu_Save()
{
    //User interaction
    QString FileNameQ = QFileDialog::getSaveFileName(this, tr("Export file..."), "", "XML files (*.xml);;All files (*.*)");

    if (FileNameQ.isEmpty())
        return;

    File F;
    if (!F.Create(ZenLib::Ztring().From_UTF8(FileNameQ.toUtf8().data())))
        return;

    //Filling
    List2Xml();
    F.Write(Ztring().From_UTF8(Xml.c_str()));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::OnMenu_Load()
{
    //User interaction
    QString FileNameQ = QFileDialog::getOpenFileName(this, tr("Import file..."), "", "XML files (*.xml);;All files (*.*)");

    if (FileNameQ.isEmpty())
        return;

    File F;
    if (!F.Open(ZenLib::Ztring().From_UTF8(FileNameQ.toUtf8().data())))
        return;
    int64u F_Size=F.Size_Get();
    if (F_Size>((size_t)-1)-1)
        return;

    //Creating buffer
    int8u* Buffer=new int8u[(size_t)F_Size];
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

    if(C->IsValid(FileName, "cuexml", ModifiedContent), true)
    {
        Xml=ModifiedContent;
        Xml2List();
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(ReadOnly || C->IsValid(FileName, "cuexml", Xml));
    }
    else
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        MessageBox.setStandardButtons(QMessageBox::Ok);
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        MessageBox.exec();
    }
}


//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::OnAccept()
{
    IsAccepted=true;
    accept();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::OnEdited(QTableWidgetItem* Item)
{
    if (Updating)
        return;

    Updating=true;

    if (!Table->rowCount())
    {
        UpdateRows();
        Updating=false;
        return;
    }

    bool IsEmpty=true;
    for (int Pos=Column_Id; Pos<Column_Max; Pos++)
    {
        if (Table->item(Table->rowCount()-1, Pos) && !Table->item(Table->rowCount()-1, Pos)->text().isEmpty())
        {
            IsEmpty=false;
            break;
        }
    }

    if (!IsEmpty)
    {
        if (Table->cellWidget(Table->rowCount()-1, Column_Close)==NULL)
        {
            //Add delete button
            QIcon CloseIcon(":/Image/Menu/File_Close.svg");
            CloseIcon.addFile(":/Image/Menu/File_Close_Hovered.svg",  QSize(), QIcon::Active);

            QTableWidgetItem* Item=new QTableWidgetItem(""); //Create QTableWidgetItem to edit cell flags
            Item->setToolTip("Delete");
            Item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            Table->setItem(Table->rowCount()-1, Column_Close, Item);

            QPushButton* Close=new QPushButton(CloseIcon, "");
            Close->setFlat(true);
            Table->setCellWidget(Table->rowCount()-1, Column_Close, Close);
            connect(Close, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
        }

        UpdateRows();
    }

    C->IsValid(FileName, "cue_labl", Table->item(Item->row(), Column_Label)->text().toStdString());
    UpdateItem(Table->item(Item->row(), Column_Label));

    if (Item->column()==Column_PurposeId)
    {
        C->IsValid(FileName, "cue_ltxt_purpose", Item->data(Qt::UserRole+1).toString().toStdString());
        UpdateItem(Item);
    }
    else if (Item->column()==Column_Country)
    {
        C->IsValid(FileName, "cue_ltxt_country", Item->data(Qt::UserRole+1).toString().toStdString());
        UpdateItem(Item);
    }
    else if (Item->column()==Column_Language || Item->column()==Column_Dialect)
    {
        string Language=Table->item(Item->row(), Column_Language)->data(Qt::UserRole+1).toString().toStdString();
        string Dialect=Table->item(Item->row(), Column_Dialect)->data(Qt::UserRole+1).toString().toStdString();
        C->IsValid(FileName, "cue_ltxt_language", Language);
        UpdateItem(Table->item(Item->row(), Column_Language));
        C->IsValid(FileName, "cue_ltxt_language_dialect", Language + ";" + Dialect);
        UpdateItem(Table->item(Item->row(), Column_Dialect));
    }

    Table->resizeColumnToContents(Item->column());

    List2Xml();
    Dialog->button(QDialogButtonBox::Ok)->setEnabled(ReadOnly || C->IsValid(FileName, "cuexml", Xml));

    Updating=false;
}

//***************************************************************************
// Events
//***************************************************************************

void GUI_Main_xxxx_CueDialog::hideEvent (QHideEvent*)
{
    if (!IsAccepted || ReadOnly)
        return;

    List2Xml();
    C->Set(FileName, "cuexml", Xml);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::UpdateItem(QTableWidgetItem* Item)
{
    string LastError=C->IsValid_LastError(FileName);
    string LastWarning=C->IsValid_LastWarning(FileName);
    if (!LastError.empty())
    {
        Item->setIcon(QIcon(":/Image/Menu/Error.svg"));
        Item->setToolTip(QString("<qt>%1</qt>").arg(QString(LastError.c_str()).toHtmlEscaped()));
    }
    else if (!LastWarning.empty())
    {
        Item->setIcon(QIcon(":/Image/Menu/Warning.svg"));
        Item->setToolTip(QString("<qt>%1</qt>").arg(QString(LastWarning.c_str()).toHtmlEscaped()));
    }
    else
    {
        Item->setToolTip(QString());
        Item->setIcon(QIcon());
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::UpdateRows()
{
    if (!ReadOnly)
    {
        int Index=Table->rowCount();
        Table->setRowCount(Index+1);
        for (size_t Column=0; Column<Column_Max; Column++)
        {
            Table->setItem(Index, Column, new QTableWidgetItem(""));
            if (Column==Column_Start || Column==Column_Length)
                Table->item(Index, Column)-> setFlags( Table->item(Index, Column)->flags()&~Qt::ItemIsEditable);
        }
    }
    Table->resizeColumnsToContents();
    Table->horizontalHeader()->setStretchLastSection(true);

}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::Xml2List()
{
    //Unsuported
    bool DataChunkID_NotEqual_data=false;
    bool CodecID_NotEqual_PCM=false;
    bool Incoherent_Position=false;
    bool NonZero_ChunkStart=false;
    bool NonZero_BlockStart=false;

    if (C->Get(FileName, "CodecID")!="0001" && C->Get(FileName, "CodecID")!="FFFE")
        CodecID_NotEqual_PCM=true;
    else if (C->Get(FileName, "CodecID")=="FFFE")
    {
        int128u extFormatType=Ztring().From_UTF8(C->Get(FileName, "CodecID")).To_UUID();
        if ((extFormatType.hi&0x0000FFFFFFFFFFFFLL)!=0x0000000000001000LL ||
            (extFormatType.lo)!=0x800000AA00389B71LL ||
            ((int16u)((((extFormatType.hi>>48)&0xFF)<<8) | (extFormatType.hi>>56)))!=1)
            CodecID_NotEqual_PCM=true;
    }

    //Preparing
    Updating=true;
    Table->clearContents();
    Table->setRowCount(0);

    Table->setColumnCount(Column_Max);
    Table->setHorizontalHeaderItem(Column_Close, new QTableWidgetItem(""));
    Table->setHorizontalHeaderItem(Column_Id, new QTableWidgetItem("ID"));
    Table->setHorizontalHeaderItem(Column_Start, new QTableWidgetItem("Start"));
    Table->setHorizontalHeaderItem(Column_Length, new QTableWidgetItem("Length"));
    Table->setHorizontalHeaderItem(Column_Label, new QTableWidgetItem("Label"));
    Table->setItemDelegateForColumn(Column_Label, new CueDialogEdit_Delegate());
    Table->setHorizontalHeaderItem(Column_Note, new QTableWidgetItem("Note"));
    Table->setItemDelegateForColumn(Column_Note, new CueDialogEdit_Delegate());
    Table->setHorizontalHeaderItem(Column_PurposeId, new QTableWidgetItem("Purpose ID"));
    Table->setItemDelegateForColumn(Column_PurposeId, new CueDialogCombo_Delegate());
    Table->setHorizontalHeaderItem(Column_Country, new QTableWidgetItem("Country"));
    Table->setItemDelegateForColumn(Column_Country, new CueDialogCombo_Delegate());
    Table->setHorizontalHeaderItem(Column_Language, new QTableWidgetItem("Language"));
    Table->setItemDelegateForColumn(Column_Language, new CueDialogCombo_Delegate());
    Table->setHorizontalHeaderItem(Column_Dialect, new QTableWidgetItem("Dialect"));
    Table->setItemDelegateForColumn(Column_Dialect, new CueDialogCombo_Delegate());
    Table->setHorizontalHeaderItem(Column_CodePage, new QTableWidgetItem("CodePage"));
    Table->setItemDelegateForColumn(Column_CodePage, new CueDialogEdit_Delegate());
    Table->setHorizontalHeaderItem(Column_LabeledText, new QTableWidgetItem("Labeled Text"));
    Table->setItemDelegateForColumn(Column_LabeledText, new CueDialogEdit_Delegate());
    Table->setItemDelegateForColumn(Column_Id, new CueDialogEdit_Delegate());

    tinyxml2::XMLDocument Document;
    if (Document.Parse(Xml.c_str())!=tinyxml2::XML_SUCCESS)
    {
        UpdateRows();
        Updating=false;
        return;
    }

    tinyxml2::XMLElement* Root=Document.FirstChildElement("Cues")->ToElement();

    if (!Root)
    {
        UpdateRows();
        Updating=false;
        return;
    }


    for(tinyxml2::XMLNode* Node=Root->FirstChildElement("Cue"); Node; Node=Node->NextSibling())
    {
        int Index=Table->rowCount();
        Table->setRowCount(Index+1);
        for (size_t Column=0; Column<Column_Max; Column++)
        {
            Table->setItem(Index, Column, new QTableWidgetItem(""));
            if (Column==Column_Start || Column==Column_Length)
                Table->item(Index, Column)-> setFlags( Table->item(Index, Column)->flags()&~Qt::ItemIsEditable);
        }

        //Add delete button
        Table->item(Index, Column_Close)->setToolTip("Delete");

        QIcon CloseIcon(":/Image/Menu/File_Close.svg");
        CloseIcon.addFile(":/Image/Menu/File_Close_Hovered.svg",  QSize(), QIcon::Active);

        QPushButton* Close = new QPushButton(CloseIcon, "");
        Close->setFlat(true);
        Table->setCellWidget(Index, Column_Close, Close);
        connect(Close, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));

        tinyxml2::XMLElement* Element=NULL;
        Element=Node->FirstChildElement("ID");
        Table->item(Index, Column_Id)->setText(Element&&Element->GetText()?Element->GetText():"0");
        Element=Node->FirstChildElement("SampleOffset");
        Table->item(Index, Column_Start)->setText(QString().fromStdString(SamplesToTimeStamp(Element?Element->UnsignedText(0):0, SampleRate)));
        Table->item(Index, Column_Start)->setData(Qt::UserRole+1, QString().number(Element?Element->UnsignedText(0):0));
        Element=Node->FirstChildElement("Label");
        if (Element && Element->GetText())
            Table->item(Index, Column_Label)->setText(Element->GetText());
        Element=Node->FirstChildElement("Note");
        if (Element && Element->GetText())
            Table->item(Index, Column_Note)->setText(Element->GetText());
        tinyxml2::XMLElement* LabeledTextElement=Node->FirstChildElement("LabeledText");
        if (LabeledTextElement)
        {
            Element=LabeledTextElement->FirstChildElement("SampleLength");
            Table->item(Index, Column_Length)->setText(QString().fromStdString(SamplesToTimeStamp(Element?Element->UnsignedText(0):0, SampleRate)));
            Table->item(Index, Column_Length)->setData(Qt::UserRole+1, QString().number(Element?Element->UnsignedText(0):0));
            Element=LabeledTextElement->FirstChildElement("PurposeID");
            string PurposeID=Element&&Element->GetText()?Element->GetText():"0x00000000";
            Table->item(Index, Column_PurposeId)->setText(QString().fromStdString(HexToCC4(PurposeID)));
            Table->item(Index, Column_PurposeId)->setData(Qt::UserRole+1, QString().fromStdString(PurposeID));
            C->IsValid(FileName, "cue_ltxt_purpose", PurposeID);
            UpdateItem(Table->item(Index, Column_PurposeId));
            Element=LabeledTextElement->FirstChildElement("Country");
            int16u Country=Element?Element->UnsignedText(0):0;
            Table->item(Index, Column_Country)->setText(QString().fromStdString(CodeToCountry(Country)));
            Table->item(Index, Column_Country)->setData(Qt::UserRole+1, QString::number(Country));
            C->IsValid(FileName, "cue_ltxt_country", QString::number(Country).toStdString());
            UpdateItem(Table->item(Index, Column_Country));
            Element=LabeledTextElement->FirstChildElement("Language");
            int16u Language=Element?Element->UnsignedText(0):0;
            Table->item(Index, Column_Language)->setText(QString().fromStdString(CodeToLanguage(Language)));
            Table->item(Index, Column_Language)->setData(Qt::UserRole+1, QString::number(Language));
            Element=LabeledTextElement->FirstChildElement("Dialect");
            int16u Dialect=Element?Element->UnsignedText(0):0;
            Table->item(Index, Column_Dialect)->setText(QString().fromStdString(CodeToDialect(Language, Dialect)));
            Table->item(Index, Column_Dialect)->setData(Qt::UserRole+1, Dialect);
            C->IsValid(FileName, "cue_ltxt_language", QString::number(Language).toStdString());
            UpdateItem(Table->item(Index, Column_Language));
            C->IsValid(FileName, "cue_ltxt_language_dialect", QString::number(Language).toStdString() + ";" + QString::number(Dialect).toStdString());
            UpdateItem(Table->item(Index, Column_Dialect));
            Element=LabeledTextElement->FirstChildElement("CodePage");
            Table->item(Index, Column_CodePage)->setText(QString::fromUtf8(Element&&Element->GetText()?Element->GetText():"0"));
            Element=LabeledTextElement->FirstChildElement("Text");
            Table->item(Index, Column_LabeledText)->setText(QString::fromUtf8(Element&&Element->GetText()?Element->GetText():""));
        }

        // Check coherency: DataChunkID
        {
            Element=Node->FirstChildElement("DataChunkID");
            if (Element && Element->GetText() && strcmp(Element->GetText(), "0x64617461")!=0)
                DataChunkID_NotEqual_data=true;
        }

        // Check coherency: Position
        {
            Element=Node->FirstChildElement("Position");
            int32u Position = Element?Element->UnsignedText(0):0;
            Element=Node->FirstChildElement("SampleOffset");
            int32u SampleOffset = Element?Element->UnsignedText(0):0;
            if (Position!=0 && Position!=SampleOffset)
                Incoherent_Position=true;
        }

        // Check coherency: ChunkStart
        {
            Element=Node->FirstChildElement("ChunkStart");
            int32u ChunkStart=Element?Element->UnsignedText(0):0;

            if (ChunkStart!=0)
                NonZero_ChunkStart=true;
        }

        // Check coherency: BlockStart
        {
            Element=Node->FirstChildElement("BlockStart");
            int32u BlockStart=Element?Element->UnsignedText(0):0;

            Element=Node->FirstChildElement("Position");
            int32u Position=Element?Element->UnsignedText(0):0;

            Element=Node->FirstChildElement("SampleOffset");
            int64u SampleOffset=Element?Element->UnsignedText(0):0;

            if (BlockStart!=0 && Position==0 && BitsPerSample && BlockStart==SampleOffset*(BitsPerSample/8))
                DavidMode->setCheckState(Qt::Checked);
            else if (BlockStart!=0)
                NonZero_BlockStart=true;
        }
    }

    // Messages
    if (CodecID_NotEqual_PCM)
    {
        ReadOnly=true;
        QString Msg=QString("Unsuported: Edition mode is only supported for PCM WAVE.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);
    }

    if (DataChunkID_NotEqual_data)
    {
        ReadOnly=true;
        QString Msg=QString("Unsuported: others values than 'data' aren't supported for DataChunkID.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);
    }

    if (Incoherent_Position)
    {
        ReadOnly=true;
        QString Msg=QString("Unsuported: Position and SampleOffset aren't referencing the same point.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);
    }

    if (NonZero_BlockStart)
    {
        ReadOnly=true;
        QString Msg=QString("Unsuported: Non-zero value for BlockStart.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);
    }

    if (NonZero_ChunkStart)
    {
        ReadOnly=true;
        QString Msg=QString("Unsuported: Non-zero value for ChunkStart.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);
    }

    if(!C->IsValid(FileName, "cuexml", Xml, true))
    {
        ReadOnly=true;
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+QString().fromStdString(C->IsValid_LastError(FileName)));
        else
            Message->setText(QString().fromStdString(C->IsValid_LastError(FileName)));
    }
    if (!C->IsValid_LastWarning(FileName).empty())
    {
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+QString().fromStdString(C->IsValid_LastWarning(FileName)));
        else
            Message->setText(QString().fromStdString(C->IsValid_LastWarning(FileName)));
    }

    if (ReadOnly)
    {
        QString Msg=QString("Read-only mode enabled.");
        if (!Message->text().isEmpty())
            Message->setText(Message->text()+QString("\n")+Msg);
        else
            Message->setText(Msg);

        for (int Index=0; Index<Table->rowCount(); Index++)
        {
            Table->cellWidget(Index, Column_Close)->setEnabled(false);
            for (int Column=0; Column<Column_Max; Column++)
            {
                const QPalette DefaultPalette;
                bool DarkMode=DefaultPalette.color(QPalette::WindowText).lightness()>DefaultPalette.color(QPalette::Window).lightness();
                Table->item(Index, Column)->setBackground(QBrush(DarkMode?DefaultPalette.color(QPalette::Base).lighter(125):DefaultPalette.color(QPalette::Base).darker(125)));
                Table->item(Index, Column)->setFlags(Table->item(Index, Column)->flags()&~Qt::ItemIsEditable);
            }
        }
    }

    UpdateRows();
    Updating=false;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CueDialog::List2Xml()
{
    tinyxml2::XMLPrinter Printer;
    Printer.PushHeader(false, true);
    Printer.OpenElement("Cues");

    if (SampleRate)
        Printer.PushAttribute("samplerate", SampleRate);

    for (int Pos=0;Pos<Table->rowCount(); Pos++)
    {
        bool IsEmpty=true, LabelIsEmpty=true, NoteIsEmpty=true, TextIsEmpty=true;

        int32u Id=0, Start=0, Length=0, Country=0, Language=0, Dialect=0, CodePage=0;
        string Label, Note, Text, PurposeId;

        if (Table->item(Pos, Column_Id) && !Table->item(Pos, Column_Id)->text().isEmpty())
        {
            Id=Table->item(Pos, Column_Id)->text().toInt();
            IsEmpty=false;
        }

        if (Table->item(Pos, Column_Start) && !Table->item(Pos, Column_Start)->text().isEmpty())
        {
            Start=Table->item(Pos, Column_Start)->data(Qt::UserRole+1).toString().toInt();
            IsEmpty=false;
        }

        if (Table->item(Pos, Column_Label) && !Table->item(Pos, Column_Label)->text().isEmpty())
        {
            Label=Table->item(Pos, Column_Label)->text().toStdString();
            IsEmpty=false;
            LabelIsEmpty=false;
        }

        if (Table->item(Pos, Column_Note) && !Table->item(Pos, Column_Note)->text().isEmpty())
        {
            Note=Table->item(Pos, Column_Note)->text().toStdString();
            IsEmpty=false;
            NoteIsEmpty=false;
        }

        if (Table->item(Pos, Column_Length) && !Table->item(Pos, Column_Length)->text().isEmpty())
        {
            Length=Table->item(Pos, Column_Length)->data(Qt::UserRole+1).toString().toInt();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_PurposeId) && !Table->item(Pos, Column_PurposeId)->text().isEmpty())
        {
            PurposeId=Table->item(Pos, Column_PurposeId)->data(Qt::UserRole+1).toString().toStdString();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_Country) && !Table->item(Pos, Column_Country)->text().isEmpty())
        {
            Country=Table->item(Pos, Column_Country)->data(Qt::UserRole+1).toString().toInt();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_Language) && !Table->item(Pos, Column_Language)->text().isEmpty())
        {
            Language=Table->item(Pos, Column_Language)->data(Qt::UserRole+1).toString().toInt();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_Dialect) && !Table->item(Pos, Column_Dialect)->text().isEmpty())
        {
            Dialect=Table->item(Pos, Column_Dialect)->data(Qt::UserRole+1).toString().toInt();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_CodePage) && !Table->item(Pos, Column_CodePage)->text().isEmpty())
        {
            CodePage=Table->item(Pos, Column_CodePage)->text().toInt();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (Table->item(Pos, Column_LabeledText) && !Table->item(Pos, Column_LabeledText)->text().isEmpty())
        {
            Text=Table->item(Pos, Column_LabeledText)->text().toStdString();
            IsEmpty=false;
            TextIsEmpty=false;
        }

        if (!IsEmpty)
        {
            if (!Table->item(Pos, Column_Id) || Table->item(Pos, Column_Id)->text().isEmpty())
            {
                QVector<long> Ids;
                for (int Pos2=0; Pos2<Table->rowCount(); Pos2++)
                {
                    if (Pos2==Pos || !Table->item(Pos2, Column_Id))
                        continue;

                    bool Ok;
                    long Value=Table->item(Pos2, Column_Id)->text().toLong(&Ok);

                    if (Ok)
                    Ids.append(Value);
                }

                for (size_t Pos2=1; Pos2<0xFFFFFFFF; Pos2++)
                {
                    if (!Ids.contains(Pos2))
                    {
                        Id=Pos2;
                        break;
                    }
                }
            }

            Printer.OpenElement("Cue");
                Printer.OpenElement("ID");
                    Printer.PushText(Id);
                Printer.CloseElement();
                Printer.OpenElement("Position");
                if (DavidMode->checkState()==Qt::Checked)
                   Printer.PushText("0");
                else
                   Printer.PushText(Start);
                Printer.CloseElement();
                Printer.OpenElement("DataChunkID");
                    Printer.PushText("0x64617461");
                Printer.CloseElement();
                Printer.OpenElement("ChunkStart");
                    Printer.PushText("0");
                Printer.CloseElement();
                Printer.OpenElement("BlockStart");
                if (DavidMode->checkState()==Qt::Checked)
                   Printer.PushText(Start*(BitsPerSample/8));
                else
                    Printer.PushText("0");
                Printer.CloseElement();
                Printer.OpenElement("SampleOffset");
                    Printer.PushText(Start);
                Printer.CloseElement();
            if (!LabelIsEmpty)
            {
                Printer.OpenElement("Label");
                    Printer.PushText(Label.c_str());
                Printer.CloseElement();
            }
            if (!NoteIsEmpty)
            {
                Printer.OpenElement("Note");
                    Printer.PushText(Note.c_str());
                Printer.CloseElement();
            }
            if (!TextIsEmpty)
            {
                Printer.OpenElement("LabeledText");
                    Printer.OpenElement("SampleLength");
                        Printer.PushText(Length);
                    Printer.CloseElement();
                    Printer.OpenElement("PurposeID");
                        Printer.PushText(PurposeId.c_str());
                    Printer.CloseElement();
                    Printer.OpenElement("Country");
                        Printer.PushText(Country);
                    Printer.CloseElement();
                    Printer.OpenElement("Language");
                    Printer.PushText(Language);
                    Printer.CloseElement();
                    Printer.OpenElement("Dialect");
                        Printer.PushText(Dialect);
                    Printer.CloseElement();
                    Printer.OpenElement("CodePage");
                        Printer.PushText(CodePage);
                    Printer.CloseElement();
                    Printer.OpenElement("Text");
                        Printer.PushText(Text.c_str());
                    Printer.CloseElement();
                Printer.CloseElement();
            }
            Printer.CloseElement();
        }
    }
    Printer.CloseElement();

    Xml=string(Printer.CStr());
}
