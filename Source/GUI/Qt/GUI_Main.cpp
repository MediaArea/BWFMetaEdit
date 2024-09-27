// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
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
#include "GUI/Qt/GUI_Main_PerFile.h"
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

#if defined(__APPLE__) && QT_VERSION < 0x050400
#include <CoreFoundation/CFURL.h>
#endif

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
    Exit=false;
    ToClose_FileName=NULL;

    //Options
    Bext_Toggle=false;

    //Configure
    View_Current=View_PerFile;
    MustCreate=true;

    //GUI
    View=NULL;

    // Set minimum window size to reasonable value
    setMinimumSize(640, 480);

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
                case Option_DefaultView_PerFile     : Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile   ]->setChecked(true); break;
                default                             : ;
            }
    }

    for (size_t Option=0; Option<Preferences->Group_Options_Count_Get(Group_Encoding); Option++)
    {
        if (Preferences->Group_Option_Checked_Get(Group_Encoding, Option))
            switch (Option)
            {
                case Option_Encoding_UTF8   : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_UTF8]->setChecked(true); break;
                case Option_Encoding_CP437  : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP437]->setChecked(true); break;
                case Option_Encoding_CP850  : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP850]->setChecked(true); break;
                case Option_Encoding_CP858  : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP858]->setChecked(true); break;
                case Option_Encoding_CP1252 : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP1252]->setChecked(true); break;
                case Option_Encoding_8859_1 : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_1]->setChecked(true); break;
                case Option_Encoding_8859_2 : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_2]->setChecked(true); break;
                case Option_Encoding_Local  : Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_Local]->setChecked(true); break;
                default                     : ;
            }
    }

    for (size_t Option=0; Option<Preferences->Group_Options_Count_Get(Group_Encoding_Fallback); Option++)
    {
        if (Preferences->Group_Option_Checked_Get(Group_Encoding_Fallback, Option))
            switch (Option)
            {
                case Option_Encoding_Fallback_CP437  : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP437]->setChecked(true); break;
                case Option_Encoding_Fallback_CP850  : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP850]->setChecked(true); break;
                case Option_Encoding_Fallback_CP858  : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP858]->setChecked(true); break;
                case Option_Encoding_Fallback_CP1252 : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP1252]->setChecked(true); break;
                case Option_Encoding_Fallback_8859_1 : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_1]->setChecked(true); break;
                case Option_Encoding_Fallback_8859_2 : Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_2]->setChecked(true); break;
                default                     : ;
            }
    }

    Trace_UseDec_Set(Preferences->Group_Option_Checked_Get(Group_Trace, Option_Trace_UseDec));

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
    setWindowTitle("BWF MetaEdit - Audio-Visual Working Group of the Federal Agencies Digital Guidelines Initiative");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));
    setUnifiedTitleAndToolBarOnMac(false); // BUG: black toolbar on PerFile view with true
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
            case View_PerFile                   : View=new GUI_Main_PerFile         (C,    this); MustCreate=false; break;
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
    ZtringList Files;
    ZtringList Cores;

    if (Event->mimeData()->hasUrls())
    {

        //foreach (QUrl url, Event->mimeData()->urls())
        QList<QUrl> urls=Event->mimeData()->urls();
        for (int Pos=0; Pos<urls.size(); Pos++)
        {
            Ztring File;
#if defined(__APPLE__) && QT_VERSION < 0x050400
            if (urls[Pos].url().startsWith("file:///.file/id="))
            {
                CFErrorRef Error = 0;
                CFURLRef Cfurl = urls[Pos].toCFURL();
                CFURLRef Absurl = CFURLCreateFilePathURL(kCFAllocatorDefault, Cfurl, &Error);

                if(Error)
                    continue;

                File.From_UTF8(QUrl::fromCFURL(Absurl).toLocalFile().toUtf8().data());
                CFRelease(Cfurl);
                CFRelease(Absurl);
            }
            else
            {
                File.From_UTF8(urls[Pos].toLocalFile().toUtf8().data());
            }
#else
            File.From_UTF8(urls[Pos].toLocalFile().toUtf8().data());
#endif

            #ifdef __WINDOWS__
                File.FindAndReplace(__T("/"), __T("\\"), 0, Ztring_Recursive);
            #endif // __WINDOWS__
            if (File.size()>=4 && (File.rfind(__T(".csv"),File.size()-4, 4)!=string::npos || File.rfind(__T(".xml"),File.size()-4, 4)!=string::npos))
                Cores.push_back(File);
            else
                Files.push_back(File);
        }
    }

    if (Cores.size())
    {
        //Configuring
        C->StdOut("Importing Core file...");
        for (size_t Pos=0; Pos<Cores.size(); Pos++)
            C->Menu_File_Import_Core(Cores[Pos].To_UTF8());
        C->StdOut("Importing Core file, finished");
        //Showing
        if (C->Text_stderr_Updated_Get())
        {
            Menu_View_Output_stderr->setChecked(true);
            View_Refresh(View_Output_stderr);
        }
        else
            View_Refresh();
    }

    if (Files.size())
    {
        //Configuring
        C->StdOut("Opening files...");
        C->Menu_File_Open_Files_Begin();
        for (size_t Pos=0; Pos<Files.size(); Pos++)
            C->Menu_File_Open_Files_Continue(Files[Pos].To_UTF8());
        //Showing
        Open_Timer_Init(Timer_DragAndDrop);
        C->Menu_File_Open_Files_Finish_Start();
        C->Menu_File_Open_Files_Finish_Middle_Threaded();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::closeEvent(QCloseEvent *Event)
{
    if (Exit || Close(string(), true))
        Event->accept();
    else
        Event->ignore();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main::Close(const string &FileName, bool AndExit)
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
            MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Save    : // Save was clicked
                                            OnMenu_File_Save_All();
                                            Exit=AndExit;
                                            return false; // Work in progress in another thread
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
            MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Save    : // Save was clicked
                                            OnMenu_File_Save_All();
                                            ToClose_FileName=new string(FileName);
                                            return false; // work in progress in another thread;
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
        string Text="Close "+List.Read().To_UTF8();
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
        string Text="Close "+Ztring::ToZtring(List.size()).To_UTF8()+" files";
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
bool GUI_Main::Trace_UseDec_Get()
{
    return C->Trace_UseDec;
}

//---------------------------------------------------------------------------
void GUI_Main::Trace_UseDec_Set(bool UseDec)
{
    C->Trace_UseDec=UseDec;
    C->Menu_File_Options_Update();
}

//---------------------------------------------------------------------------
void GUI_Main::OpenSaveDirectory_Set(const string &Value)
{
    C->OpenSaveFolder=Value;
}

//---------------------------------------------------------------------------
void GUI_Main::BackupDirectory_Set(const string &Value)
{
    C->ApplicationFolder=Ztring().From_UTF8(Value);
}

//---------------------------------------------------------------------------
void GUI_Main::LogFile_Set(const string &Value)
{
    C->LogFile=Ztring().From_UTF8(Value);
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
                                          C->StdOut("Saving files, finished");
                                          if (ToClose_FileName)
                                          {
                                              if (C->Text_stderr.str().empty())
                                                  C->Menu_File_Close_File(*ToClose_FileName);
                                              delete ToClose_FileName;
                                              ToClose_FileName=NULL;
                                          }
                                          if (Exit)
                                          {
                                              if (C->Text_stderr.str().empty())
                                              {
                                                   C->Menu_File_Close_All();
                                                   close();
                                              }
                                              else
                                                  Exit=false;
                                          }
                                          break;
            default                     : C->StdOut("???, finished"); break;
        }
    if (View_Current==View_PerFile)
        View->setVisible(true);

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
    if (View_Current==View_PerFile)
        View->setVisible(false);

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

bool GUI_Main::ShouldApplyDarkTheme()
{
    const QPalette DefaultPalette;
    return DefaultPalette.color(QPalette::WindowText).lightness() > DefaultPalette.color(QPalette::Window).lightness();
}
