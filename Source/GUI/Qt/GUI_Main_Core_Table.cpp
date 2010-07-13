// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_Core_Table.h"
#include "GUI/Qt/GUI_Main.h"
#include "GUI/Qt/GUI_Main_xxxx_Bext.h"
#include "GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_DateDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h"
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"
#include <QtGui/QLabel>
#include <QtCore/QEvent>
#include <QtGui/QFont>
#include <QtGui/QTextEdit>
#include <QtGui/QDateEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QItemDelegate>
#include <QtGui/QStandardItemModel>
#include <QtCore/QDate>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QAction>
#include <QtGui/QMenu>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Core_Table::GUI_Main_Core_Table(Core* _C, GUI_Main* parent)
: GUI_Main_xxxx__Common(_C, parent)
{
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Core_Table::contextMenuEvent (QContextMenuEvent* Event)
{
    //Retrieving data
    QTableWidgetItem* Item=itemAt(Event->pos());
    if (Item==NULL)
        return;
    string FileName=FileName_Before+item(Item->row(), 0)->text().toLocal8Bit().data();
    string Field=horizontalHeaderItem(Item->column())->text().toLocal8Bit().data();
    ZtringList History; History.Write(C->History(FileName, Field));
    Ztring Date;
    if (Field=="OriginationDate" || Field=="OriginationTime" || Field=="ICRD")
    {
        Date=C->FileDate_Get(FileName);
        if (Date.size()>=10+1+8)
        {
            if (Date.size()>=10+1+8)
                Date.resize(10+1+8);
            if (Field=="ICRD")
                Date.insert(0, "&Set ICRD to file creation timestamp ("); //If you change this, change at the end of method too
            else
                Date.insert(0, "&Set originationDate and Time to file creation timestamp ("); //If you change this, change at the end of method too
            Date.append(")");
        }
        else
            Date.clear();
    }

    //Creating menu
    QMenu menu(this);

    //Handling AllFiles display
    {
        menu.addAction(new QAction("Fill all open files with this field value", this)); //If you change this, change the test text too
        menu.addSeparator();
    }

    //Handling Clear display
    if (!item(Item->row(), Item->column())->text().isEmpty() && C->IsValid(FileName, Field, string()))
    {
        menu.addAction(new QAction("Clear this value", this)); //If you change this, change the test text too
        menu.addSeparator();
    }

    //Handling date display
    if (!Date.empty())
    {
        menu.addAction(new QAction(QString().fromUtf8(Date.To_Local().c_str()), this));
        menu.addSeparator();
    }

    //Handling history display
    size_t Pos=History.size();
    if (!History.empty())
        do
        {
            Pos--;

            QString Text=QString().fromUtf8(History[Pos].To_Local().c_str());
            if (!Text.isEmpty())
            {
                QAction* Action=new QAction(Text, this);
                menu.addAction(Action);
            }
        }
        while (Pos>0);
    
    //Displaying
    QAction* Action=menu.exec(Event->globalPos());
    if (Action==NULL)
        return;

    //Retrieving data
    QString Text=Action->text();

    //Special cases
    if (Text=="Fill all open files with this field value") //If you change this, change the creation text too
    {
        for (int Row=0; Row<rowCount(); Row++)
        {
            item(Row, Item->column())->setText(QString().fromUtf8(Ztring(C->Get(FileName, Field)).To_Local().c_str()));
            dataChanged(indexFromItem(item(Row, Item->column())), indexFromItem(item(Row, Item->column())));

            //Special cases
            if (Field=="UMID")
            {
                //Changing BextVersion Enabled value
                string FileNameTemp=FileName_Before+item(Row, 0)->text().toLocal8Bit().data();
                item(Row, 8)->setText(C->Get(FileNameTemp, "BextVersion").c_str());
                dataChanged(indexFromItem(item(Row, 8)), indexFromItem(item(Row, 8)));
                bool Enabled=Fill_Enabled(FileName, "BextVersion", C->Get(FileNameTemp, "BextVersion"));
                if (Enabled)
                    item(Row, 8)->setFlags(item(Row, 8)->flags()|Qt::ItemIsEnabled);
                else
                    item(Row, 8)->setFlags(item(Row, 8)->flags()&((Qt::ItemFlags)-1-Qt::ItemIsEnabled));
            }
        }
        return;
    }
    if (Text=="Clear this value") //If you change this, change the creation text too
        Text.clear();

    //Filling
    if (Text.contains("&Set ")) //If you change this, change the creation text too
    {
        Text=Text.remove("&Set ICRD to file creation timestamp ("); //If you change this, change the creation text too
        Text=Text.remove("&Set originationDate and Time to file creation timestamp ("); //If you change this, change the creation text too
        Text=Text.remove(")"); //If you change this, change the creation text too
        if (horizontalHeaderItem(Item->column())->text()==QString().fromUtf8("ICRD"))
        {
            item(Item->row(), Item->column())->setText(Text);
            dataChanged(indexFromItem(item(Item->row(), Item->column())), indexFromItem(item(Item->row(), Item->column())));
        }
        else
        {
            QString Date=Text;
            Date.remove(10, 1+12);
            QString Time=Text;
            Time.remove(0, 10+1);
            Time.remove(8, 4);
            int Date_Pos=Item->column()+(horizontalHeaderItem(Item->column())->text()==QString().fromUtf8("OriginationDate")?0:-1);
            int Time_Pos=Item->column()+(horizontalHeaderItem(Item->column())->text()==QString().fromUtf8("OriginationTime")?0:1);
            item(Item->row(), Date_Pos)->setText(Date);
            dataChanged(indexFromItem(item(Item->row(), Date_Pos)), indexFromItem(item(Item->row(), Date_Pos)));
            item(Item->row(), Time_Pos)->setText(Time);
            dataChanged(indexFromItem(item(Item->row(), Time_Pos)), indexFromItem(item(Item->row(), Time_Pos)));
        }
    }
    else
    {
        item(Item->row(), Item->column())->setText(Text);
        dataChanged(indexFromItem(item(Item->row(), Item->column())), indexFromItem(item(Item->row(), Item->column())));

        //Special case
        if (Field=="TimeReference")
        {
            item(Item->row(), Item->column()-1)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
            dataChanged(indexFromItem(item(Item->row(), Item->column()-1)), indexFromItem(item(Item->row(), Item->column()-1)));
        }
        if (Field=="TimeReference (translated)")
        {
            item(Item->row(), Item->column()+1)->setText(C->Get(FileName, "TimeReference").c_str());
            dataChanged(indexFromItem(item(Item->row(), Item->column()+1)), indexFromItem(item(Item->row(), Item->column()+1)));
        }
        if (Field=="UMID")
        {
            //Changing BextVersion Enabled value
            item(Item->row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
            dataChanged(indexFromItem(item(Item->row(), 8)), indexFromItem(item(Item->row(), 8)));
            bool Enabled=Fill_Enabled(FileName, "BextVersion", C->Get(FileName, "BextVersion"));
            if (Enabled)
                item(Item->row(), 8)->setFlags(item(Item->row(), 8)->flags()|Qt::ItemIsEnabled);
            else
                item(Item->row(), 8)->setFlags(item(Item->row(), 8)->flags()&((Qt::ItemFlags)-1-Qt::ItemIsEnabled));
        }
    }

    item(Item->row(), 6)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
    Colors_Update(item(Item->row(), 6), FileName, "TimeReference (translated)");
    item(Item->row(), 7)->setText(C->Get(FileName, "TimeReference").c_str());
    Colors_Update(item(Item->row(), 7), FileName, "TimeReference");
    item(Item->row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
    Colors_Update(item(Item->row(), 8), FileName, "BextVersion");

    //Menu
    Main->Menu_Update();
}

//---------------------------------------------------------------------------
bool GUI_Main_Core_Table::edit (const QModelIndex &index, EditTrigger trigger, QEvent *event) 
{
    //Must we edit or not
    if (!index.isValid())
        return QTableWidget::edit(index, trigger, event); //Normal editing

    //Init
    string FileName=FileName_Before+item(index.row(), 0)->text().toLocal8Bit().data();
    string Field=horizontalHeaderItem(index.column())->text().toLocal8Bit().data();

    //Line is selected?
    if (trigger==CurrentChanged)
    {
        C->Menu_File_Close_File_FileName_Clear();
        C->Menu_File_Close_File_FileName_Set(FileName_Before+item(index.row(), 0)->text().toLocal8Bit().data());
        Main->Menu_Update();
    }

    //Should we handle edition manualy?
    if (trigger!=DoubleClicked && trigger!=AnyKeyPressed)
        return QTableWidget::edit(index, trigger, event); //Normal editing 

    //Retrieving data
    QString ModifiedContentQ;
    if (trigger==AnyKeyPressed)
    {
        ModifiedContentQ=((QKeyEvent*)event)->text(); //What the user has pressed
        if (!ModifiedContentQ.isEmpty() && ModifiedContentQ[0]==127)
            ModifiedContentQ.clear();
    }
    else
        ModifiedContentQ=index.model()->data(index.model()->index(index.row(), index.column(), rootIndex())).toString(); //Old value

    //Description / Originator / OriginatorReference
    if (Field=="Description" || Field=="Originator" || Field=="OriginatorReference" || Field=="IARL" || Field=="UMID") 
    {
        //User interaction
        GUI_Main_xxxx_TextEditDialog* Edit=new GUI_Main_xxxx_TextEditDialog(C, FileName, Field, ModifiedContentQ);
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Filling
        Ztring NewValue(C->Get(FileName, Field));
        NewValue.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
        item(index.row(), index.column())->setText(NewValue.c_str());
        item(index.row(), 6)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
        item(index.row(), 7)->setText(C->Get(FileName, "TimeReference").c_str());
        item(index.row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
        if (Field=="UMID")
        {
            //Changing BextVersion Enabled value
            bool Enabled=Fill_Enabled(FileName, "BextVersion", C->Get(FileName, "BextVersion"));
            if (Enabled)
                item(index.row(), 8)->setFlags(item(index.row(), 8)->flags()|Qt::ItemIsEnabled);
            else
                item(index.row(), 8)->setFlags(item(index.row(), 8)->flags()&((Qt::ItemFlags)-1-Qt::ItemIsEnabled));
        }
        if (Field=="Originator")
            item(index.row(), 11)->setText(C->Get(FileName, "IARL").c_str()); //IARL is sometimes updated if Originator is modified
        return false;
    }

    //BextVersion
    if (Field=="BextVersion") 
    {
        if (Main->Bext_Toggle_Get())
        {
            //Retrieving data
            Ztring NewValue(C->Get(FileName, "BextVersion"));
            if (NewValue=="1")
                NewValue="0";
            else
                NewValue="1";

            //Filling
            C->Set(FileName, "BextVersion", NewValue);
            item(index.row(), index.column())->setText(NewValue.c_str());
            Colors_Update(item(index.row(), index.column()), FileName, Field); //Must be forced because normal method does not handle Yes/No
            return false;
        }
        else
        {
            //User interaction
            GUI_Main_xxxx_Bext* Edit=new GUI_Main_xxxx_Bext(C, FileName);
            if (Edit->exec()!=QDialog::Accepted)
            {
                delete Edit; //Edit=NULL;
                return false; //No change
            }
            delete Edit; //Edit=NULL;
            Ztring NewValue(C->Get(FileName, "BextVersion"));

            //Updating
            item(index.row(), index.column())->setText(NewValue.c_str());
            Colors_Update(item(index.row(), index.column()), FileName, Field); //Must be forced because normal method does not handle Yes/No
            return false;
        }
    }

    //OriginationDate / OriginationTime / ICRD
    if (Field=="OriginationDate" || Field=="OriginationTime" || Field=="ICRD") 
    {
        //User interaction
        GUI_Main_xxxx_DateDialog* Edit=new GUI_Main_xxxx_DateDialog(C, FileName, Field, ModifiedContentQ);
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Updating
        Ztring NewValue(C->Get(FileName, Field));
        NewValue.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
        item(index.row(), index.column())->setText(NewValue.c_str());
        item(index.row(), 6)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
        item(index.row(), 7)->setText(C->Get(FileName, "TimeReference").c_str());
        item(index.row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
        return false;
    }

    //TimeReference
    if (Field=="TimeReference (translated)" || Field=="TimeReference") 
    {
        //User interaction
        GUI_Main_xxxx_TimeReferenceDialog* Edit=new GUI_Main_xxxx_TimeReferenceDialog(C, FileName, "TimeReference");
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Updating
        item(index.row(), 6)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
        item(index.row(), 7)->setText(C->Get(FileName, "TimeReference").c_str());
        item(index.row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
        return false;
    }

    //History
    if (Field=="CodingHistory") 
    {
        //User interaction
        GUI_Main_xxxx_CodingHistoryDialog* Edit=new GUI_Main_xxxx_CodingHistoryDialog(C, FileName, Field, ModifiedContentQ, C->Rules.CodingHistory_Rec);
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Updating
        Ztring NewValue(C->Get(FileName, Field));
        NewValue.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
        item(index.row(), index.column())->setText(NewValue.c_str());
        item(index.row(), 6)->setText(C->Get(FileName, "TimeReference (translated)").c_str());
        item(index.row(), 7)->setText(C->Get(FileName, "TimeReference").c_str());
        item(index.row(), 8)->setText(C->Get(FileName, "BextVersion").c_str());
        return false;
    }

    return QTableWidget::edit(index, trigger, event); //Normal editing 
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
const string &GUI_Main_Core_Table::Fill_Content () 
{
    return C->Core_Get();
}

//---------------------------------------------------------------------------
group GUI_Main_Core_Table::Fill_Group () 
{
    return Group_Core;
}

//---------------------------------------------------------------------------
bool GUI_Main_Core_Table::Fill_Enabled (const string &FileName, const string &Field, const string &Value)
{
    if (Field=="FileName")
        return false;

    if (Field=="BextVersion")
    {
        if (C->Get(FileName, "UMID").empty())
            return true;
        else
            return false;
    }

    if (!C->Overwrite_Reject)
        return true;
    
    return Value.empty();
}
