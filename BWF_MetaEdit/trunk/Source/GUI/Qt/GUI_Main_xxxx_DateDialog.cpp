// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
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
    setWindowIcon (QIcon(":/Image/FADGI/Logo.png"));

    //Buttons
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    Date=NULL;
    if (Field=="OriginationDate")
    {
        QString DateQ=QString().fromUtf8(ZenLib::Ztring(C->FileDate_Get(FileName)).To_Local().c_str());
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
    if (Field=="OriginationTime")
    {
        QString TimeQ=QString().fromUtf8(ZenLib::Ztring(C->FileDate_Get(FileName)).To_Local().c_str());
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

    //Central
    TextEdit=new QTextEdit(this);
    connect(TextEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
    TimeEdit=new QTimeEdit(this);
    TimeEdit->setVisible(Field=="OriginationTime");
    TimeEdit->setDisplayFormat("hh:mm:ss");
    connect(TimeEdit, SIGNAL(timeChanged (const QTime&)), this, SLOT(OnCalendar_Changed()));
    Central_Calendar=new QWidget(this);
    Central_Calendar_Layout=new QGridLayout();
    if (Field=="OriginationDate" || Field=="ICRD")
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
    Central->addTab(Central_Calendar, (Field=="OriginationDate" || Field=="OriginationTime")?tr("BWF (EBU Tech 3285) Recommandation"):tr("INFO (Microsoft definition) requirements"));
    Central->addTab(TextEdit, tr("Free text"));
    connect(Central, SIGNAL(currentChanged (int)), this, SLOT(OnCurrentChanged(int)));
    L->addWidget(Central);
    L->addWidget(Label);
    L->addWidget(Dialog);

    setLayout(L);
        
    //Filling
    TextEdit->setPlainText(Value);
    string ValueS=Value.toLocal8Bit().data();
    if (C->IsValid(FileName, Field, ValueS)
     && (ValueS.empty() || ValueS.size()==(Field=="OriginationDate" || Field=="ICRD")?10:8)
     && (ValueS.empty() || QDate::fromString(Value, Qt::ISODate)!=QDate() || (Field=="OriginationTime" && QTime::fromString(Value, Qt::ISODate)!=QTime())))
        OnMenu_Calendar();
    else
    {
        Central->setCurrentIndex(1);
        OnMenu_Text();
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
    std::string Value=TextEdit->toPlainText().toLocal8Bit().data();
    if (!C->IsValid(FileName, Field, Value))
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Ok);
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
        MessageBox.exec();
        return;
    }

    C->Set(FileName, Field, Value);

    accept();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnTextChanged ()
{
    std::string Value=TextEdit->toPlainText().toLocal8Bit().data();
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
    TextEdit->setPlainText(
        ( Calendar                                                             ?Calendar->selectedDate().toString(Qt::ISODate):tr(""))
       +((Calendar && TimeEdit_Activated && TimeEdit_Activated->isChecked()   )?tr(" "):tr(""))
       +((!Calendar || (TimeEdit_Activated && TimeEdit_Activated->isChecked()))?TimeEdit->text():tr("")));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnMenu_Date()
{
    if (Date->text().contains("date"))
        Calendar->setSelectedDate(QDate::fromString(Date->text().replace("&Set date to ", ""), Qt::ISODate)); //If you change this, change the creation text too
    if (Date->text().contains("time"))
        TimeEdit->setTime(QTime::fromString(Date->text().replace("&Set time to ", ""), Qt::ISODate)); //If you change this, change the creation text too
    OnCalendar_Changed();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_DateDialog::OnMenu_Calendar ()
{
    QString PlainText=TextEdit->toPlainText();
    if (Calendar) Calendar->setSelectedDate(QDate::fromString(PlainText, Qt::ISODate));
    Ztring Value=PlainText.toLocal8Bit().data();
    if ((Field=="OriginationDate" || Field=="OriginationTime"))
    {
        if (Field=="OriginationDate")
            TimeEdit->setVisible(false);
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
            TimeEdit->setTime(QTime::fromString(QString().fromUtf8(Value.To_Local().c_str()), Qt::ISODate));
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
    OnCalendar_Changed();
}

