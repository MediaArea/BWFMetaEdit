// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_Technical_Table.h"
#include "GUI/Qt/GUI_Main.h"
#include "GUI/Qt/GUI_Main_xxxx_Bext.h"
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QTextEdit>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Technical_Table::GUI_Main_Technical_Table(Core* _C, GUI_Main* parent)
: GUI_Main_xxxx__Common(_C, parent)
{
    setSelectionMode(SingleSelection);
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Technical_Table::contextMenuEvent (QContextMenuEvent* Event)
{
    //Retrieving data
    QTableWidgetItem* Item=itemAt(Event->pos());
    if (Item==NULL)
        return;
    string FileName=FileName_Before+item(Item->row(), 0)->text().toLocal8Bit().data();
    string Field=horizontalHeaderItem(Item->column())->text().toLocal8Bit().data();
    ZtringList History; History.Write(C->History(FileName, Field));
    Ztring Import;
    Ztring Export;
    Ztring Fill;
    Ztring Remove;
    if (Field=="XMP"
     || Field=="aXML"
     || Field=="iXML")
    {
        Import="Import..."; //If you change this, change at the end of method too
        if (!C->Get(FileName, Field).empty())
        {
            Export="Export..."; //If you change this, change at the end of method too
            Remove="Remove it"; //If you change this, change at the end of method too
        }
    }
    if (Field=="MD5Stored" && Fill_Enabled(FileName, Field, C->Get(FileName, "MD5Stored")))
    {
        if (!C->Get(FileName, "MD5Generated").empty() && C->Get(FileName, "MD5Stored")!=C->Get(FileName, "MD5Generated"))
            Fill="Fill with MD5Generated"; //If you change this, change at the end of method too      
        if (!item(Item->row(), Item->column())->text().isEmpty())
            Remove="Remove it"; //If you change this, change at the end of method too
    }

    //Do we need a context menu?
    if (Import.empty() && Export.empty() && Fill.empty() && Remove.empty() && History.empty())
        return;
    
    //Creating menu
    QMenu menu(this);

    //Handling export display
    if (!Import.empty())
        menu.addAction(new QAction(QString().fromUtf8(Import.To_Local().c_str()), this));
    if (!Export.empty())
        menu.addAction(new QAction(QString().fromUtf8(Export.To_Local().c_str()), this));
    if (!Import.empty() || !Export.empty())
        menu.addSeparator();

    //Handling Fill display
    if (!Fill.empty())
    {
        menu.addAction(new QAction(QString().fromUtf8(Fill.To_Local().c_str()), this));
        menu.addSeparator();
    }

    //Handling history display
    size_t Pos=History.size();
    if (!History.empty() && !(Field=="MD5Stored" && !Fill_Enabled(FileName, Field, C->Get(FileName, "MD5Stored"))))
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
    
    //Handling remove display
    if (!Remove.empty())
    {
        menu.addSeparator();
        menu.addAction(new QAction(QString().fromUtf8(Remove.To_Local().c_str()), this));
    }

    //Displaying
    QAction* Action=menu.exec(Event->globalPos());
    if (Action==NULL)
        return;

    //Retrieving data
    string ModifiedContent=Action->text().toLocal8Bit().data();

    //Specific cases
    if (ModifiedContent=="Remove it") //If you change this, change the creation text too
        ModifiedContent.clear();
    if (ModifiedContent=="Export...") //If you change this, change the creation text too
    {
        //User interaction
        QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                            tr("Export file..."),
                                                            "",
                                                            "XML files (*.xml);;All files (*.*)");
        if (FileNamesQ.isEmpty())
            return;

        //Filling
        File F;
        if (!F.Create(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
            return;
        F.Write(C->Get(FileName, Field));
        
        return;
    }
    if (ModifiedContent=="Import...") //If you change this, change the creation text too
    {
        //User interaction
        QString FileNamesQ = QFileDialog::getOpenFileName(  this,
                                                            tr("Import file..."),
                                                            "",
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
        Buffer[Buffer_Offset]='\0';

        //Filling
        Ztring Value((const char*)Buffer);
        delete[] Buffer;
        Value.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
        Value.FindAndReplace("\r", "\n", 0, Ztring_Recursive);
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

        ModifiedContent=Value;
    }

    //Filling
    if (Field=="XMP"
     || Field=="aXML"
     || Field=="iXML")
    {
        C->Set(FileName, Field, ModifiedContent);
        item(Item->row(), Item->column())->setText(C->Get(FileName, Field).empty()?"No":"Yes");
    }
    else if (Field=="MD5Stored" && ModifiedContent=="Fill with MD5Generated")
        item(Item->row(), Item->column())->setText(C->Get(FileName, "MD5Generated").c_str());
    else
    {
        Ztring NewValue(ModifiedContent);
        NewValue.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
        item(Item->row(), Item->column())->setText(NewValue.c_str());
    }

    //Configuring
    Colors_Update(item(Item->row(), Item->column()), FileName, Field);

    //Menu
    Main->Menu_Update();
}

//---------------------------------------------------------------------------
bool GUI_Main_Technical_Table::edit (const QModelIndex &index, EditTrigger trigger, QEvent *Event) 
{
    //Must we edit or not
    if (!index.isValid())
        return QTableWidget::edit(index, trigger, Event); //Normal editing

    //Init
    string FileName=FileName_Before+item(index.row(), 0)->text().toLocal8Bit().data();
    string Field=horizontalHeaderItem(index.column())->text().toLocal8Bit().data();

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

    //bext
    if (Field=="bext") 
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
            item(index.row(), index.column())->setText(Ztring(_T("Version ")+Ztring::ToZtring(NewValue)).To_Local().c_str());
            dataChanged(indexFromItem(item(index.row(), index.column())), indexFromItem(item(index.row(), index.column())));
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
            item(index.row(), index.column())->setText(("Version "+NewValue).c_str());
            dataChanged(indexFromItem(item(index.row(), index.column())), indexFromItem(item(index.row(), index.column())));
            return false;
        }
    }

    //XML
    if (Field=="XMP"
     || Field=="aXML"
     || Field=="iXML") 
    {
        //Retrieving data
        if (!ModifiedContentQ.isEmpty())
        {
            Ztring ModifiedContent=C->Get(FileName, Field);
            ModifiedContent.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
            ModifiedContentQ=QString().fromUtf8(ModifiedContent.To_Local().c_str());
        }

        //User interaction
        GUI_Main_xxxx_TextEditDialog* Edit=new GUI_Main_xxxx_TextEditDialog(C, FileName, Field, ModifiedContentQ);
        if (Edit->exec()!=QDialog::Accepted)
        {
            delete Edit; //Edit=NULL;
            return false; //No change
        }
        delete Edit; //Edit=NULL;

        //Updating
        item(index.row(), index.column())->setText(C->Get(FileName, Field).empty()?"No":"Yes");
        dataChanged(indexFromItem(item(index.row(), index.column())), indexFromItem(item(index.row(), index.column())));
        return false;
    }
 
    //MD5Stored
    if (Field=="MD5Stored") 
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
        return false;
    }


    return false;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
const string &GUI_Main_Technical_Table::Fill_Content () 
{
    return C->Technical_Get();
}

//---------------------------------------------------------------------------
group GUI_Main_Technical_Table::Fill_Group () 
{
    return Group_Tech;
}

//---------------------------------------------------------------------------
bool GUI_Main_Technical_Table::Fill_Enabled (const string &FileName, const string &Field, const string &Value)
{
    if (Field!="XMP"
     && Field!="aXML"
     && Field!="iXML"
     && Field!="bext"
     && !(Field=="MD5Stored" && (Value.empty() || C->EmbedMD5_AuthorizeOverWritting)))
        return false;

    if (Field=="bext")
    {
        if (Main->Bext_MaxVersion_Get()>2 || Ztring(C->Get(FileName, "BextVersion")).To_int16u()>Main->Bext_MaxVersion_Get() || (C->Get(FileName, "LoudnessValue").empty() && C->Get(FileName, "LoudnessRange").empty() && C->Get(FileName, "MaxTruePeakLevel").empty() && C->Get(FileName, "MaxMomentaryLoudness").empty() && C->Get(FileName, "MaxShortTermLoudness").empty()))
            return !C->Overwrite_Reject && Value!="No";
        else
            return false;
    }

    if (!C->Overwrite_Reject)
        return true;

    if (Field=="XMP"
     || Field=="aXML"
     || Field=="iXML")
        return Value!="Yes";

    return Value.empty();
}

