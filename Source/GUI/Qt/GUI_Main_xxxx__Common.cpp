// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx__Common.h"
#include "GUI/Qt/GUI_Main.h"
#include "GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_DateDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h"
#include "GUI/Qt/GUI_Preferences.h"
#include "Common/Core.h"
#include "Common/Common_About.h"
#include "ZenLib/ZtringListList.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QTextEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QDate>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QAction>
#include <QMenu>
#include <QCollator>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
QString Swap_MD5_Endianess(const QString& Value)
{
    QString ToReturn;
    int128u Bytes;

    int128u2BigEndian((int8u*)&Bytes, Ztring().From_UTF8(Value.toStdString()).To_int128u());

    if (Bytes==0)
        return Value;

    ToReturn=QString::fromUtf8(Ztring().From_Number(Bytes, 16).To_UTF8().c_str());
    while (ToReturn.size()<32)
        ToReturn.prepend('0'); //Padding with 0, this must be a 32-byte string

    return ToReturn;
}

//***************************************************************************
// OriginationTimeDelegate
//***************************************************************************

//---------------------------------------------------------------------------
OriginationTimeDelegate::OriginationTimeDelegate(QObject *parent)
: QItemDelegate(parent)
{
}

//---------------------------------------------------------------------------
QWidget *OriginationTimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QTimeEdit* Editor=new QTimeEdit(parent);
    Editor->setDisplayFormat("hh:mm:ss");
    Editor->installEventFilter(const_cast<OriginationTimeDelegate*>(this));

    return Editor;
}

//---------------------------------------------------------------------------
void OriginationTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString Value=index.model()->data(index, Qt::DisplayRole).toString();
    QTimeEdit *Editor=static_cast<QTimeEdit*>(editor);

    Editor->setTime(QTime::fromString(Value, "hh:mm:ss"));
}

//---------------------------------------------------------------------------
void OriginationTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *Editor=static_cast<QTimeEdit*>(editor);
    QString Value=Editor->time().toString("hh:mm:ss");

    model->setData(index, Value);
}

//---------------------------------------------------------------------------
void OriginationTimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

   
//***************************************************************************
// TimeReferenceDelegate
//***************************************************************************

//---------------------------------------------------------------------------
TimeReferenceDelegate::TimeReferenceDelegate(QObject *parent)
: QItemDelegate(parent)
{
}

//---------------------------------------------------------------------------
QWidget *TimeReferenceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QTimeEdit* Editor=new QTimeEdit(parent);
    Editor->setDisplayFormat("hh:mm:ss.zzz");
    Editor->installEventFilter(const_cast<TimeReferenceDelegate*>(this));

    return Editor;
}

//---------------------------------------------------------------------------
void TimeReferenceDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString Value=index.model()->data(index, Qt::DisplayRole).toString();
    QTimeEdit *Editor=static_cast<QTimeEdit*>(editor);

    Editor->setTime(QTime::fromString(Value, "hh:mm:ss.zzz"));
}

//---------------------------------------------------------------------------
void TimeReferenceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *Editor=static_cast<QTimeEdit*>(editor);
    QString Value=Editor->time().toString("hh:mm:ss.zzz");

    model->setData(index, Value);
}

//---------------------------------------------------------------------------
void TimeReferenceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

//***************************************************************************
// TableWidgetFileNameItem
//***************************************************************************
bool TableWidgetFileNameItem::operator<(const QTableWidgetItem &other) const
{
    const QString& first=text();
    const QString& second=other.text();

    QCollator collator;
    collator.setNumericMode(Main->Preferences->Group_Option_Checked_Get(Group_Tables, Option_Tables_NaturalFileSorting));

    if (Main->Preferences->Group_Option_Checked_Get(Group_Tables, Option_Tables_SortByFileName))
    {
        int temp=collator.compare(QUrl(first).fileName(), QUrl(second).fileName());
        if (temp)
            return temp < 0;
        else
            return collator.compare(first, second) < 0;
    }
    else
        return collator.compare(first, second) < 0;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx__Common::GUI_Main_xxxx__Common(Core* _C, GUI_Main* parent)
: QTableWidget(parent)
{
    //Internal
    C=_C;
    Main=parent;
    Updating=false;

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnItemSelectionChanged()));
    connect(horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(OnSortIndicatorChanged(int, Qt::SortOrder)));
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main_xxxx__Common::event (QEvent* Event) 
{
    //Should fill data
    if (Event->type()==QEvent::User)
    {
        Fill();
        setFocus();

        C->Menu_File_Close_File_FileName_Clear();
        if (currentRow()!=-1)
            C->Menu_File_Close_File_FileName_Set(FileName_Before+item(currentRow(), FILENAME_COL)->text().toUtf8().data());

        Event->accept();
        return true;
    }

    //User wants to resize contents
    if (Event->type()==(QEvent::Type)(QEvent::User+1))
    {
        resizeColumnsToContents();
        resizeRowsToContents();
        setFocus();

        Event->accept();
        return true;
    }

    //User has saved data or changed rules
    if (Event->type()==(QEvent::Type)(QEvent::User+2))
    {
        clear(); //Forcing a complete updated else there are some non-updated fields on Mac
        Fill();
        Colors_Update();
        setFocus();
        
        Event->accept();
        return true;
    }

    //User has changed display options
    if (Event->type()==(QEvent::Type)(QEvent::User+3))
    {
        clear();
        Fill();
        Colors_Update();
        setFocus();
        
        Event->accept();
        return true;
    }

    return QTableWidget::event(Event);
}

