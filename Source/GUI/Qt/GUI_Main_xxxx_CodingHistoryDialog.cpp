// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QMessageBox>
#include <QDesktopWidget>
//---------------------------------------------------------------------------

//***************************************************************************
// CodingHistoryDialog_TableWidget
//***************************************************************************

//---------------------------------------------------------------------------
CodingHistoryDialog_TableWidget::CodingHistoryDialog_TableWidget (ZtringListList &List_, QWidget *parent)
: QTableWidget(parent)
{
    Updating=false;
    List=&List_;
}

//---------------------------------------------------------------------------
void CodingHistoryDialog_TableWidget::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) 
{
    if (Updating)
        return;

    //Retrieving data
    size_t Row=(size_t)topLeft.row();
    size_t Column=(size_t)topLeft.column();
    string Value=topLeft.model()->data(topLeft.model()->index(topLeft.row(), topLeft.column(), rootIndex())).toString().toLocal8Bit().data();
    
    //Sizing
    if (Row>=List->size())
        List->resize(Row+1);    
    if (Column>=(*List)[Row].size())
        (*List)[Row].resize(Column+1);    

    //Filling
    (*List)[Row][Column]=Value;

    //Adapting
    if (Row+1==(*List).size())
    {
        bool IsEmpty=true;
        for (size_t Pos=0; Pos<(*List)[Row].size(); Pos++)
            if (!(*List)[Row][Pos].empty())
                IsEmpty=false;
        if (!IsEmpty)
        {
            setRowCount((int)(*List).size()+1);
            resizeRowsToContents();
        }
    }
}

//***************************************************************************
// CodingHistoryDialog_Delegate
//***************************************************************************

//---------------------------------------------------------------------------
CodingHistoryDialog_Delegate::CodingHistoryDialog_Delegate(QObject *parent)
: QItemDelegate(parent)
{
}

//---------------------------------------------------------------------------
QWidget *CodingHistoryDialog_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox* Editor=new QComboBox(parent);
    Editor->installEventFilter(const_cast<CodingHistoryDialog_Delegate*>(this));

    return Editor;
}

//---------------------------------------------------------------------------
void CodingHistoryDialog_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString Value=index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox* Editor=static_cast<QComboBox*>(editor);

    Fill(Editor);
    
    for (int Pos=0; Pos<Editor->count(); Pos++)
        if (Value==Editor->itemText(Pos))
            Editor->setCurrentIndex(Pos);
}

//---------------------------------------------------------------------------
void CodingHistoryDialog_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* Editor=static_cast<QComboBox*>(editor);
    QString Value=Editor->currentText();

    model->setData(index, Value);
}

//---------------------------------------------------------------------------
void CodingHistoryDialog_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


//***************************************************************************
// CodingAlgorithmDelegate
//***************************************************************************

//---------------------------------------------------------------------------
void CodingAlgorithmDelegate::Fill(QComboBox* Editor) const
{
    Editor->addItem("");
    Editor->addItem("ANALOGUE");
    Editor->addItem("PCM");
    Editor->addItem("MPEG1L1");
    Editor->addItem("MPEG1L2");
    Editor->addItem("MPEG1L3");
    Editor->addItem("MPEG2L1");
    Editor->addItem("MPEG2L2");
    Editor->addItem("MPEG2L3");
}

//***************************************************************************
// SamplingFrequencyDelegate
//***************************************************************************

//---------------------------------------------------------------------------
void SamplingFrequencyDelegate::Fill(QComboBox* Editor) const
{
    Editor->addItem("");
    Editor->addItem("11000");
    Editor->addItem("22050");
    Editor->addItem("24000");
    Editor->addItem("32000");
    Editor->addItem("44100");
    Editor->addItem("48000");
    Editor->addItem("64000");
    Editor->addItem("88200");
    Editor->addItem("96000");
}

//***************************************************************************
// BitRateDelegate
//***************************************************************************

//---------------------------------------------------------------------------
void BitRateDelegate::Fill(QComboBox* Editor) const
{
    Editor->addItem("");
    Editor->addItem("8");
    Editor->addItem("16");
    Editor->addItem("24");
    Editor->addItem("32");
    Editor->addItem("40");
    Editor->addItem("48");
    Editor->addItem("56");
    Editor->addItem("64");
    Editor->addItem("80");
    Editor->addItem("96");
    Editor->addItem("112");
    Editor->addItem("128");
    Editor->addItem("144");
    Editor->addItem("160");
    Editor->addItem("176");
    Editor->addItem("192");
    Editor->addItem("224");
    Editor->addItem("256");
    Editor->addItem("320");
    Editor->addItem("352");
    Editor->addItem("384");
    Editor->addItem("416");
    Editor->addItem("448");
}

//***************************************************************************
// WordLengthDelegate
//***************************************************************************

