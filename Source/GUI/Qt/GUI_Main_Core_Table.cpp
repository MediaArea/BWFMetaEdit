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
#include "GUI/Qt/GUI_Main_xxxx_Loudness.h"
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_UmidDialog.h"
#include "Common/Core.h"
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
    QList<QTableWidgetItem*> Items=selectedItems();
    if (Items.size()==1)
    {
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
        if (!item(Item->row(), Item->column())->text().isEmpty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
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

                //Changing BextVersion Enabled value
                SetText   (*Item, "BextVersion");
                SetEnabled(*Item, "BextVersion");
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
                SetText(*Item, "TimeReference (translated)");
            if (Field=="TimeReference (translated)")
                SetText(*Item, "TimeReference");

            //Changing BextVersion Enabled value
            SetText   (*Item, "BextVersion");
            SetEnabled(*Item, "BextVersion");
        }
    }
    else
    {
        //Creating menu
        QMenu menu(this);

        //Handling Clear display
        bool ShowClear=false;
        for (int Pos=0; Pos<Items.size(); Pos++)
        {
            QTableWidgetItem* Item=Items[Pos];
            if (Item)
            {
                string FileName=FileName_Before+item(Item->row(), 0)->text().toLocal8Bit().data();
                string Field=horizontalHeaderItem(Item->column())->text().toLocal8Bit().data();
                if (!item(Item->row(), Item->column())->text().isEmpty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
                {
                    ShowClear=true;
                    break;
                }
            }
        }
        if (ShowClear)
        {
            menu.addAction(new QAction("Clear these values", this)); //If you change this, change the test text too
            menu.addSeparator();
        }
    
        //Displaying
        QAction* Action=menu.exec(Event->globalPos());
        if (Action==NULL)
            return;

        //Retrieving data
        QString Text=Action->text();

        //Special cases
        if (Text=="Clear these values") //If you change this, change the creation text too
        {
            for (int Pos=0; Pos<Items.size(); Pos++)
            {
                QTableWidgetItem* Item=Items[Pos];
                if (Item)
                {
                    string Field=horizontalHeaderItem(Item->column())->text().toLocal8Bit().data();

                    if (Field!="BextVersion")
                    {
                        item(Item->row(), Item->column())->setText(QString());
                        dataChanged(indexFromItem(item(Item->row(), Item->column())), indexFromItem(item(Item->row(), Item->column())));

                        //Special case
                        if (Field=="TimeReference")
                            SetText(*Item, "TimeReference (translated)");
                        if (Field=="TimeReference (translated)")
                            SetText(*Item, "TimeReference");
                    }
                }
            }
        }
    }

    //Menu
    Main->Menu_Update();
}

//---------------------------------------------------------------------------
bool GUI_Main_Core_Table::edit (const QModelIndex &index, EditTrigger trigger, QEvent *Event) 
{
    //Must we edit or not
    if (!index.isValid())
        return QTableWidget::edit(index, trigger, Event); //Normal editing

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
        return QTableWidget::edit(index, trigger, Event); //Normal editing 

    //Retrieving data
    QString ModifiedContentQ;
    if (trigger==AnyKeyPressed)
    {
        ModifiedContentQ=((QKeyEvent*)Event)->text(); //What the user has pressed
        if (!ModifiedContentQ.isEmpty() && ModifiedContentQ[0]==127)
            ModifiedContentQ.clear();
    }
    else
        ModifiedContentQ=index.model()->data(index.model()->index(index.row(), index.column(), rootIndex())).toString(); //Old value

    //Description / Originator / OriginatorReference
    if (Field=="Description" || Field=="Originator" || Field=="OriginatorReference" || Field=="IARL") 
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
        if (Field=="Originator")
            SetText(index, "IARL"); //IARL is sometimes updated if Originator is modified

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

        return false;
    }

    //Description / Originator / OriginatorReference
    if (Field=="UMID") 
    {
        //User interaction
        GUI_Main_xxxx_UmidDialog* Edit=new GUI_Main_xxxx_UmidDialog(C, FileName, Field, ModifiedContentQ, this);
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

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

        return false;
    }

    //BextVersion
    if (Field=="BextVersion") 
    {
        if (Main->Bext_Toggle_Get())
        {
            //Retrieving data
            int8u NewValue=Ztring().From_Local(C->Get(FileName, "BextVersion").c_str()).To_int8u();
            if (NewValue>=Main->Bext_MaxVersion_Get())
            {
                bool HasV1=C->Get(FileName, "LoudnessValue").empty() && C->Get(FileName, "LoudnessRange").empty() && C->Get(FileName, "MaxTruePeakLevel").empty() && C->Get(FileName, "MaxMomentaryLoudness").empty() && C->Get(FileName, "MaxShortTermLoudness").empty();
                bool HasV0=HasV1 && C->Get(FileName, "UMID").empty();
                if (HasV0)
                    NewValue=0;
                else if (HasV1)
                    NewValue=1;
                else
                    NewValue=2;
            }
            else 
                NewValue++;

            //Filling
            C->Set(FileName, "BextVersion", Ztring::ToZtring(NewValue).To_Local());
            item(index.row(), index.column())->setText(Ztring::ToZtring(NewValue).To_Local().c_str());
            Colors_Update(item(index.row(), index.column()), FileName, Field); //Must be forced because normal method does not handle Yes/No
            return false;
        }
        else
        {
            //User interaction
            GUI_Main_xxxx_Bext* Edit=new GUI_Main_xxxx_Bext(C, FileName, Main->Bext_MaxVersion_Get());
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

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

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
        SetText(index, "TimeReference");
        SetText(index, "TimeReference (translated)");

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

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

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

        return false;
    }

    //f
    if (Field=="LoudnessValue" || Field=="LoudnessRange" || Field=="MaxTruePeakLevel" || Field=="MaxMomentaryLoudness" || Field=="MaxShortTermLoudness") 
    {
        //User interaction
        GUI_Main_xxxx_Loudness* Edit=new GUI_Main_xxxx_Loudness(C, FileName, Field, ModifiedContentQ, C->Rules.Tech3285_Req);
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

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

        return false;
    }

    return QTableWidget::edit(index, trigger, Event); //Normal editing 
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
        if (Main->Bext_MaxVersion_Get()>2 || Ztring(C->Get(FileName, "BextVersion")).To_int16u()>Main->Bext_MaxVersion_Get() || (C->Get(FileName, "LoudnessValue").empty() && C->Get(FileName, "LoudnessRange").empty() && C->Get(FileName, "MaxTruePeakLevel").empty() && C->Get(FileName, "MaxMomentaryLoudness").empty() && C->Get(FileName, "MaxShortTermLoudness").empty()))
            return true;
        else
            return false;
    }

    if (!C->Overwrite_Reject)
        return true;
    
    return Value.empty();
}
