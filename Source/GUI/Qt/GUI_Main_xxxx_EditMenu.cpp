// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_EditMenu.h"
#include "GUI/Qt/GUI_Main.h"
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"

#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QEvent>
#include <QMenu>

#ifdef __MACOSX__
#include "Common/Mac_Helpers.h"
#endif //__MACOSX__

using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
void  GUI_Main_xxxx_EditMenu::updateEditMenu( QList<QPair<string, string> > forItems)
{
    if (Updating)
        return;
    Updating=true;

    Items=forItems;

    if (!Main || !Main->Menu_Edit)
        return;

    ZtringListList FileList;
    FileList.Separator_Set(0, EOL);
    FileList.Separator_Set(1, __T(","));
    FileList.Write(Ztring().From_UTF8(C->Core_Get().c_str()));
    if (!FileList.empty())
       FileList.erase(FileList.begin()); //Remove headers

    Main->Menu_Edit->clear();
    #ifdef __MACOSX__
    //QMenu::clear() alone causes crashes on macOS
    clearNSMenu((void*)Main->Menu_Edit->toNSMenu());
    #endif //__MACOSX__

    if (Items.count()==1)
    {
        string FileName=Items.first().first;
        string Field=Items.first().second;

        //History data
        ZtringList History; History.Write(Ztring().From_UTF8(C->History(FileName, Field)));
        
        //Data data
        string Date;
        if (Field=="OriginationDate" || Field=="OriginationTime" || Field=="ICRD")
        {
            Date=C->FileDate_Get(FileName);
            if (Date.size()>=10+1+8)
                Date.resize(10+1+8);
            else
                Date.clear();
        }

        //Copy
        if (!C->Get(FileName, Field).empty())
        {
            QAction* Action=new QAction("Copy");
            Action->setProperty("Action", "Copy");
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Paste
        if (!QApplication::clipboard()->text().isEmpty() && C->IsValid(FileName, Field, QApplication::clipboard()->text().toStdString()))
        {
            QAction* Action=new QAction("Paste");
            Action->setProperty("Action", "Paste");
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        if(Main->Menu_Edit->actions().count())
             Main->Menu_Edit->addSeparator();

        //Handling AllFiles display
        if (FileList.size()>1) {
            QAction* Action=new QAction("Fill all open files with this field value");
            Action->setProperty("Action", "Fill");
            Main->Menu_Edit->addAction(Action);
            Main->Menu_Edit->addSeparator();
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling Clear display
        if (!C->Get(FileName, Field).empty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
        {
            QAction* Action=new QAction("Clear this value");
            Action->setProperty("Action", "Clear");
            Main->Menu_Edit->addAction(Action);
            Main->Menu_Edit->addSeparator();
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling date display
        if (!Date.empty())
        {
            string Text;
            if (Field=="ICRD")
                Text="&Set ICRD to file creation timestamp ("+Date+")";
            else
                Text="&Set originationDate and Time to file creation timestamp ("+Date+")";

            QAction* Action=new QAction(QString::fromUtf8(Text.c_str()));
            Action->setProperty("Action", "Date");
            Main->Menu_Edit->addAction(Action);
            Main->Menu_Edit->addSeparator();
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling history display
        for (size_t Pos=History.size(); Pos>0; Pos--)
        {
            string Text=History[Pos-1].To_UTF8();
            if (!Text.empty())
            {
                QAction* Action=new QAction(QString::fromUtf8(Text.c_str()));
                Action->setProperty("Action", "History");
                Main->Menu_Edit->addAction(Action);
                connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
            }
        }
    }
    else if (Items.count()>1)
    {
        //Clear data
        bool ShowOriginationDate=true;
        bool ShowICRDDate=true;
        bool ShowClear=false;
        for (int Pos=0; Pos<Items.size(); Pos++)
        {
            string FileName=Items.at(Pos).first;
            string Field=Items.at(Pos).second;

            if (!C->Get(FileName, Field).empty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
                ShowClear=true;

            if (Field!="OriginationDate" && Field!="OriginationTime")
                ShowOriginationDate=false;

            if (Field!="ICRD")
                ShowICRDDate=false;

            if (ShowOriginationDate || ShowICRDDate)
            {
                string Date=C->FileDate_Get(FileName);
                if (Date.size()<10+1+8)
                {
                    ShowOriginationDate=false;
                    ShowICRDDate=false;
                }
            }
        }

        //Handling AllFiles display
        {
            QAction* Action=new QAction("Fill all open files with these fields values");
            Action->setProperty("Action", "Fill");
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling Clear display
        if (ShowClear)
        {
            QAction* Action=new QAction("Clear these values");
            Action->setProperty("Action", "Clear");
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling origination date display
        if (ShowOriginationDate)
        {
            QAction* Action=new QAction("Set originationDate and Time to files creation timestamp");
            Action->setProperty("Action", "Date");
            Main->Menu_Edit->addSeparator();
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

        //Handling ICRD date display
        if (ShowICRDDate)
        {
            QAction* Action=new QAction("Set ICRD to files creation timestamp");
            Main->Menu_Edit->addSeparator();
            Action->setProperty("Action", "Date");
            Main->Menu_Edit->addAction(Action);
            connect(Action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
        }

    }

    Main->Menu_Edit->setEnabled(Main->Menu_Edit->actions().count());
    Updating=false;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_EditMenu::showContextMenu(const QPoint& globalPos)
{
    if (!Main || !Main->Menu_Edit || !Main->Menu_Edit->actions().count())
        return;

    //Displaying
    Main->Menu_Edit->exec(globalPos);
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_EditMenu::onActionTriggered()
{
    QAction* Action=qobject_cast<QAction*>(sender());
    if(!Action || Items.isEmpty())
        return;

    //Filling
    if (Action->property("Action")=="Copy")
    {
        string FileName=Items.first().first;
        string Field=Items.first().second;
        QApplication::clipboard()->setText(QString::fromUtf8(C->Get(FileName, Field).c_str()));
    }
    else if (Action->property("Action")=="Paste")
    {
        string FileName=Items.first().first;
        string Field=Items.first().second;
        C->Set(FileName, Field, QApplication::clipboard()->text().toStdString());
        Q_EMIT valuesChanged(true);
    }
    else if (Action->property("Action")=="Fill")
    {
        //Fill data
        ZtringListList FileList;
        FileList.Separator_Set(0, EOL);
        FileList.Separator_Set(1, __T(","));
        FileList.Write(Ztring().From_UTF8(C->Core_Get().c_str()));
        if (!FileList.empty())
           FileList.erase(FileList.begin()); //Remove headers

        for (int Pos=0; Pos<Items.size(); Pos++)
        {
            string FileName=Items.at(Pos).first;
            string Field=Items.at(Pos).second;
            string NewValue=C->Get(FileName, Field);
            for (size_t Pos=0; Pos<FileList.size(); Pos++)
            {
                if (FileList[Pos][0].empty())
                    continue;

                C->Set(FileList[Pos][0].To_UTF8(), Field, NewValue);
            }
        }
        Q_EMIT valuesChanged(false);
    }
    else if (Action->property("Action")=="Date")
    {
        for (int Pos=0; Pos<Items.size(); Pos++)
        {
            string FileName=Items.at(Pos).first;
            string Field=Items.at(Pos).second;
            if (Field=="OriginationDate" || Field=="OriginationTime")
            {
                C->Set(FileName, "OriginationDate", "TIMESTAMP");
                C->Set(FileName, "OriginationTime", "TIMESTAMP");
            }
            else
                C->Set(FileName, Field, "TIMESTAMP");
        }
        Q_EMIT valuesChanged(true);
    }
    else if (Action->property("Action")=="Clear")
    {
        for (int Pos=0; Pos<Items.size(); Pos++)
        {
            string FileName=Items.at(Pos).first;
            string Field=Items.at(Pos).second;
            if (C->IsValid(FileName, Field, string()) && Field!="BextVersion")
                C->Set(FileName, Field, string());
        }
        Q_EMIT valuesChanged(true);
    }
    else if (Action->property("Action")=="History")
    {
        string FileName=Items.first().first;
        string Field=Items.first().second;
        C->Set(FileName, Field, Action->text().toStdString());
        Q_EMIT valuesChanged(true);
    }
}
