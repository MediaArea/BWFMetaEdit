// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main.h"
#include "Common/Core.h"
#include <vector>
#include <algorithm>
#include <QStatusBar>
#include <QTextEdit>
#include "GUI/Qt/GUI_Main_Technical_Table.h"
#include "GUI/Qt/GUI_Main_Technical_Text.h"
#include "GUI/Qt/GUI_Main_Core_Table.h"
#include "GUI/Qt/GUI_Main_Core_Text.h"
#include "GUI/Qt/GUI_Main_Output_Log.h"
#include "GUI/Qt/GUI_Main_Output_Trace.h"
#include "GUI/Qt/GUI_Preferences.h"
#include <QEvent>
#include <QMimeData>
#include <QUrl>
#include <QApplication>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QProgressDialog>
#include <QThread>
#include <QTimer>
#include "ZenLib/Ztring.h"
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
GUI_Main::GUI_Main(Core* _C)
{
    //Internal
    C=_C;

    //Options
    Bext_Toggle=false;

    //Configure
    View_Current=View_Core_Table;
    MustCreate=true;

    //GUI
    View=NULL;

    //Menu and ToolBar
    Menu_Create();
    ToolBar_Create();

    //Timer
    ProgressDialog=NULL;
    Timer=NULL;

    //CenterOnScreen();
    setStatusBar(new QStatusBar());
    move(40, (QApplication::desktop()->screenGeometry().height()-440)/2);
    resize(QApplication::desktop()->screenGeometry().width()-80, 440);

    //Drag n drop
    setAcceptDrops(true);

    //Defaults
    for (size_t Option=0; Option<Preferences->Group_Options_Count_Get(Group_DefaultView); Option++)
    {
        if (Preferences->Group_Option_Checked_Get(Group_DefaultView, Option))
            switch (Option)
            {
                case Option_DefaultView_Tech_Table  : Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]->setChecked(true); break;
                case Option_DefaultView_Tech_Text   : Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text ]->setChecked(true); break;
                case Option_DefaultView_Core_Table  : Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]->setChecked(true); break;
                case Option_DefaultView_Core_Text   : Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text ]->setChecked(true); break;
                default                             : ;
            }
    }
    
    /*
    Menu_View_Technical_Table->setChecked(true);
    emit OnMenu_View_Technical_Table();
    Menu_Rules_BWF_Req->setChecked(true);
    emit OnMenu_Rules_BWF_Req();
    Menu_Rules_INFO_Req->setChecked(true);
    emit OnMenu_Rules_INFO_Req();
    Menu_Options_WrongExtension_Skip->setChecked(true);
    emit OnMenu_Options_WrongExtension_Skip();
    */
    
    //GUI
    setWindowTitle("BWF MetaEdit - Audio-Visual Working Group of the Federal Agencies Digitization Guidelines Initiative");
    setWindowIcon (QIcon(":/Image/FADGI/Logo.png"));
    setUnifiedTitleAndToolBarOnMac(true);
}

//---------------------------------------------------------------------------
GUI_Main::~GUI_Main()
{
}

