// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_ContextMenu.h"
#include "GUI/Qt/GUI_Main.h"
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"

#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>

using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Functions
//***************************************************************************
int GUI_Main_xxxx_ContextMenu::showCoreMenu(const QPoint& globalPos, QList<QPair<string, string> > forItems)
{
    if (forItems.isEmpty())
        return 0;

    //Fill data
    ZtringListList FileList;
    FileList.Separator_Set(0, EOL);
    FileList.Separator_Set(1, __T(","));
    FileList.Write(Ztring().From_UTF8(C->Core_Get().c_str()));
    if (!FileList.empty())
       FileList.erase(FileList.begin()); //Remove headers

    if (forItems.count()==1)
    {
        string FileName=forItems.first().first;
        string Field=forItems.first().second;

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


        //Creating menu
        QMenu menu;

        //Copy
        if (!C->Get(FileName, Field).empty())
        {
            QAction* Action=new QAction("Copy");
            Action->setProperty("Action", "Copy");
            menu.addAction(Action);
        }

        //Paste
        if (!QApplication::clipboard()->text().isEmpty() && C->IsValid(FileName, Field, QApplication::clipboard()->text().toStdString()))
        {
            QAction* Action=new QAction("Paste");
            Action->setProperty("Action", "Paste");
            menu.addAction(Action);
        }

        if(menu.actions().count())
            menu.addSeparator();

        //Handling AllFiles display
        if (FileList.size()>1) {
            QAction* Action=new QAction("Fill all open files with this field value");
            Action->setProperty("Action", "Fill");
            menu.addAction(Action);
            menu.addSeparator();
        }

        //Handling Clear display
        if (!C->Get(FileName, Field).empty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
        {
            QAction* Action=new QAction("Clear this value");
            Action->setProperty("Action", "Clear");
            menu.addAction(Action);
            menu.addSeparator();
        }

        //Handling date display
        if (!Date.empty())
        {
            string Text;
            if (Field=="ICRD")
                Text="&Set ICRD to file creation timestamp ("+Date+")";
            else
                Text="&Set originationDate and Time to file creation timestamp ("+Date+")";

            QAction* Action=new QAction(QString().fromUtf8(Text.c_str()));
            Action->setProperty("Action", "Date");
            menu.addAction(Action);
            menu.addSeparator();
        }

        //Handling history display
        for (size_t Pos=History.size(); Pos>0; Pos--)
        {
            string Text=History[Pos-1].To_UTF8();
            if (!Text.empty())
            {
                QAction* Action=new QAction(QString().fromUtf8(Text.c_str()));
                Action->setProperty("Action", "History");
                menu.addAction(Action);
            }
        }

        //Displaying
        QAction* Action=menu.exec(globalPos);
        if (Action==NULL)
            return 0;

        //Filling
        if (Action->property("Action")=="Copy")
        {
            QApplication::clipboard()->setText(QString::fromUtf8(C->Get(FileName, Field).c_str()));
            return 0;
        }
        else if (Action->property("Action")=="Paste")
        {
            C->Set(FileName, Field, QApplication::clipboard()->text().toStdString());
            return 1;
        }
        else if (Action->property("Action")=="Fill")
        {
            string NewValue=C->Get(FileName, Field);
            for (size_t Pos=0; Pos<FileList.size(); Pos++)
            {
                if (FileList[Pos][0].empty())
                    continue;

                C->Set(FileList[Pos][0].To_UTF8(), Field, NewValue);
            }

            return FileList.size();
        }
        else if (Action->property("Action")=="Clear")
        {
            C->Set(FileName, Field, string());
            return 1;
        }
        else if (Action->property("Action")=="Date")
        {
            if (Field=="ICRD")
                C->Set(FileName, Field, Date);
            else
            {
                string OriginationDate=Date;
                OriginationDate.erase(10, 1+12);
                
                string OriginationTime=Date;
                OriginationTime.erase(0, 10+1);
                OriginationTime.erase(8, 4);

                C->Set(FileName, "OriginationDate", OriginationDate);
                C->Set(FileName, "OriginationTime", OriginationTime);
            }

            return 1;
        }
        else if (Action->property("Action")=="History")
        {
            C->Set(FileName, Field, Action->text().toStdString());
            return 1;
        }
    }
    else
    {
        //Clear data
        bool ShowClear=false;
        for (int Pos=0; Pos<forItems.size(); Pos++)
        {
            string FileName=forItems.at(Pos).first;
            string Field=forItems.at(Pos).second;
            if (!C->Get(FileName, Field).empty() && C->IsValid(FileName, Field, string()) && Field!="BextVersion")
            {
                ShowClear=true;
                break;
            }
        }

        //Creating menu
        QMenu menu;

        //Handling AllFiles display
        {
            QAction* Action=new QAction("Fill all open files with these fields values");
            Action->setProperty("Action", "Fill");
            menu.addAction(Action);
            menu.addSeparator();
        }

        //Handling Clear display
        if (ShowClear)
        {
            QAction* Action=new QAction("Clear these values");
            Action->setProperty("Action", "Clear");
            menu.addAction(Action);
        }

        //Displaying
        QAction* Action=menu.exec(globalPos);
        if (Action==NULL)
            return 0;

        //Filling
        if (Action->property("Action")=="Fill")
        {
            for (int Pos=0; Pos<forItems.size(); Pos++)
            {
                string FileName=forItems.at(Pos).first;
                string Field=forItems.at(Pos).second;
                string NewValue=C->Get(FileName, Field);
                for (size_t Pos=0; Pos<FileList.size(); Pos++)
                {
                    if (FileList[Pos][0].empty())
                        continue;

                    C->Set(FileList[Pos][0].To_UTF8(), Field, NewValue);
                }
            }

            return FileList.size();
        }
        else if (Action->property("Action")=="Clear")
        {
            for (int Pos=0; Pos<forItems.size(); Pos++)
            {
                string FileName=forItems.at(Pos).first;
                string Field=forItems.at(Pos).second;
                if (C->IsValid(FileName, Field, string()) && Field!="BextVersion")
                    C->Set(FileName, Field, string());
            }

            return forItems.size();
        }
    }

    return 0;
}