//***************************************************************************
// Override
//***************************************************************************

//---------------------------------------------------------------------------
QStyleOptionViewItem GUI_Main_xxxx__Common::viewOptions() const
{
    QStyleOptionViewItem Options=QTableWidget::viewOptions();
    Options.decorationAlignment=Qt::AlignCenter;
    Options.decorationPosition=QStyleOptionViewItem::Right;

    return Options;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) 
{
    //Preparing
    if (Updating)
        return;
    Updating=true;

    //Retrieving data
    string FileName=FileName_Before+item(topLeft.row(), FILENAME_COL)->text().toUtf8().data();
    string Field=horizontalHeaderItem(topLeft.column())->text().toUtf8().data();
    string ModifiedContent=topLeft.model()->data(topLeft.model()->index(topLeft.row(), topLeft.column(), rootIndex())).toString().toUtf8().data();
    
    //Filling
    if (!(Field=="Cue" || Field=="XMP" || Field=="aXML" || Field=="iXML" || Field =="bext")) //this is special cases
        C->Set(FileName, Field, ModifiedContent);

    //Configuring
    Colors_Update(item(topLeft.row(), topLeft.column()), FileName, Field);

    //Preparing
    Updating=false;

    //Menu
    Main->Menu_Update();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::Colors_Update () 
{
    for (int Row=0; Row<rowCount(); Row++)
    {
        string FileName=FileName_Before+item(Row, FILENAME_COL)->text().toUtf8().data();
        for (int Column=FILENAME_COL; Column<columnCount(); Column++)
        {
            string Field=horizontalHeaderItem(Column)->text().toUtf8().data();

            Colors_Update(item(Row, Column), FileName, Field);
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::Colors_Update (QTableWidgetItem* Item, const string &FileName, const string &Field) 
{
    if (!C->IsValid_Get(FileName) || C->IsReadOnly_Get(FileName) || !Fill_Enabled(FileName, Field, C->Get(FileName, Field=="Cue"?"cuexml":Field)))
    {
        const QPalette DefaultPalette;
        bool DarkMode=DefaultPalette.color(QPalette::WindowText).lightness()>DefaultPalette.color(QPalette::Window).lightness();
        Item->setBackground(QBrush(DarkMode?DefaultPalette.color(QPalette::Base).lighter(125):DefaultPalette.color(QPalette::Base).darker(125)));
    }
    else if (!C->IsValid(FileName, Field=="Cue"?"cuexml":Field, C->Get(FileName, Field=="Cue"?"cuexml":Field)))
    {
        Item->setToolTip(QString("<qt>%1</qt>").arg(QString(C->IsValid_LastError(FileName).c_str()).toHtmlEscaped()));
        Item->setIcon(QIcon(":/Image/Menu/Error.svg"));
        Item->setBackground(QBrush());
    }
    else
    {
        if (C->IsModified(FileName, Field=="Cue"?"cuexml":Field))
            Item->setBackground(QBrush(QColor(144, 238, 144))); //Green
        else
            Item->setBackground(QBrush());

        if(!C->IsValid_LastWarning(FileName.c_str()).empty())
        {
            Item->setToolTip(QString("<qt>%1</qt>").arg(QString(C->IsValid_LastWarning(FileName).c_str()).toHtmlEscaped()));
            Item->setIcon(QIcon(":/Image/Menu/Warning.svg"));
        }
        else
        {
            Item->setToolTip(QString());
            Item->setIcon(QIcon());
        }
    }

    if (C->IsReadOnly_Get(FileName))
    {
        if(Field=="FileName" && Item->icon().isNull())
            Item->setIcon(QIcon(":/Image/Menu/Warning.svg"));

        QString Message;
        if (!Item->toolTip().isEmpty())
        {
            Message+=Item->toolTip();
            Message+="\n\n";
        }
        Message+="Edit mode disabled, file is read only!";
        Item->setToolTip(Message);
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::SetEnabled (int Row, const QString &Field) 
{
    for (int Column=FILENAME_COL; Column<columnCount(); Column++)
    {
        QString Field_Current=horizontalHeaderItem(Column)->text();
        if (Field_Current==Field)
        {
            string FileName=FileName_Before+item(Row, FILENAME_COL)->text().toUtf8().data();
            dataChanged(indexFromItem(item(Row, Column)), indexFromItem(item(Row, Column)));
            Colors_Update(item(Row, Column), FileName, Field.toUtf8().data());
            return;
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::SetText (int Row, const QString &Field) 
{
    for (int Column=FILENAME_COL; Column<columnCount(); Column++)
    {
        QString Field_Current=horizontalHeaderItem(Column)->text();
        if (Field_Current==Field)
        {
            string FileName=FileName_Before+item(Row, FILENAME_COL)->text().toUtf8().data();
            if ((Field=="MD5Generated" || Field=="MD5Stored") && Main->Preferences->Group_Option_Checked_Get(Group_MD5, Option_MD5_SwapEndian))
                item(Row, Column)->setText(Swap_MD5_Endianess(QString::fromUtf8(C->Get(FileName, Field.toUtf8().data()).c_str())));
            else
                item(Row, Column)->setText(QString::fromUtf8(C->Get(FileName, Field.toUtf8().data()).c_str()));
            dataChanged(indexFromItem(item(Row, Column)), indexFromItem(item(Row, Column)));
            //Colors_Update(item(Row, Column), FileName, Field.toUtf8().data());
            return;
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::Fill () 
{
    //Preparing
    Updating=true;
    
    //Showing
    ZtringListList List;
    List.Separator_Set(0, EOL);
    List.Separator_Set(1, __T(","));
    List.Write(Ztring().From_UTF8(Fill_Content()));

    //Elminating unuseful info from filenames
    FileName_Before.clear();
    if (List.size()==2 && !List[1].empty())
    {
        size_t ToDelete=List[1][0].rfind(PathSeparator);
        if (ToDelete!=string::npos)
        {
            FileName_Before=Ztring(List[1][0].substr(0, ToDelete+1)).To_UTF8();
            List[1][0].erase(0, ToDelete+1);
        }
    }
    else if (List.size()>1 && !List[1].empty())
    {
        size_t ToDelete;
        size_t File_Pos;
        for (ToDelete=0; ToDelete<List[1][0].size(); ToDelete++)
        {
            Char Char_File1=List[1][0][ToDelete];
            for (File_Pos=2; File_Pos<List.size(); File_Pos++)
                if (ToDelete>=List[File_Pos][0].size() || List[File_Pos][0][ToDelete]!=Char_File1)
                    break;
            if (File_Pos!=List.size())
                break;
        }

        ToDelete=List[1][0].rfind(PathSeparator, ToDelete);
        if (ToDelete!=string::npos)
        {
            FileName_Before=Ztring(List[1][0].substr(0, ToDelete+1)).To_UTF8();
            for (File_Pos=1; File_Pos<List.size(); File_Pos++)
                List[File_Pos][0].erase(0, ToDelete+1);
        }
    }

    if (List.empty() || List[0].empty())
    {
        setRowCount(0);
        setColumnCount(0);

        //Preparing
        Updating=false;

        return;
    }

    bool AddingMode=rowCount()>0;

    //Forcing reset, else this seems to be some Qt bug in the table display
    setRowCount(0);
    setColumnCount(0);

    //Filling - HorizontalHeader
    setRowCount((int)List.size()-1);
    size_t ColumnMissing_Count=0;
    for (size_t Option=0; Option<Main->Preferences->Group_Options_Count_Get(Fill_Group()); Option++)
        if (!Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Option]->isChecked())
            ColumnMissing_Count++;
    setColumnCount((int)(FILENAME_COL+List[0].size()-ColumnMissing_Count));
    ColumnMissing_Count=0;

    QTableWidgetItem* Item=new QTableWidgetItem("");
    Item->setToolTip("Close");
    setHorizontalHeaderItem(0, Item);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);

    for (size_t Data_Pos=0; Data_Pos<List[0].size(); Data_Pos++)
        if (Data_Pos==0 || Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Data_Pos-1]->isChecked())
        {
            QTableWidgetItem* Item=new QTableWidgetItem(QString::fromUtf8(List[0][Data_Pos].To_UTF8().c_str()));
            setHorizontalHeaderItem((int)(FILENAME_COL+Data_Pos-ColumnMissing_Count), Item);
            horizontalHeaderItem((FILENAME_COL+Data_Pos-ColumnMissing_Count))->setToolTip(Columns_ToolTip(List[0][Data_Pos].To_UTF8()));
        }
        else
            ColumnMissing_Count++;

    //Filling - VerticalHeader and content 
    for (size_t File_Pos=1; File_Pos<List.size(); File_Pos++)
    {
        //Add close button
        QIcon CloseIcon(":/Image/Menu/File_Close.svg");
        CloseIcon.addFile(":/Image/Menu/File_Close_Hovered.svg",  QSize(), QIcon::Active);

        QTableWidgetItem* Item=new QTableWidgetItem(""); //Create QTableWidgetItem to edit cell flags
        Item->setToolTip("Close this file");
        Item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        setItem(File_Pos-1, 0, Item);

        QPushButton* Close = new QPushButton(CloseIcon, "");
        Close->setFlat(true);
        setCellWidget(File_Pos-1, 0, Close);
        connect(Close, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));

        //Fill fields
        if (List[File_Pos].empty())
            List[File_Pos].resize(1); //Empty filename, we need to manualy add it.
        ColumnMissing_Count=0;
        for (size_t Data_Pos=0; Data_Pos<List[0].size(); Data_Pos++)
            if (Data_Pos==0 || Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Data_Pos-1]->isChecked())
            {
                QTableWidgetItem* Item;
                if (Data_Pos<List[File_Pos].size())
                {
                    Ztring Value=List[File_Pos][Data_Pos];

                    QString Field=horizontalHeaderItem(FILENAME_COL+Data_Pos-ColumnMissing_Count)->text();
                    if ((Field=="MD5Generated" || Field=="MD5Stored") && Main->Preferences->Group_Option_Checked_Get(Group_MD5, Option_MD5_SwapEndian) && !Value.empty())
                        Value=Ztring().From_UTF8(Swap_MD5_Endianess(QString::fromUtf8(Value.To_UTF8().c_str())).toStdString());

                    if (Data_Pos==0)
                        Item=new TableWidgetFileNameItem(Main, QString::fromUtf8(Value.To_UTF8().c_str()));
                    else
                        Item=new QTableWidgetItem(QString::fromUtf8(Value.To_UTF8().c_str()));
                }
                else
                {
                    if (Data_Pos==0)
                        Item=new TableWidgetFileNameItem(Main, QString());
                    else
                        Item=new QTableWidgetItem(QString());
                }

                setItem((int)File_Pos-1, (int)(FILENAME_COL+Data_Pos-ColumnMissing_Count), Item);
            }
            else
                ColumnMissing_Count++;
    }

    //Configuring
    Colors_Update();
    resizeColumnsToContents();
    setColumnWidth(0, rowHeight(0));
    setSortingEnabled(true);
    sortByColumn(Sort_Column_Get(), Sort_Order_Get());

    //Preparing
    Updating=false;
}

//***************************************************************************
// Slots
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::OnItemSelectionChanged () 
{
    C->Menu_File_Close_File_FileName_Clear();

    QList<QTableWidgetSelectionRange> List=selectedRanges();
    for (int List_Pos=0; List_Pos<List.size(); List_Pos++)
    {
        for (int Pos=List[List_Pos].topRow(); Pos<=List[List_Pos].bottomRow(); Pos++)
        {
            QTableWidgetItem* Item=item(Pos, FILENAME_COL);
            if (Item)
                C->Menu_File_Close_File_FileName_Set(FileName_Before+Item->text().toUtf8().data());
        }
    }

    Main->Menu_Update();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::OnSortIndicatorChanged(int Index, Qt::SortOrder Order)
{
    if (Index==0)
        horizontalHeader()->setSortIndicator(Sort_Column_Get(), Sort_Order_Get());
    else
    {
        horizontalHeader()->setSortIndicator(Index, Order);
        Sort_Column_Set(Index);
        Sort_Order_Set(Order);
        sortItems(Index, Order);
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::OnCloseClicked ()
{
    QObject* Sender=sender();
    if (!Sender)
        return;

    for (int Row=0; Row<rowCount(); Row++)
    {
        if (cellWidget(Row, 0)==Sender)
        {
            C->Menu_File_Close_File_FileName_Clear();
            C->Menu_File_Close_File_FileName_Set(FileName_Before+item(Row, FILENAME_COL)->text().toUtf8().data());
            Main->OnMenu_File_Close_Files();
            break;
        }
    }
}