//***************************************************************************
// View_Refresh
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main::View_Refresh(view View_New)
{
    if (View_New!=View_None)
    {
        View_Current=View_New;
        MustCreate=true;
    }
        
    if (MustCreate)
    {
        switch (View_Current)
        {
            case View_Technical_Table           : View=new GUI_Main_Technical_Table (C,    this); MustCreate=false; break;
            case View_Technical_Text            : View=new GUI_Main_Technical_Text  (C,    this); MustCreate=false; break;
            case View_Core_Table                : View=new GUI_Main_Core_Table      (C,    this); MustCreate=false; break;
            case View_Core_Text                 : View=new GUI_Main_Core_Text       (C,    this); MustCreate=false; break;
            case View_Output_stdall             : View=new GUI_Main_Output_Log      (C, 3, this); MustCreate=false; break;
            case View_Output_stdout             : View=new GUI_Main_Output_Log      (C, 1, this); MustCreate=false; break;
            case View_Output_stderr             : View=new GUI_Main_Output_Log      (C, 2, this); MustCreate=false; break;
            case View_Output_Trace              : View=new GUI_Main_Trace           (C,    this); MustCreate=false; break;
            default                             : View=NULL;
        }

        setCentralWidget(View);
    }

    if (View)
    {
        QEvent Event(QEvent::User);
        QApplication::sendEvent(View, &Event);
    }
    
    //Menu
    Menu_Update();

    //Bug in Qt? With somes tables, dimensionning is not done. Hacking for emiting resizing.
    QSize S=size();
    S.setHeight(S.height()+1);
    resize(S);
    S.setHeight(S.height()-1);
    resize(S);
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main::dragEnterEvent(QDragEnterEvent *Event)
{
    Event->acceptProposedAction();
}
 
void GUI_Main::dropEvent(QDropEvent *Event)
{
    //Configuring
    C->StdOut("Opening files...");
    C->Menu_File_Open_Files_Begin();
    const QMimeData* Data=Event->mimeData ();
    if (Event->mimeData()->hasUrls())
    {
        //foreach (QUrl url, Event->mimeData()->urls())
        QList<QUrl> urls=Event->mimeData()->urls();
        for (int Pos=0; Pos<urls.size(); Pos++)
        {
            Ztring File; File.From_UTF8(urls[Pos].toLocalFile().toUtf8().data());
            #ifdef __WINDOWS__
                File.FindAndReplace("/", "\\", 0, Ztring_Recursive);
            #endif // __WINDOWS__
            C->Menu_File_Open_Files_Continue(File);
        }
    }

    //Showing
    C->Menu_File_Open_Files_Finish_Start();
    C->Menu_File_Open_Files_Finish_Middle_Threaded();
    Open_Timer_Init(Timer_DragAndDrop);
}

//---------------------------------------------------------------------------
void GUI_Main::closeEvent(QCloseEvent *Event)
{
    if (Close())
        Event->accept();
    else
        Event->ignore();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main::Close(const string &FileName)
{
    if (FileName.empty())
    {
        if (C->Files_Modified_NotWritten_Count>0)
        {
            QMessageBox MessageBox;
            MessageBox.setWindowTitle("BWF MetaEdit");
            MessageBox.setText("Files have been modified.");
            MessageBox.setInformativeText("Do you want to save your changes?");
            #if (QT_VERSION >= 0x040200)
                MessageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                #if (QT_VERSION >= 0x040300)
                    MessageBox.setDefaultButton(QMessageBox::Save);
                #endif // (QT_VERSION >= 0x040300)
            #endif // (QT_VERSION >= 0x040200)
            MessageBox.setIcon(QMessageBox::Warning);
            MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Save    : // Save was clicked
                                            OnMenu_File_Save_All();
                                            if (!C->Text_stderr.str().empty())
                                                return false; // Errors were present, showing errors
                                            else
                                            {
                                                C->Menu_File_Close_All();
                                                return true;
                                            }
                                            break;
                case QMessageBox::Discard : // Don't Save was clicked
                                            C->Menu_File_Close_All();
                                            return true;
                                            break;
                case QMessageBox::Cancel  : // Cancel was clicked
                                            return false;
                                            break;
                default:                    return false; // Should never be reached
            } 
        }
        else
        {
            C->Menu_File_Close_All();
            return true;
        }
    }
    else
    {
        if (C->IsModified_Get(FileName))
        {
            QMessageBox MessageBox;
            MessageBox.setWindowTitle("BWF MetaEdit");
            MessageBox.setText((FileName+" has been modified.").c_str());
            MessageBox.setInformativeText("Do you want to save your changes?");
            #if (QT_VERSION >= 0x040200)
                MessageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                #if (QT_VERSION >= 0x040300)
                    MessageBox.setDefaultButton(QMessageBox::Save);
                #endif // (QT_VERSION >= 0x040300)
            #endif // (QT_VERSION >= 0x040200)
            MessageBox.setIcon(QMessageBox::Warning);
            MessageBox.setWindowIcon(QIcon(":/Image/FADGI/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Save    : // Save was clicked
                                            OnMenu_File_Save_All();
                                            if (!C->Text_stderr.str().empty())
                                                return false; // Errors were present, showing errors
                                            else
                                            {
                                                C->Menu_File_Close_File(FileName);
                                                return true;
                                            }
                                            break;
                case QMessageBox::Discard : // Don't Save was clicked
                                            C->Menu_File_Close_File(FileName);
                                            return true;
                                            break;
                case QMessageBox::Cancel  : // Cancel was clicked
                                            return false;
                                            break;
                default:                    return false; // Should never be reached
            } 
        }
        else
        {
            C->Menu_File_Close_File(FileName);
            return true;
        }
    }
}

//---------------------------------------------------------------------------
void GUI_Main::Menu_Update()
{
    //File_Backup
    Menu_File_Undo->setEnabled(C->Menu_File_Undo_BackupFilesExist());
    
    //File_Save
    ZtringList List=C->Menu_File_Close_File_FileName_Get();
    if (List.empty())
    {
        Menu_File_Close_Files->setText("Close");
        Menu_File_Close_Files->setEnabled(false);
    }
    else if (List.size()==1)
    {
        Ztring Text="Close "+List.Read();
        Menu_File_Close_Files->setText(Text.c_str());
        Menu_File_Close_Files->setEnabled(true);
    }
    else if (!C->Menu_File_Close_All_ShouldBeDisplayed())
    {
        Menu_File_Close_Files->setText("Close all files");
        Menu_File_Close_Files->setEnabled(true);
    }
    else
    {
        Ztring Text="Close "+Ztring::ToZtring(List.size())+" files";
        Menu_File_Close_Files->setText(Text.c_str());
        Menu_File_Close_Files->setEnabled(true);
    }
    Menu_File_Close_All->setVisible(C->Menu_File_Close_All_ShouldBeDisplayed());
    Menu_File_Close_All->setEnabled(C->Menu_File_Close_All_ShouldBeEnabled());
    Menu_File_Save_All->setEnabled(C->Files_Modified_NotWritten_Count?true:false);

    //Options_ResetFieldSizes
    switch (View_Current)
    {
        case View_Technical_Table           : 
        case View_Core_Table                : Menu_Options_ResetFieldSizes->setVisible(true); break;
        default                             : Menu_Options_ResetFieldSizes->setVisible(false);
    }
}

//---------------------------------------------------------------------------
void GUI_Main::Bext_DefaultVersion_Set(unsigned short Version)
{
    C->Bext_DefaultVersion=Version;
}

//---------------------------------------------------------------------------
unsigned short GUI_Main::Bext_MaxVersion_Get()
{
    return C->Bext_MaxVersion;
}

//---------------------------------------------------------------------------
void GUI_Main::Bext_MaxVersion_Set(unsigned short Max)
{
    C->Bext_MaxVersion=Max;
}

//---------------------------------------------------------------------------
bool GUI_Main::Bext_Toggle_Get()
{
    return Bext_Toggle;
}

//---------------------------------------------------------------------------
void GUI_Main::Bext_Toggle_Set(bool Toggle)
{
    Bext_Toggle=Toggle;
}

//---------------------------------------------------------------------------
void GUI_Main::BackupDirectory_Set(const string &Value)
{
    C->ApplicationFolder=Value;
}

//---------------------------------------------------------------------------
void GUI_Main::LogFile_Set(const string &Value)
{
    C->LogFile=Value;
}

//---------------------------------------------------------------------------
void GUI_Main::OnOpen_Timer ()
{
    if (ProgressDialog==NULL)
        return;    
        
    float Result=C->Menu_File_Open_State();
    ProgressDialog->setValue((int)(Result*100));

    if (Result==1.0 || ProgressDialog->wasCanceled())
    {
        ProgressDialog->hide();
        Timer->stop();

        delete ProgressDialog; ProgressDialog=NULL;
        delete Timer; Timer=NULL;

        switch (Open_Timer_Action)
        {
            case Timer_Open_Files       : C->Menu_File_Open_Files_Finish_End();
                                          C->StdOut("Opening files, finished");
                                          break;
            case Timer_Open_Directory   : C->Menu_File_Open_Files_Finish_End();
                                          C->StdOut("Opening directory, finished");
                                          break;
            case Timer_DragAndDrop      : C->Menu_File_Open_Files_Finish_End();
                                          C->StdOut("Opening files, finished");
                                          break;
            case Timer_Save             : C->Menu_File_Save_End();
                                          C->StdOut("Saving files, finished"); break;
            default                     : C->StdOut("???, finished"); break;
        }

        //Showing
        if (C->Text_stderr_Updated_Get())
        {
            Menu_View_Output_stderr->setChecked(true);
            View_Refresh(View_Output_stderr);
        }
        else
            View_Refresh();
    }
}     
     
//---------------------------------------------------------------------------
void GUI_Main::Open_Timer_Init (open_timer_init Action)
{
    Open_Timer_Action=Action;
    
        switch (Open_Timer_Action)
        {
            case Timer_Open_Files       : ProgressDialog=new QProgressDialog("Opening files...", "Abort Opening", 0, 100, this);
                                          break;
            case Timer_Open_Directory   : ProgressDialog=new QProgressDialog("Opening directory...", "Abort Opening", 0, 100, this);
                                          break;
            case Timer_DragAndDrop      : ProgressDialog=new QProgressDialog("Opening files...", "Abort Opening", 0, 100, this);
                                          break;
            case Timer_Save             : ProgressDialog=new QProgressDialog("Saving files...", "Abort Saving", 0, 100, this);
                                          break;
            default                     : ProgressDialog=new QProgressDialog("???...", "Abort ???", 0, 100, this);
        }


    
    ProgressDialog->setWindowModality(Qt::WindowModal);
    ProgressDialog->setMinimumDuration(0);
    ProgressDialog->setWindowTitle("BWF MetaEdit");

    if (Timer==NULL)
    {
        Timer=new QTimer();
        connect(Timer, SIGNAL(timeout()), this, SLOT(OnOpen_Timer()));
        Timer->start(100);
    }
}     
