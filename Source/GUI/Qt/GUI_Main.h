// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_MainH
#define GUI_MainH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <string>
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
class FileDrop;
class QWidget;
class QDropEvent;
class QDragEnterEvent;
class GUI_Preferences;
class QProgressDialog;
class QTimer;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main
//***************************************************************************

class GUI_Main : public QMainWindow
{
     Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main (Core* _C);
    ~GUI_Main ();

    //Actions
    void Menu_Update();
    void Bext_DefaultVersion_Set(unsigned short Version);
    void Bext_MaxVersion_Set(unsigned short Version);
    unsigned short Bext_MaxVersion_Get();
    bool Bext_Toggle_Get();
    void Bext_Toggle_Set(bool Version);
    bool Trace_UseDec_Get();
    void Trace_UseDec_Set(bool UseDec);
    void OpenSaveDirectory_Set(const string &Value);
    void BackupDirectory_Set(const string &Value);
    void LogFile_Set(const string &Value);

    //Preferences
    GUI_Preferences* Preferences;
    QMenu*          Menu_Edit;
    QMenu*          Menu_Fields_Main;
    QMenu**         Menu_Fields_Menus;
    QAction**       Menu_Fields_CheckBoxes;
    QAction**       Menu_Fields_RadioButtons;
    QActionGroup**  Menu_Fields_ActionGroups;

private:
    //Menu - List
    void Menu_Create();
    QMenu*          Menu_File;
    QAction*        Menu_File_Open_Files;
    QAction*        Menu_File_Open_Directory;
    QAction*        Menu_File_Close_Files;
    QAction*        Menu_File_Close_All;
    QAction*        Menu_File_Save_Files;
    QAction*        Menu_File_Save_All;
    QAction*        Menu_File_Undo;
    QAction*        Menu_File_Quit;
    QMenu*          Menu_View;
    QAction*        Menu_View_Log_Table;
    QAction*        Menu_View_Log_Text;
    QAction*        Menu_View_Output_stdall;
    QAction*        Menu_View_Output_stdout;
    QAction*        Menu_View_Output_stderr;
    QAction*        Menu_View_Output_Trace;
    QActionGroup*   Menu_View_Group;
    QMenu*          Menu_Import;
    QAction*        Menu_Import_Core_File;
    QAction*        Menu_Import_Core_XML;
    QAction*        Menu_Import_aXML_XML;
    QAction*        Menu_Import__PMX_XML;
    QAction*        Menu_Import_iXML_XML;
    QMenu*          Menu_Export;
    QAction*        Menu_Export_Unified_XML_Global;
    QAction*        Menu_Export_Technical_CSV_Global;
    QAction*        Menu_Export_Technical_XML_PerFile;
    QAction*        Menu_Export_Technical_XML_Global;
    QAction*        Menu_Export_Core_CSV_Global;
    QAction*        Menu_Export_Core_XML_PerFile;
    QAction*        Menu_Export_Core_XML_Global;
    QAction*        Menu_Export__PMX_XML_PerFile;
    QAction*        Menu_Export_aXML_XML_PerFile;
    QAction*        Menu_Export_iXML_XML_PerFile;
    QAction*        Menu_Export_cue__XML_PerFile;
    QMenu*          Menu_Options;
    QAction*        Menu_Options_ResetFieldSizes;
    QAction*        Menu_Options_Preferences;
    QMenu*          Menu_Help;
    QAction*        Menu_Help_Help;
    QAction*        Menu_Help_About;
    QAction*        Menu_Help_FADGI_Website;

public Q_SLOTS:
    //Menu - Actions
    void OnMenu_File_Open_Files                     ();
    void OnMenu_File_Open_Directory                 ();
    void OnMenu_File_Close_Files                    ();
    void OnMenu_File_Close_All                      ();
    void OnMenu_File_Save_File                      (const QString& FileName);
    void OnMenu_File_Save_Files                     ();
    void OnMenu_File_Save_All                       ();
    void OnMenu_File_Undo                           ();
    void OnMenu_View_Technical_Table                (bool);
    void OnMenu_View_Technical_Text                 (bool);
    void OnMenu_View_Core_Table                     (bool);
    void OnMenu_View_Core_Text                      (bool);
    void OnMenu_View_PerFile                        (bool);
    void OnMenu_View_Output_stdall                  ();
    void OnMenu_View_Output_stdout                  ();
    void OnMenu_View_Output_stderr                  ();
    void OnMenu_View_Output_Trace                   ();
    void OnMenu_Import_Core_File                    ();
    void OnMenu_Import_Core_XML                     ();
    void OnMenu_Import_aXML_XML                     ();
    void OnMenu_Import__PMX_XML                     ();
    void OnMenu_Import_iXML_XML                     ();
    void OnMenu_Export_Unified_XML_Global           ();
    void OnMenu_Export_Technical_CSV_Global         ();
    void OnMenu_Export_Technical_XML_Global         ();
    void OnMenu_Export_Technical_XML_PerFile        ();
    void OnMenu_Export_Core_CSV_Global              ();
    void OnMenu_Export_Core_XML_Global              ();
    void OnMenu_Export_Core_XML_PerFile             ();
    void OnMenu_Export__PMX_XML_PerFile             ();
    void OnMenu_Export_aXML_XML_PerFile             ();
    void OnMenu_Export_iXML_XML_PerFile             ();
    void OnMenu_Export_cue__XML_PerFile             ();
    void OnMenu_Rules_Tech3285_Req                  (bool);
    void OnMenu_Rules_Tech3285_Rec                  (bool);
    void OnMenu_Rules_CodingHistory_Rec             (bool);
    void OnMenu_Rules_CodingHistory_Rec_Ex_Comma    (bool);
    void OnMenu_Rules_CodingHistory_Rec_Ex_Analog   (bool);
    void OnMenu_Rules_CodingHistory_Rec_Ex_Frequency(bool);
    void OnMenu_Rules_CodingHistory_Rec_Ex_WordLength(bool);
    void OnMenu_Rules_OriginatorReference_Rec       (bool);
    void OnMenu_Rules_INFO_Req                      (bool);
    void OnMenu_Rules_INFO_Rec                      (bool);
    void OnMenu_Rules_FADGI_Rec                     (bool);
    void OnMenu_Rules_EBU_ISRC_Rec                  (bool);
    void OnMenu_Options_EncodingUTF8                (bool);
    void OnMenu_Options_EncodingCP437               (bool);
    void OnMenu_Options_EncodingCP850               (bool);
    void OnMenu_Options_EncodingCP858               (bool);
    void OnMenu_Options_EncodingCP1252              (bool);
    void OnMenu_Options_Encoding8859_1              (bool);
    void OnMenu_Options_Encoding8859_2              (bool);
    void OnMenu_Options_EncodingLocal               (bool);
    void OnMenu_Options_EncodingFallbackCP437       (bool);
    void OnMenu_Options_EncodingFallbackCP850       (bool);
    void OnMenu_Options_EncodingFallbackCP858       (bool);
    void OnMenu_Options_EncodingFallbackCP1252      (bool);
    void OnMenu_Options_EncodingFallback8859_1      (bool);
    void OnMenu_Options_EncodingFallback8859_2      (bool);
    void OnMenu_Options_Ignore_File_Encoding        (bool);
    void OnMenu_Options_Write_CodePage              (bool);
    void OnMenu_Options_TechCore                    (bool);
    void OnMenu_Options_riff2rf64_Reject            (bool);
    void OnMenu_Options_Overwrite_Reject            (bool);
    void OnMenu_Options_NoPadding_Accept            (bool);
    void OnMenu_Options_FileNotValid_Skip           (bool);
    void OnMenu_Options_WrongExtension_Skip         (bool);
    void OnMenu_Options_NewChunksAtTheEnd           (bool);
    void OnMenu_Options_GenerateMD5                 (bool);
    void OnMenu_Options_VerifyMD5                   (bool);
    void OnMenu_Options_EmbedMD5                    (bool);
    void OnMenu_Options_EmbedMD5_AuthorizeOverWritting (bool);
    void OnMenu_Options_SwapMD5Endianness           (bool);
    void OnMenu_Options_ResetFieldSizes             ();
    void OnMenu_Options_Preferences                 ();
    void OnMenu_Help_Help                           ();
    void OnMenu_Help_About                          ();
    void OnMenu_Help_FADGI_Website                  ();
    void OnOpen_Timer                               ();

private:
    //ToolBar - List
    QToolBar*  ToolBar;