//---------------------------------------------------------------------------
void WordLengthDelegate::Fill(QComboBox* Editor) const
{
    Editor->addItem("");
    Editor->addItem("8");
    Editor->addItem("12");
    Editor->addItem("14");
    Editor->addItem("16");
    Editor->addItem("18");
    Editor->addItem("20");
    Editor->addItem("22");
    Editor->addItem("24");
    Editor->addItem("32");
}

//***************************************************************************
// WordLengthDelegate
//***************************************************************************

//---------------------------------------------------------------------------
void ModeDelegate::Fill(QComboBox* Editor) const
{
    Editor->addItem("");
    Editor->addItem("mono");
    Editor->addItem("stereo");
    Editor->addItem("dual-mono");
    Editor->addItem("joint-stereo");
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_CodingHistoryDialog::GUI_Main_xxxx_CodingHistoryDialog(Core* C_, const std::string &FileName_, const std::string &Field_, const QString &OldText, bool Rules_Recommendations_, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=C_;
    FileName=FileName_;
    Field=Field_;
    Rules_Recommendations=Rules_Recommendations_;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("Coding History");
    setWindowIcon (QIcon(":/Image/FADGI/Logo.png"));

    //Buttons
    Load=new QPushButton("&Load file...");
    Save=new QPushButton("&Save file...");
    QDialogButtonBox* Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    Dialog->addButton(Load, QDialogButtonBox::ResetRole);
    Dialog->addButton(Save, QDialogButtonBox::ResetRole);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    connect(Load, SIGNAL(clicked()), this, SLOT(OnMenu_Load()));
    connect(Save, SIGNAL(clicked()), this, SLOT(OnMenu_Save()));

    //Central
    TextEdit=new QTextEdit(this);
    Table=new CodingHistoryDialog_TableWidget(List, this);
    Central=new QTabWidget(this);
    Central->addTab(Table   , tr("EBU R98-1999"));
    Central->addTab(TextEdit, tr("Free text"));
    connect(Central, SIGNAL(currentChanged (int)), this, SLOT(OnCurrentChanged(int)));
        
    QVBoxLayout* L=new QVBoxLayout();
    L->addWidget(Central);
    L->addWidget(Dialog);

    setLayout(L);

    resize(QApplication::desktop()->screenGeometry().width()/2, QApplication::desktop()->screenGeometry().height()/3);

    //Filling
    TextEdit->setPlainText(OldText);
    string Text=OldText.toLocal8Bit().data();
    if (Text.find("Coding History ")==0)
        Text.erase(0, 15);    
    if (Text.size()==0
     || (Text.size()>=2
      && Text[1]=='='
      && (Text[0]=='A' || Text[0]=='F' || Text[0]=='B' || Text[0]=='W' || Text[0]=='M' || Text[0]=='T')
      && Text.find("ANALOG,")==string::npos
      && Text.find(", ")==string::npos))
        OnCurrentChanged(0); //By default, index=0
    else
        Central->setCurrentIndex(1);
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::OnAccept ()
{
    OnCurrentChanged(1);
    
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
void GUI_Main_xxxx_CodingHistoryDialog::OnMenu_Load()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getOpenFileName(this, "", "", "");
    
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
    Buffer[Buffer_Offset]='\0';

    //Filling
    Ztring ModifiedContent((const char*)Buffer);
    delete[] Buffer;
    ModifiedContent.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
    ModifiedContent.FindAndReplace("\r", "\n", 0, Ztring_Recursive);
    QString ModifiedContentQ=QString().fromUtf8(ModifiedContent.To_Local().c_str());

    TextEdit->setPlainText(ModifiedContentQ);
    if (Central->currentIndex()==0)
        OnMenu_List();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::OnMenu_Save()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(this, "", "", "");
    
    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Create(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;

    //Filling
    F.Write(Ztring(TextEdit->toPlainText().toLocal8Bit().data()));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::OnMenu_List ()
{
    Text2List();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::OnMenu_Text ()
{
    List2Text();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::OnCurrentChanged (int Index)
{
    //Showing
    switch (Index)
    {
        case 0  : OnMenu_List(); break;
        case 1  : OnMenu_Text(); break;
        default : ;
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::List2Text ()
{
    if (List.empty())
        return;

    Ztring ToReturn;
    for (size_t Line=0; Line<List.size(); Line++)
    {
        for (size_t Column=0; Column<List[Line].size(); Column++)
        {
            if (!List[Line][Column].empty())
            {
                Char Header;
                switch (Column)
                {
                    case 0 : Header='A'; break;
                    case 1 : Header='F'; break;
                    case 2 : Header='B'; break;
                    case 3 : Header='W'; break;
                    case 4 : Header='M'; break;
                    case 5 : Header='T'; break;
                    default: Header='\0';
                }
                if (Header!='\0')
                {
                    ToReturn+=Header;
                    ToReturn+='=';
                }
                ToReturn+=List[Line][Column];
                if (Column!=List[Line].size())
                    ToReturn+=',';
            }
        }
        
        while (!ToReturn.empty() && (*(ToReturn.end()-1))==',')
            ToReturn.erase(ToReturn.end()-1);
            
        ToReturn+='\n';
    }
    
    if (!ToReturn.empty())
        TextEdit->setPlainText(ToReturn.To_Local().c_str());
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_CodingHistoryDialog::Text2List ()
{
    //Preparing
    Table->Updating=true;
	Table->clearContents();
    //Warning display
    bool Ununderstable=false;
    bool Modified=false;

    //Loading
    string Text=TextEdit->toPlainText().toLocal8Bit().data();
//	if(Text.empty())
//		return;
	List.clear();
    List.Separator_Set(0, "\n");
    if (Text.find(", ")==string::npos)
        List.Separator_Set(1, ",");
    else
        List.Separator_Set(1, ", ");
    List.Write(Text);
	int size = List.size();

    Table->setRowCount((int)List.size()+1);
    Table->setColumnCount(6);

    Table->setHorizontalHeaderItem(0, new QTableWidgetItem("Coding algorithm"));
    Table->setHorizontalHeaderItem(1, new QTableWidgetItem("Sampling frequency"));
    Table->setHorizontalHeaderItem(2, new QTableWidgetItem("Bit rate"));
    Table->setHorizontalHeaderItem(3, new QTableWidgetItem("Word length"));
    Table->setHorizontalHeaderItem(4, new QTableWidgetItem("Mode             "));
    Table->setHorizontalHeaderItem(5, new QTableWidgetItem("Text, free string"));
    for (size_t Line_Pos=0; Line_Pos<List.size(); Line_Pos++)
    {
        for (size_t Data_Pos=0; Data_Pos<List[Line_Pos].size(); Data_Pos++)
        {
            int Column=-1;
            Ztring &Value=List[Line_Pos][Data_Pos];
            if (Line_Pos==0 && Data_Pos==0 && Value.find("Coding History ")==0)
            {
                Modified=true;
                Value.erase(0, 15);
            }
            if (Value.size()>=2 && Value[1]=='=')
            {
                switch (Value[0])
                {
                    case 'A' : Column=0; break;
                    case 'F' : Column=1; break;
                    case 'B' : Column=2; break;
                    case 'W' : Column=3; break;
                    case 'M' : Column=4; break;
                    case 'T' : Column=5; break;
                    default  : Ununderstable=true;
                }
            }
            else
                Ununderstable=true;
            if (Column==Data_Pos)
            {
                Value.erase(0, 2);
                if (Value=="ANALOG")
                {
                    Modified=true;
                    Value="ANALOGUE";
                }
                QTableWidgetItem* Item=new QTableWidgetItem(QString().fromUtf8(Value.To_Local().c_str()));
            
                Table->setItem((int)Line_Pos, Column, Item);
            }
            else if (Column!=-1 || Data_Pos>5)
            {
                List[Line_Pos].insert(List[Line_Pos].begin()+Data_Pos, Ztring());
                if (List[Line_Pos].size()>6)
                {
                    Ununderstable=true;
                    break;
                }
            }
        }
    }

    Table->setItemDelegateForColumn(0, new CodingAlgorithmDelegate);
    Table->setItemDelegateForColumn(1, new SamplingFrequencyDelegate);
    Table->setItemDelegateForColumn(2, new BitRateDelegate);
    Table->setItemDelegateForColumn(3, new WordLengthDelegate);
    Table->setItemDelegateForColumn(4, new ModeDelegate);

    Table->resizeColumnsToContents();
    Table->resizeRowsToContents();

    if (Ununderstable)
    {
        if (isVisible())
        {
            QMessageBox MessageBox;
            MessageBox.setWindowTitle("BWF MetaEdit");
            MessageBox.setText("Text does not meet specifications, and matching is not possible");
                #if (QT_VERSION >= 0x040200)
                    MessageBox.setStandardButtons(QMessageBox::Close);
                #endif // (QT_VERSION >= 0x040200)
            MessageBox.setIcon(QMessageBox::Critical);
            MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
            MessageBox.exec();
        }

        //Clear
        List.clear();

        //Preparing
        Table->Updating=false;

        //Going back
        Central->setCurrentIndex(1);
        return;
    }

    if (Modified)
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText("Text does not meet specifications, do you accept to modify text to meet specifications?");
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            #if (QT_VERSION >= 0x040300)
                MessageBox.setDefaultButton(QMessageBox::No);
            #endif // (QT_VERSION >= 0x040300)
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Question);
        MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
        switch (MessageBox.exec())
        {
            case QMessageBox::No    :   //Save was clicked
                                        List.clear();

                                        //Preparing
                                        Table->Updating=false;

                                        //Going back
                                        Central->setCurrentIndex(1);
                                        return;
            default                 : ; //Continue
        } 
    }

    //Preparing
    Table->Updating=false;
}

//***************************************************************************
// Events
//***************************************************************************

void GUI_Main_xxxx_CodingHistoryDialog::hideEvent (QHideEvent*)
{
    if (Central->currentIndex()==0)
        List2Text();
}
