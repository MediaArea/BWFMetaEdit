// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
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
#include <QItemSelectionModel>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Static objects
//***************************************************************************
int GUI_Main_Core_Table::SortColumn=FILENAME_COL;
Qt::SortOrder GUI_Main_Core_Table::SortOrder=Qt::AscendingOrder;

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Core_Table::GUI_Main_Core_Table(Core* _C, GUI_Main* parent)
: GUI_Main_xxxx__Common(_C, parent)
{
    MenuHandler=new GUI_Main_xxxx_EditMenu(parent, _C, this);
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
    connect(MenuHandler, SIGNAL(valuesChanged(bool)), this, SLOT(onValuesChanged(bool)));
}

GUI_Main_Core_Table::~GUI_Main_Core_Table()
{
    delete MenuHandler; //MenuHandler=NULL;
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Core_Table::contextMenuEvent (QContextMenuEvent* Event)
{
        QTableWidgetItem* Item=itemAt(Event->pos());
        if (Item)
           setCurrentItem(Item, QItemSelectionModel::Select);

    //force menu update before opening
    onItemSelectionChanged();

    MenuHandler->showContextMenu(Event->globalPos());
}

//---------------------------------------------------------------------------
 void GUI_Main_Core_Table::keyPressEvent(QKeyEvent* Event)
{
    if (selectedItems().size()==1)
    {
        QTableWidgetItem* Item=selectedItems().at(0);
        string FileName=FileName_Before+item(Item->row(), FILENAME_COL)->text().toUtf8().data();
        string Field=horizontalHeaderItem(Item->column())->text().toUtf8().data();

        if (Event->matches(QKeySequence::Copy))
        {
            QApplication::clipboard()->setText(QString::fromUtf8(C->Get(FileName, Field).c_str()));

            Event->accept();
            return;
        }
        else if (Event->matches(QKeySequence::Paste))
        {
            if (!QApplication::clipboard()->text().isEmpty() && C->IsValid(FileName, Field, QApplication::clipboard()->text().toStdString()))
            {
                string NewValue=QApplication::clipboard()->text().toStdString();
                AdaptEOL(NewValue, adapt_n);

                item(Item->row(), Item->column())->setText(QString::fromUtf8(NewValue.c_str()));
                dataChanged(indexFromItem(item(Item->row(), Item->column())), indexFromItem(item(Item->row(), Item->column())));

                update(indexFromItem(item(Item->row(), Item->column())));

                //Special case
                if (Field=="TimeReference")
                    SetText(*Item, "TimeReference (translated)");
                if (Field=="TimeReference (translated)")
                    SetText(*Item, "TimeReference");
                if (Field=="OriginationDate")
                    SetText(*Item, "OriginationTime");
                if (Field=="OriginationTime")
                    SetText(*Item, "OriginationDate");

                //Changing BextVersion Enabled value
                SetText   (*Item, "BextVersion");
                SetEnabled(*Item, "BextVersion");

                Event->accept();
                return;
            }
        }
    }

    QTableWidget::keyPressEvent(Event);
}

//---------------------------------------------------------------------------
bool GUI_Main_Core_Table::edit (const QModelIndex &index, EditTrigger trigger, QEvent *Event) 
{
    //Must we edit or not
    if (!index.isValid())
        return QTableWidget::edit(index, trigger, Event); //Normal editing

    //Init
    string FileName=FileName_Before+item(index.row(), FILENAME_COL)->text().toUtf8().data();
    string Field=horizontalHeaderItem(index.column())->text().toUtf8().data();

    if (!Fill_Enabled(FileName, Field, C->Get(FileName, Field)))
        return false;

    //Line is selected?
    if (trigger==CurrentChanged)
    {
        C->Menu_File_Close_File_FileName_Clear();
        C->Menu_File_Close_File_FileName_Set(FileName_Before+item(index.row(), FILENAME_COL)->text().toUtf8().data());
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
    if (Field=="Description" || Field=="Originator" || Field=="OriginatorReference" || Field=="IARL" || Field=="IART" || Field=="ICMS" || Field=="ICMT" || Field=="ICOP" || Field=="IENG" || Field=="IGNR" || Field=="IKEY" || Field=="IMED" || Field=="INAM" || Field=="IPRD" || Field=="ISBJ" || Field=="ISFT" || Field=="ISRC" || Field=="ISRF" || Field=="ITCH") //Most INFO fields added in order to permit to show warning 
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
        string NewValue=C->Get(FileName, Field);
        AdaptEOL(NewValue, adapt_n);
        item(index.row(), index.column())->setText(QString::fromUtf8(NewValue.c_str()));
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
        string NewValue=C->Get(FileName, Field);
        AdaptEOL(NewValue, adapt_n);
        item(index.row(), index.column())->setText(QString::fromUtf8(NewValue.c_str()));

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
            int8u NewValue=Ztring().From_UTF8(C->Get(FileName, "BextVersion").c_str()).To_int8u();
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
            C->Set(FileName, "BextVersion", Ztring::ToZtring(NewValue).To_UTF8());
            item(index.row(), index.column())->setText(Ztring::ToZtring(NewValue).To_UTF8().c_str());
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
            Ztring NewValue=Ztring().From_UTF8(C->Get(FileName, "BextVersion"));

            //Updating
            item(index.row(), index.column())->setText(NewValue.To_UTF8().c_str());
            Colors_Update(item(index.row(), index.column()), FileName, Field); //Must be forced because normal method does not handle Yes/No
            return false;
        }
    }

    //OriginationDate / OriginationTime / ICRD
    if (Field=="OriginationDate" || Field=="OriginationTime" || Field=="ICRD" || Field=="IDIT")
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
        string NewValue=C->Get(FileName, Field);
        AdaptEOL(NewValue, adapt_n);
        item(index.row(), index.column())->setText(QString::fromUtf8(NewValue.c_str()));

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
        GUI_Main_xxxx_CodingHistoryDialog* Edit=new GUI_Main_xxxx_CodingHistoryDialog(C, FileName, Field, ModifiedContentQ, C->Rules);
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Updating
        string NewValue=C->Get(FileName, Field);
        AdaptEOL(NewValue, adapt_n);
        item(index.row(), index.column())->setText(QString::fromUtf8(NewValue.c_str()));

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
        string NewValue=C->Get(FileName, Field);
        AdaptEOL(NewValue, adapt_n);
        item(index.row(), index.column())->setText(QString::fromUtf8(NewValue.c_str()));

        //Changing BextVersion Enabled value
        SetText   (index, "BextVersion");
        SetEnabled(index, "BextVersion");

        return false;
    }

    return QTableWidget::edit(index, trigger, Event); //Normal editing 
}