    //ToolBar - Actions
    void ToolBar_Create();

private:
    //Non-GUI Elements
    Core* C;

    //Options
    bool        Bext_Toggle;
    string      LogFile;

    //GUI
    QWidget* View;
    enum view
    {
        View_None,
        View_PerFile,
        View_Technical_Table,
        View_Technical_Text,
        View_Core_Table,
        View_Core_Text,
        View_Output_stdall,
        View_Output_stdout,
        View_Output_stderr,
        View_Output_Trace,
    };
    view View_Current;
    bool MustCreate;

    void dragEnterEvent(QDragEnterEvent *Event);
    void dropEvent(QDropEvent *Event);
    void closeEvent(QCloseEvent *Event);
    QProgressDialog* ProgressDialog;
    QTimer* Timer;

    //Helpers
    void View_Refresh(view View_New=View_None);
    bool Close(const std::string &FileName=std::string(), bool AndExit=false);
    std::string* ToClose_FileName;
    bool Exit;
    enum open_timer_init
    {
        Timer_Open_Files,
        Timer_Open_Directory,
        Timer_DragAndDrop,
        Timer_Save,
    };
    void Open_Timer_Init (open_timer_init Action);
    open_timer_init Open_Timer_Action;

    bool ShouldApplyDarkTheme();
};

#endif
