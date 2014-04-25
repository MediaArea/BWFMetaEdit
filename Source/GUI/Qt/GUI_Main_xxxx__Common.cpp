// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
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
#include <QAction>
#include <QMenu>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

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
            C->Menu_File_Close_File_FileName_Set(FileName_Before+item(currentRow(), 0)->text().toLocal8Bit().data());

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

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) 
{
    //Preparing
    if (Updating)
        return;
    Updating=true;

    //Retrieving data
    string FileName=FileName_Before+item(topLeft.row(), 0)->text().toLocal8Bit().data();
    Ztring Field=horizontalHeaderItem(topLeft.column())->text().toLocal8Bit().data();
    Ztring ModifiedContent=topLeft.model()->data(topLeft.model()->index(topLeft.row(), topLeft.column(), rootIndex())).toString().toLocal8Bit().data();
    
    //Filling
    if (!(Field=="XMP" || Field=="aXML" || Field=="iXML")) //this is special cases
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
        string FileName=FileName_Before+item(Row, 0)->text().toLocal8Bit().data();
        for (int Column=0; Column<columnCount(); Column++)
        {
            Ztring Field=horizontalHeaderItem(Column)->text().toLocal8Bit().data();

            Colors_Update(item(Row, Column), FileName, Field);
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::Colors_Update (QTableWidgetItem* Item, const string &FileName, const string &Field) 
{
    if (!C->IsValid(FileName, Field, C->Get(FileName, Field)))
    {
			Item->setToolTip(C->IsValid_LastError(FileName).c_str());
			Item->setBackgroundColor(Qt::red);
	}
    else if (C->IsModified(FileName, Field))
    {
        if (Item->backgroundColor()==Qt::red)
            Item->setToolTip(QString());
        Item->setBackgroundColor(Qt::green);
    }
    else
    {
        if (Item->backgroundColor()==Qt::red)
            Item->setToolTip(QString());
        Item->setBackgroundColor(Qt::white);
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::SetEnabled (int Row, const QString &Field) 
{
    for (int Column=0; Column<columnCount(); Column++)
    {
        QString Field_Current=horizontalHeaderItem(Column)->text();
        if (Field_Current==Field)
        {
            string FileName=FileName_Before+item(Row, 0)->text().toLocal8Bit().data();
            if (Fill_Enabled(FileName, "BextVersion", C->Get(FileName, "BextVersion")))
                item(Row, Column)->setFlags(item(Row, Column)->flags()|Qt::ItemIsEnabled);
            else
                item(Row, Column)->setFlags(item(Row, Column)->flags()&((Qt::ItemFlags)-1-Qt::ItemIsEnabled));
            dataChanged(indexFromItem(item(Row, Column)), indexFromItem(item(Row, Column)));
            //Colors_Update(item(Row, Column), FileName, Field.toLocal8Bit().data());
            return;
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx__Common::SetText (int Row, const QString &Field) 
{
    for (int Column=0; Column<columnCount(); Column++)
    {
        QString Field_Current=horizontalHeaderItem(Column)->text();
        if (Field_Current==Field)
        {
            string FileName=FileName_Before+item(Row, 0)->text().toLocal8Bit().data();
            item(Row, Column)->setText(C->Get(FileName, Field.toLocal8Bit().data()).c_str());
            dataChanged(indexFromItem(item(Row, Column)), indexFromItem(item(Row, Column)));
            //Colors_Update(item(Row, Column), FileName, Field.toLocal8Bit().data());
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
    List.Separator_Set(1, _T(","));
    List.Write(Fill_Content());

    //Elminating unuseful info from filenames
    FileName_Before.clear();
    if (List.size()==2 && !List[1].empty())
    {
        size_t ToDelete=List[1][0].rfind(PathSeparator);
        if (ToDelete!=string::npos)
        {
            FileName_Before=List[1][0].substr(0, ToDelete+1);
            List[1][0].erase(0, ToDelete+1);
        }
    }
    else if (List.size()>1 && !List[1].empty())
    {
        size_t ToDelete;
        size_t File_Pos;
        for (ToDelete=0; ToDelete<List[1][0].size(); ToDelete++)
        {
            char Char_File1=List[1][0][ToDelete];
            for (File_Pos=2; File_Pos<List.size(); File_Pos++)
                if (ToDelete>=List[File_Pos][0].size() || List[File_Pos][0][ToDelete]!=Char_File1)
                    break;
            if (File_Pos!=List.size())
                break;
        }

        ToDelete=List[1][0].rfind(PathSeparator, ToDelete);
        if (ToDelete!=string::npos)
        {
            FileName_Before=List[1][0].substr(0, ToDelete+1);
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

    //Forcing reset, else this seems to be some Qt bug in the table display
    setRowCount(0);
    setColumnCount(0);

    //Filling - HorizontalHeader
    setRowCount((int)List.size()-1);
    size_t ColumnMissing_Count=0;
    for (size_t Option=0; Option<Main->Preferences->Group_Options_Count_Get(Fill_Group()); Option++)
        if (!Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Option]->isChecked())
            ColumnMissing_Count++;
    setColumnCount((int)(List[0].size()-ColumnMissing_Count));
    ColumnMissing_Count=0;
    for (size_t Data_Pos=0; Data_Pos<List[0].size(); Data_Pos++)
        if (Data_Pos==0 || Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Data_Pos-1]->isChecked())
        {
            QTableWidgetItem* Item=new QTableWidgetItem(QString().fromUtf8(List[0][Data_Pos].To_Local().c_str()));
            Item->setToolTip(Columns_ToolTip(List[0][Data_Pos]));
            setHorizontalHeaderItem((int)(Data_Pos-ColumnMissing_Count), Item);
        }
        else
            ColumnMissing_Count++;

    //Filling - VerticalHeader and content 
    for (size_t File_Pos=1; File_Pos<List.size(); File_Pos++)
    {
        if (List[File_Pos].empty())
            List[File_Pos].resize(1); //Empty filename, we need to manualy add it.
        ColumnMissing_Count=0;
        for (size_t Data_Pos=0; Data_Pos<List[0].size(); Data_Pos++)
            if (Data_Pos==0 || Main->Menu_Fields_CheckBoxes[Fill_Group()*options::MaxCount+Data_Pos-1]->isChecked())
            {
                QTableWidgetItem* Item;
                if (Data_Pos<List[File_Pos].size())
                {
                    ZenLib::Ztring Value=List[File_Pos][Data_Pos];
                    Item=new QTableWidgetItem(QString().fromUtf8(Value.To_Local().c_str()));
                    Item->setToolTip(Columns_ToolTip(List[0][Data_Pos]));
                }
                else
                    Item=new QTableWidgetItem(QString());

                if (!C->IsValid_Get(FileName_Before+List[File_Pos][0])
                 || (Data_Pos<List[File_Pos].size() && !Fill_Enabled(FileName_Before+List[File_Pos][0], List[0][Data_Pos], List[File_Pos][Data_Pos])))
                    Item->setFlags(Item->flags()&((Qt::ItemFlags)-1-Qt::ItemIsEnabled));
                setItem((int)File_Pos-1, (int)(Data_Pos-ColumnMissing_Count), Item);
            }
            else
                ColumnMissing_Count++;
    }

    //Configuring
    Colors_Update();
    resizeColumnsToContents();
    setSortingEnabled(true);

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
        for (int Pos=List[List_Pos].topRow(); Pos<=List[List_Pos].bottomRow(); Pos++)
            C->Menu_File_Close_File_FileName_Set(FileName_Before+item(Pos, 0)->text().toLocal8Bit().data());

    Main->Menu_Update();
}