//---------------------------------------------------------------------------
void GUI_Main_Core_Table::onItemSelectionChanged()
{
    //Retrieving data
    QList<QTableWidgetItem*> SelectedItems=selectedItems();
    QList<QPair<string, string> > Items;
    for (int Pos=0; Pos<SelectedItems.size(); Pos++)
    {
        QTableWidgetItem* Item=SelectedItems.at(Pos);
        string FileName=FileName_Before+item(Item->row(), FILENAME_COL)->text().toUtf8().data();
        string Field=horizontalHeaderItem(Item->column())->text().toUtf8().data();
        Items.append(qMakePair(FileName, Field));
    }

    MenuHandler->updateEditMenu(Items);
}

//---------------------------------------------------------------------------
void GUI_Main_Core_Table::onValuesChanged(bool onlySelected)
{
    //Retrieving data
    QList<QTableWidgetItem*> SelectedItems=selectedItems();

    if (onlySelected)
    {
        for (int Pos=0; Pos<SelectedItems.size(); Pos++)
        {
            QTableWidgetItem* Item=SelectedItems.at(Pos);
            string FileName=FileName_Before+item(Item->row(), FILENAME_COL)->text().toUtf8().data();
            string Field=horizontalHeaderItem(Item->column())->text().toUtf8().data();

            //Updating
            string NewValue=C->Get(FileName, Field);
            AdaptEOL(NewValue, adapt_n);
            item(Item->row(), Item->column())->setText(QString::fromUtf8(NewValue.c_str()));
            dataChanged(indexFromItem(item(Item->row(), Item->column())), indexFromItem(item(Item->row(), Item->column())));

            //Special case
            if (Field=="TimeReference")
                SetText(*Item, "TimeReference (translated)");
            if (Field=="TimeReference (translated)")
                SetText(*Item, "TimeReference");
            if (Field=="OriginationDate")
                SetText(*Item, "OriginationTime");
            if (Field=="OriginationTime")
                SetText(*Item, "OriginationDate");

            //Changing BextVersion Enabled value
            SetText   (*Item, "BextVersion");
            SetEnabled(*Item, "BextVersion");
        }
    }
    else
    {
        for (int Pos=0; Pos<SelectedItems.size(); Pos++)
        {
            QTableWidgetItem* Item=SelectedItems.at(Pos);
            string FileName=FileName_Before+item(Item->row(), FILENAME_COL)->text().toUtf8().data();
            string Field=horizontalHeaderItem(Item->column())->text().toUtf8().data();

            for (int Row=0; Row<rowCount(); Row++)
            {
                string FileName=FileName_Before+item(Row, FILENAME_COL)->text().toUtf8().data();
                string NewValue=C->Get(FileName, Field);
                AdaptEOL(NewValue, adapt_n);
                item(Row, Item->column())->setText(QString::fromUtf8(NewValue.c_str()));
                dataChanged(indexFromItem(item(Row, Item->column())), indexFromItem(item(Row, Item->column())));

                //Special case
                if (Field=="TimeReference")
                    SetText(*item(Row, Item->column()), "TimeReference (translated)");
                if (Field=="TimeReference (translated)")
                    SetText(*item(Row, Item->column()), "TimeReference");
                if (Field=="OriginationDate")
                    SetText(*item(Row, Item->column()), "OriginationTime");
                if (Field=="OriginationTime")
                SetText(*item(Row, Item->column()), "OriginationDate");

                //Changing BextVersion Enabled value
                SetText   (*item(Row, Item->column()), "BextVersion");
                SetEnabled(*item(Row, Item->column()), "BextVersion");
            }
        }
    }

    //Menu
    Main->Menu_Update();

    viewport()->update();
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
        if (Main->Bext_MaxVersion_Get()>2 || Ztring().From_UTF8(C->Get(FileName, "BextVersion")).To_int16u()>Main->Bext_MaxVersion_Get() || (C->Get(FileName, "LoudnessValue").empty() && C->Get(FileName, "LoudnessRange").empty() && C->Get(FileName, "MaxTruePeakLevel").empty() && C->Get(FileName, "MaxMomentaryLoudness").empty() && C->Get(FileName, "MaxShortTermLoudness").empty()))
            return true;
        else
            return false;
    }

    if (!C->Overwrite_Reject)
        return true;
    
    return Value.empty();
}
