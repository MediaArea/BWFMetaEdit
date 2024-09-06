// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_DateDialog.h"
#include "Riff/Riff_Handler.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QLabel>
#include <QCheckBox>
#include <QTabWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <iomanip>
#include <sstream>
#include <locale>

//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_DateDialog::GUI_Main_xxxx_DateDialog(Core* _C, const std::string &FileName_, const std::string &Field_, const QString &Value, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;
    Field=Field_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle(QString::fromUtf8(Field.c_str()));
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    Date=NULL;
    if (Field=="OriginationDate")
    {
        QString DateQ=QString::fromUtf8(ZenLib::Ztring().From_UTF8(C->FileDate_Get(FileName)).To_UTF8().c_str());
        if (DateQ.size()>10)
        {
            DateQ.truncate(10);
            DateQ.insert(0, "&Set date to "); //If you change this, change the DateDialog button click text too
            Date=new QPushButton(DateQ);
            Dialog->addButton(Date, QDialogButtonBox::ResetRole);
            connect(Date, SIGNAL(clicked()), this, SLOT(OnMenu_Date()));
        }
        else
            Date=NULL;
    }
    else if (Field=="OriginationTime")
    {
        QString TimeQ=QString::fromUtf8(ZenLib::Ztring().From_UTF8(C->FileDate_Get(FileName)).To_UTF8().c_str());
        if (TimeQ.size()>10)
        {
            TimeQ.remove(0, 10+1);
            TimeQ.remove(8, 4);
            TimeQ.insert(0, "&Set time to "); //If you change this, change the DateDialog button click text too
            Date=new QPushButton(TimeQ);
            Dialog->addButton(Date, QDialogButtonBox::ResetRole);
            connect(Date, SIGNAL(clicked()), this, SLOT(OnMenu_Date()));
        }
    }
    else if (Field=="IDIT")
    {
        QString DateQ=QString::fromUtf8(ZenLib::Ztring().From_UTF8(C->FileDate_Get(FileName)).To_UTF8().c_str());
        if (DateQ.size()>=10+1+8)
        {
            DateQ.truncate(10+1+8);
            QDateTime DateTime=QDateTime::fromString(DateQ, Qt::ISODate);

            struct tm Time;
            Time.tm_sec=DateTime.time().second();
            Time.tm_min=DateTime.time().minute();
            Time.tm_hour=DateTime.time().hour();
            Time.tm_mday=DateTime.date().day();
            Time.tm_mon=DateTime.date().month()-1;
            Time.tm_year=DateTime.date().year()-1900;
            Time.tm_wday=DateTime.date().dayOfWeek()==Qt::Sunday?0:DateTime.date().dayOfWeek();
            Time.tm_yday=DateTime.date().dayOfYear();
            Time.tm_isdst=-1;

            stringstream ss;
            ss.imbue(std::locale("C"));
            ss << put_time(&Time, "%a %b %d %H:%M:%S %Y");

            DateQ=QString::fromLatin1(ss.str().c_str());
            DateQ.truncate(24);
            DateQ.insert(0, "&Set date and time to "); //If you change this, change the DateDialog button click text too
            Date=new QPushButton(DateQ);
            Dialog->addButton(Date, QDialogButtonBox::ResetRole);
            connect(Date, SIGNAL(clicked()), this, SLOT(OnMenu_Date()));
        }
    }

    //Central
    TextEdit=new QTextEdit(this);
    connect(TextEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
    TimeEdit=new QTimeEdit(this);
    TimeEdit->setVisible(Field=="OriginationTime" || Field=="IDIT");
    TimeEdit->setDisplayFormat("hh:mm:ss");
    connect(TimeEdit, SIGNAL(timeChanged (const QTime&)), this, SLOT(OnCalendar_Changed()));
    Central_Calendar=new QWidget(this);
    Central_Calendar_Layout=new QGridLayout();
    if (Field=="OriginationDate" || Field=="ICRD" || Field=="IDIT")
    {
        Calendar=new QCalendarWidget(this);
        connect(Calendar, SIGNAL(selectionChanged()), this, SLOT(OnCalendar_Changed()));
        Central_Calendar_Layout->addWidget(Calendar, 0, 0, 1, 2);
        if (Field=="OriginationDate")
            TimeEdit_Activated=NULL;
        else
        {
            TimeEdit_Activated=new QCheckBox(tr("With time"), this);
            connect(TimeEdit_Activated, SIGNAL(stateChanged (int)), this, SLOT(OnTimeActivatedChanged(int)));
            Central_Calendar_Layout->addWidget(TimeEdit_Activated, 1, 0);
        }
    }
    else
    {
        TimeEdit_Activated=NULL;
        Calendar=NULL;
    }
    Central_Calendar_Layout->addWidget(TimeEdit, 1, 1);
    #if (QT_VERSION >= 0x040300)
        Central_Calendar_Layout->setContentsMargins(0, 0, 0, 0);
    #endif
    Central_Calendar->setLayout(Central_Calendar_Layout);
        
    Label=new QLabel(this);

    QVBoxLayout* L=new QVBoxLayout();
    Central=new QTabWidget(this);
    Central->addTab(Central_Calendar, (Field=="OriginationDate" || Field=="OriginationTime")?tr("&BWF (EBU Tech 3285) Recommandation"):tr("&INFO (Microsoft definition) requirements"));
    Central->addTab(TextEdit, tr("&Free text"));
    connect(Central, SIGNAL(currentChanged (int)), this, SLOT(OnCurrentChanged(int)));
    L->addWidget(Central);
    L->addWidget(Label);
    L->addWidget(Dialog);

    setLayout(L);
        
    //Filling
    TextEdit->setPlainText(Value);
    string ValueS=Value.toUtf8().data();
    if (C->IsValid(FileName, Field, ValueS)
     && (ValueS.empty() || ValueS.size()==((Field=="OriginationDate" || Field=="ICRD")?10:8) || (Field=="IDIT" && ValueS.size()==24))
     && (ValueS.empty() || QDate::fromString(Value, Qt::ISODate)!=QDate() || (Field=="OriginationTime" && QTime::fromString(Value, Qt::ISODate)!=QTime()) || (Field=="IDIT")))
        OnMenu_Calendar();
    else
    {
        Central->setCurrentIndex(1);
        OnMenu_Text();
    }

    if (Field=="IDIT")
    {
        TimeEdit_Activated->setChecked(true);
        TimeEdit_Activated->setEnabled(false);
    }

    //Screen
    move(QApplication::desktop()->screenGeometry().width()*3/8, QApplication::desktop()->screenGeometry().height()*3/8);
    resize(QApplication::desktop()->screenGeometry().width()/4, QApplication::desktop()->screenGeometry().height()/4);
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnAccept ()
{
    std::string Value=TextEdit->toPlainText().toUtf8().data();
    if (!C->IsValid(FileName, Field, Value))
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
void GUI_Main_xxxx_DateDialog::OnTextChanged ()
{
    std::string Value=TextEdit->toPlainText().toUtf8().data();
    if (!C->IsValid(FileName, Field, Value))
    {
        Label->setText(QString::fromUtf8(C->IsValid_LastError(FileName).c_str()));
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        Label->setText(QString());
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnCalendar_Changed()
{
    if (Field=="IDIT")
    {
        struct tm Time;
        Time.tm_sec=TimeEdit->time().second();
        Time.tm_min=TimeEdit->time().minute();
        Time.tm_hour=TimeEdit->time().hour();
        Time.tm_mday=Calendar->selectedDate().day();
        Time.tm_mon=Calendar->selectedDate().month()-1;
        Time.tm_year=Calendar->selectedDate().year()-1900;
        Time.tm_wday=Calendar->selectedDate().dayOfWeek()==Qt::Sunday?0:Calendar->selectedDate().dayOfWeek();
        Time.tm_yday=Calendar->selectedDate().dayOfYear();
        Time.tm_isdst=-1;
        stringstream ss;
        ss.imbue(std::locale("C"));
        ss << put_time(&Time, "%a %b %d %H:%M:%S %Y");
        TextEdit->setPlainText(QString().fromLatin1(ss.str().c_str()));
        return;
    }

    TextEdit->setPlainText(
        ( Calendar                                                             ?Calendar->selectedDate().toString(Qt::ISODate):tr(""))
       +((Calendar && TimeEdit_Activated && TimeEdit_Activated->isChecked()   )?tr(" "):tr(""))
       +((!Calendar || (TimeEdit_Activated && TimeEdit_Activated->isChecked()))?TimeEdit->text():tr("")));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnMenu_Date()
{
    if (Date->text().contains("date and time"))
    {
        std::string Value=Date->text().replace("&Set date and time to ", "").toLatin1().data();
        int Month=0;
        if (Value[4]=='J' && Value[5]=='a' && Value[6]=='n') Month= 1;
        if (Value[4]=='F' && Value[5]=='e' && Value[6]=='b') Month= 2;
        if (Value[4]=='M' && Value[5]=='a' && Value[6]=='r') Month= 3;
        if (Value[4]=='A' && Value[5]=='p' && Value[6]=='r') Month= 4;
        if (Value[4]=='M' && Value[5]=='a' && Value[6]=='y') Month= 5;
        if (Value[4]=='J' && Value[5]=='u' && Value[6]=='n') Month= 6;
        if (Value[4]=='J' && Value[5]=='u' && Value[6]=='l') Month= 7;
        if (Value[4]=='A' && Value[5]=='u' && Value[6]=='g') Month= 8;
        if (Value[4]=='S' && Value[5]=='e' && Value[6]=='p') Month= 9;
        if (Value[4]=='O' && Value[5]=='c' && Value[6]=='t') Month=10;
        if (Value[4]=='N' && Value[5]=='o' && Value[6]=='v') Month=11;
        if (Value[4]=='D' && Value[5]=='e' && Value[6]=='c') Month=12;
        int Day=(Value[8]-'0')*10+(Value[9]-'0');
        int Year=(Value[20]-'0')*1000+(Value[21]-'0')*100+(Value[22]-'0')*10+(Value[23]-'0');
        int Hours  =(Value[11]-'0')*10+(Value[12]-'0');
        int Minutes=(Value[14]-'0')*10+(Value[15]-'0');
        int Seconds=(Value[17]-'0')*10+(Value[18]-'0');

        Calendar->setSelectedDate(QDate(Year, Month, Day));
        TimeEdit->setTime(QTime(Hours, Minutes, Seconds));
    }
    else if (Date->text().contains("date"))
         Calendar->setSelectedDate(QDate::fromString(Date->text().replace("&Set date to ", ""), Qt::ISODate)); //If you change this, change the creation text too
    else if (Date->text().contains("time"))
         TimeEdit->setTime(QTime::fromString(Date->text().replace("&Set time to ", ""), Qt::ISODate)); //If you change this, change the creation text too
     OnCalendar_Changed();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnMenu_Calendar ()
{
    QString PlainText=TextEdit->toPlainText();
    if (Calendar)
    {
        if (Field=="IDIT")
        {
            std::string Value=PlainText.toLatin1().data();
            if (Value.size()==24)
            {
                int Month=0;
                if (Value[4]=='J' && Value[5]=='a' && Value[6]=='n') Month= 1;
                if (Value[4]=='F' && Value[5]=='e' && Value[6]=='b') Month= 2;
                if (Value[4]=='M' && Value[5]=='a' && Value[6]=='r') Month= 3;
                if (Value[4]=='A' && Value[5]=='p' && Value[6]=='r') Month= 4;
                if (Value[4]=='M' && Value[5]=='a' && Value[6]=='y') Month= 5;
                if (Value[4]=='J' && Value[5]=='u' && Value[6]=='n') Month= 6;
                if (Value[4]=='J' && Value[5]=='u' && Value[6]=='l') Month= 7;
                if (Value[4]=='A' && Value[5]=='u' && Value[6]=='g') Month= 8;
                if (Value[4]=='S' && Value[5]=='e' && Value[6]=='p') Month= 9;
                if (Value[4]=='O' && Value[5]=='c' && Value[6]=='t') Month=10;
                if (Value[4]=='N' && Value[5]=='o' && Value[6]=='v') Month=11;
                if (Value[4]=='D' && Value[5]=='e' && Value[6]=='c') Month=12;
                int Day=(Value[8]-'0')*10+(Value[9]-'0');
                int Year=(Value[20]-'0')*1000+(Value[21]-'0')*100+(Value[22]-'0')*10+(Value[23]-'0');

                Calendar->setSelectedDate(QDate(Year, Month, Day));
            }
        }
        else
            Calendar->setSelectedDate(QDate::fromString(PlainText, Qt::ISODate));
    }

    Ztring Value=Ztring().From_UTF8(PlainText.toStdString());
    if ((Field=="OriginationDate" || Field=="OriginationTime" || Field=="IDIT"))
    {
        if (Field=="OriginationDate")
            TimeEdit->setVisible(false);
        else if (Field=="IDIT")
        {
            TimeEdit->setVisible(true);
            if (TimeEdit_Activated)
                TimeEdit_Activated->setChecked(true);

            if (Value.size()==24)
            {
                int Hours  =(Value[11]-'0')*10+(Value[12]-'0');
                int Minutes=(Value[14]-'0')*10+(Value[15]-'0');
                int Seconds=(Value[17]-'0')*10+(Value[18]-'0');
                TimeEdit->setTime(QTime(Hours, Minutes, Seconds));
            }
        }
        else
        {
            TimeEdit->setVisible(true);
            if (TimeEdit_Activated) TimeEdit_Activated->setChecked(true);
            TimeEdit->setTime(QTime::fromString(PlainText, Qt::ISODate));
        }
    }
    else if (Value.size()<10
      ||  Value[0]< '1' || Value[0]> '9' 
      ||  Value[1]< '0' || Value[1]> '9' 
      ||  Value[2]< '0' || Value[2]> '9' 
      ||  Value[3]< '0' || Value[3]> '9' 
      ||  Value[4]!='-'
      ||  Value[5]< '0' || Value[5]> '1' 
      || (Value[6]< (Value[5]=='0'?'1':'0')) || (Value[6]> (Value[5]=='1'?'2':'9')) //Only 01-12
      ||  Value[7]!='-'
      ||  Value[8]< '0' || Value[8]> '3' 
      || (Value[9]< (Value[8]=='0'?'1':'0')) || (Value[9]> (Value[8]=='3'?'1':'9'))) //Only 01-31
        ; //
    else
    {
        Value.erase(0, 11);
        if (Value.size()<8
          ||  Value[0]< '0' || Value[0]> '9' 
          ||  Value[1]< '0' || (Value[1]> (Value[0]=='2'?'3':'9')) //Only 00-23
          ||  Value[2]!=':'
          ||  Value[3]< '0' || Value[3]> '5' 
          ||  Value[4]< '0' || Value[4]> '9' 
          ||  Value[5]!=':'
          ||  Value[6]< '0' || Value[6]> '5' 
          ||  Value[7]< '0' || Value[7]> '9') 
        {
            TimeEdit->setVisible(false);
            TimeEdit_Activated->setChecked(false);
        }
        else
        {
            TimeEdit->setVisible(true);
            TimeEdit_Activated->setChecked(true);
            TimeEdit->setTime(QTime::fromString(QString::fromUtf8(Value.To_UTF8().c_str()), Qt::ISODate));
        }
    }
    OnCalendar_Changed();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnMenu_Text ()
{
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnCurrentChanged (int Index)
{
    //Showing
    switch (Index)
    {
        case 0  : OnMenu_Calendar(); break;
        case 1  : OnMenu_Text(); break;
        default : ;
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnTimeActivatedChanged (int State)
{
    TimeEdit->setVisible(State);
}

