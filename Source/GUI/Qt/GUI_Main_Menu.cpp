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
#include "GUI/Qt/GUI_Main_UndoDialog.h"
#include "Common/Core.h"
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QListWidget>
#include <QThread>
#include <QUrl>
#include <QEvent>
#include "GUI/Qt/GUI_Preferences.h"
#include "GUI/Qt/GUI_Help.h"
#include "GUI/Qt/GUI_About.h"
#include "ZenLib/ZtringList.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Menu creation
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main::Menu_Create()
{
    bool Dark=ShouldApplyDarkTheme();

    //Menu Files
    Menu_File_Open_Files = new QAction(QIcon(":/Image/Menu/File_Open_File.png"), tr("Open file(s)..."), this);
    Menu_File_Open_Files->setShortcut(tr("Ctrl+F"));
    Menu_File_Open_Files->setStatusTip(tr("Open file(s)"));
    connect(Menu_File_Open_Files, SIGNAL(triggered()), this, SLOT(OnMenu_File_Open_Files()));

    Menu_File_Open_Directory = new QAction(QIcon(":/Image/Menu/File_Open_Directory.png"), tr("Open Directory..."), this);
    Menu_File_Open_Directory->setShortcut(tr("Ctrl+D"));
    Menu_File_Open_Directory->setStatusTip(tr("Open directory"));
    connect(Menu_File_Open_Directory, SIGNAL(triggered()), this, SLOT(OnMenu_File_Open_Directory()));

    Menu_File_Close_Files = new QAction(tr("Close"), this);
    Menu_File_Close_Files->setShortcut(tr("Ctrl+W"));
    Menu_File_Close_Files->setStatusTip(tr(""));
    connect(Menu_File_Close_Files, SIGNAL(triggered()), this, SLOT(OnMenu_File_Close_Files()));

    Menu_File_Close_All = new QAction(tr("Close all files"), this);
    Menu_File_Close_All->setStatusTip(tr(""));
    connect(Menu_File_Close_All, SIGNAL(triggered()), this, SLOT(OnMenu_File_Close_All()));

    Menu_File_Save_All = new QAction(QIcon(Dark?":/Image/Menu/File_Save_Dark.png":":/Image/Menu/File_Save.png"), tr("Save all modified files"), this);
    Menu_File_Save_All->setEnabled(false);
    connect(Menu_File_Save_All, SIGNAL(triggered()), this, SLOT(OnMenu_File_Save_All()));

    Menu_File_Undo = new QAction(tr("Undo last save operations..."), this);
    connect(Menu_File_Undo, SIGNAL(triggered()), this, SLOT(OnMenu_File_Undo()));

    Menu_File_Quit = new QAction(tr("Q&uit"), this);
    Menu_File_Quit->setShortcut(tr("Ctrl+Q"));
    Menu_File_Quit->setStatusTip(tr("Quit the application"));
    connect(Menu_File_Quit, SIGNAL(triggered()), this, SLOT(close()));

    Menu_File = menuBar()->addMenu(tr("&File"));
    Menu_File->addAction(Menu_File_Open_Files);
    Menu_File->addAction(Menu_File_Open_Directory);
    Menu_File->addAction(Menu_File_Close_Files);
    Menu_File->addAction(Menu_File_Close_All);
    Menu_File->addSeparator();
    Menu_File->addAction(Menu_File_Save_All);
    Menu_File->addSeparator();
    Menu_File->addAction(Menu_File_Undo);
    Menu_File->addSeparator();
    Menu_File->addAction(Menu_File_Quit);

    //Menu Edit
    Menu_Edit = menuBar()->addMenu(tr("&Edit"));
    Menu_Edit->setEnabled(false);

    //Menu Views
    Menu_Fields_RadioButtons=new QAction*[Preferences->Groups_Count_Get()*options::MaxCount];

    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile] = new QAction(QIcon(Dark?":/Image/Menu/View_PerFile_Dark.png":":/Image/Menu/View_PerFile.png"), tr("File"), this);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]->setShortcut(tr(""));
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]->setCheckable(true);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]->setStatusTip(tr(""));
    connect(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile], SIGNAL(toggled(bool)), this, SLOT(OnMenu_View_PerFile(bool)));

    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table] = new QAction(QIcon(Dark?":/Image/Menu/View_Technical_Dark.png":":/Image/Menu/View_Technical.png"), tr("Technical Metadata (table)"), this);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]->setShortcut(tr(""));
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]->setCheckable(true);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]->setStatusTip(tr(""));
    connect(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table], SIGNAL(toggled(bool)), this, SLOT(OnMenu_View_Technical_Table(bool)));

    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text] = new QAction(tr("Technical Metadata (text)"), this);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text]->setShortcut(tr(""));
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text]->setCheckable(true);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text]->setStatusTip(tr(""));
    connect(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text], SIGNAL(toggled(bool)), this, SLOT(OnMenu_View_Technical_Text(bool)));

    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table] = new QAction(QIcon(Dark?":/Image/Menu/View_Core_Dark.png":":/Image/Menu/View_Core.png"), tr("Core Metadata (table)"), this);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]->setShortcut(tr(""));
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]->setCheckable(true);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]->setStatusTip(tr(""));
    connect(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table], SIGNAL(toggled(bool)), this, SLOT(OnMenu_View_Core_Table(bool)));

    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text] = new QAction(tr("Core Metadata (text)"), this);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text]->setShortcut(tr(""));
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text]->setCheckable(true);
    Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text]->setStatusTip(tr(""));
    connect(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text], SIGNAL(toggled(bool)), this, SLOT(OnMenu_View_Core_Text(bool)));

    Menu_View_Output_stdall = new QAction(tr("Log of operations and errors"), this);
    Menu_View_Output_stdall->setShortcut(tr(""));
    Menu_View_Output_stdall->setCheckable(true);
    Menu_View_Output_stdall->setStatusTip(tr(""));
    connect(Menu_View_Output_stdall, SIGNAL(triggered()), this, SLOT(OnMenu_View_Output_stdall()));

    Menu_View_Output_stdout = new QAction(tr("Log of operations only"), this);
    Menu_View_Output_stdout->setShortcut(tr(""));
    Menu_View_Output_stdout->setCheckable(true);
    Menu_View_Output_stdout->setStatusTip(tr(""));
    connect(Menu_View_Output_stdout, SIGNAL(triggered()), this, SLOT(OnMenu_View_Output_stdout()));

    Menu_View_Output_stderr = new QAction(tr("Log of errors only"), this);
    Menu_View_Output_stderr->setShortcut(tr(""));
    Menu_View_Output_stderr->setCheckable(true);
    Menu_View_Output_stderr->setStatusTip(tr(""));
    connect(Menu_View_Output_stderr, SIGNAL(triggered()), this, SLOT(OnMenu_View_Output_stderr()));

    Menu_View_Output_Trace = new QAction(tr("Trace"), this);
    Menu_View_Output_Trace->setShortcut(tr(""));
    Menu_View_Output_Trace->setCheckable(true);
    Menu_View_Output_Trace->setStatusTip(tr(""));
    connect(Menu_View_Output_Trace, SIGNAL(triggered()), this, SLOT(OnMenu_View_Output_Trace()));

    Menu_View_Group = new QActionGroup(this);
    Menu_View_Group->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]);
    Menu_View_Group->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]);
    Menu_View_Group->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text]);
    Menu_View_Group->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]);
    Menu_View_Group->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text]);
    Menu_View_Group->addAction(Menu_View_Output_stdall);
    Menu_View_Group->addAction(Menu_View_Output_stdout);
    Menu_View_Group->addAction(Menu_View_Output_stderr);
    Menu_View_Group->addAction(Menu_View_Output_Trace);
    Menu_View = menuBar()->addMenu(tr("&View"));
    Menu_View->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]);
    Menu_View->addSeparator();
    Menu_View->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]);
    Menu_View->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Text]);
    Menu_View->addSeparator();
    Menu_View->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]);
    Menu_View->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Text]);
    Menu_View->addSeparator();
    Menu_View->addAction(Menu_View_Output_stdall);
    Menu_View->addAction(Menu_View_Output_stdout);
    Menu_View->addAction(Menu_View_Output_stderr);
    Menu_View->addAction(Menu_View_Output_Trace);
    Menu_View_Group = new QActionGroup(this);

    //Menu Import
    Menu_Import_Core_File = new QAction(tr("Core Document..."), this);
    Menu_Import_Core_File->setStatusTip(tr("load Core data from a CSV or XML file"));
    connect(Menu_Import_Core_File, SIGNAL(triggered()), this, SLOT(OnMenu_Import_Core_File()));

    Menu_Import_Core_XML = new QAction(tr("Core (one XML file per input)"), this);
    Menu_Import_Core_XML->setStatusTip(tr(""));
    connect(Menu_Import_Core_XML, SIGNAL(triggered()), this, SLOT(OnMenu_Import_Core_XML()));

    Menu_Import__PMX_XML = new QAction(tr("XMP (one XML file per input)"), this);
    Menu_Import__PMX_XML->setStatusTip(tr(""));
    connect(Menu_Import__PMX_XML, SIGNAL(triggered()), this, SLOT(OnMenu_Import__PMX_XML()));

    Menu_Import_aXML_XML = new QAction(tr("aXML (one XML file per input)"), this);
    Menu_Import_aXML_XML->setStatusTip(tr(""));
    connect(Menu_Import_aXML_XML, SIGNAL(triggered()), this, SLOT(OnMenu_Import_aXML_XML()));

    Menu_Import_iXML_XML = new QAction(tr("iXML (one XML file per input)"), this);
    Menu_Import_iXML_XML->setStatusTip(tr(""));
    connect(Menu_Import_iXML_XML, SIGNAL(triggered()), this, SLOT(OnMenu_Import_iXML_XML()));

    Menu_Import = menuBar()->addMenu(tr("&Import"));
    Menu_Import->addAction(Menu_Import_Core_File);
    //Menu_Import->addAction(Menu_Import_Core_XML);
    //Menu_Import->addSeparator();
    //Menu_Import->addAction(Menu_Import__PMX_XML);
    //Menu_Import->addSeparator();
    //Menu_Import->addAction(Menu_Import_aXML_XML);
    //Menu_Import->addSeparator();
    //Menu_Import->addAction(Menu_Import_iXML_XML);

    //Menu Export
    Menu_Export_Unified_XML_Global = new QAction(tr("Unified XML Metadata (one global XML file)..."), this);
    Menu_Export_Unified_XML_Global->setStatusTip(tr("Save All Metadata in a global XML file"));
    connect(Menu_Export_Unified_XML_Global, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Unified_XML_Global()));

    Menu_Export_Technical_CSV_Global = new QAction(tr("Technical Metadata (one global CSV file)..."), this);
    Menu_Export_Technical_CSV_Global->setStatusTip(tr("Save Technical Metadata in a global CSV file"));
    connect(Menu_Export_Technical_CSV_Global, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Technical_CSV_Global()));

    Menu_Export_Technical_XML_Global = new QAction(tr("Technical Metadata (one global XML file)..."), this);
    Menu_Export_Technical_XML_Global->setStatusTip(tr("Save Technical Metadata in a global XML file"));
    connect(Menu_Export_Technical_XML_Global, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Technical_XML_Global()));

    Menu_Export_Technical_XML_PerFile = new QAction(tr("Technical Metadata (one XML file per WAV file)"), this);
    Menu_Export_Technical_XML_PerFile->setStatusTip(tr(""));
    Menu_Export_Technical_XML_PerFile->setVisible(true);
    connect(Menu_Export_Technical_XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Technical_XML_PerFile()));

    Menu_Export_Core_CSV_Global = new QAction(tr("Core Document (one global CSV file)..."), this);
    Menu_Export_Core_CSV_Global->setStatusTip(tr("Save Core data in a global CSV file"));
    connect(Menu_Export_Core_CSV_Global, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Core_CSV_Global()));

    Menu_Export_Core_XML_Global = new QAction(tr("Core Document (one global XML file)..."), this);
    Menu_Export_Core_XML_Global->setStatusTip(tr("Save Core data in a global XML file"));
    connect(Menu_Export_Core_XML_Global, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Core_XML_Global()));

    Menu_Export_Core_XML_PerFile = new QAction(tr("Core Document (one XML file per WAV file)"), this);
    Menu_Export_Core_XML_PerFile->setStatusTip(tr(""));
    Menu_Export_Core_XML_PerFile->setVisible(true);
    connect(Menu_Export_Core_XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export_Core_XML_PerFile()));

    Menu_Export__PMX_XML_PerFile = new QAction(tr("XMP (one XML file per WAV file)"), this);
    Menu_Export__PMX_XML_PerFile->setStatusTip(tr(""));
    connect(Menu_Export__PMX_XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export__PMX_XML_PerFile()));

    Menu_Export_aXML_XML_PerFile = new QAction(tr("aXML (one XML file per WAV file)"), this);
    Menu_Export_aXML_XML_PerFile->setStatusTip(tr(""));
    connect(Menu_Export_aXML_XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export_aXML_XML_PerFile()));

    Menu_Export_iXML_XML_PerFile = new QAction(tr("iXML (one XML file per WAV file)"), this);
    Menu_Export_iXML_XML_PerFile->setStatusTip(tr(""));
    connect(Menu_Export_iXML_XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export_iXML_XML_PerFile()));

    Menu_Export_cue__XML_PerFile = new QAction(tr("cue (one XML file per WAV file)"), this);
    Menu_Export_cue__XML_PerFile->setStatusTip(tr(""));
    connect(Menu_Export_cue__XML_PerFile, SIGNAL(triggered()), this, SLOT(OnMenu_Export_cue__XML_PerFile()));

    Menu_Export = menuBar()->addMenu(tr("E&xport"));
    Menu_Export->addAction(Menu_Export_Unified_XML_Global);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export_Technical_CSV_Global);
    Menu_Export->addAction(Menu_Export_Technical_XML_Global);
    Menu_Export->addAction(Menu_Export_Technical_XML_PerFile);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export_Core_CSV_Global);
    Menu_Export->addAction(Menu_Export_Core_XML_Global);
    Menu_Export->addAction(Menu_Export_Core_XML_PerFile);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export__PMX_XML_PerFile);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export_aXML_XML_PerFile);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export_iXML_XML_PerFile);
    Menu_Export->addSeparator();
    Menu_Export->addAction(Menu_Export_cue__XML_PerFile);

    //Options (dynamic)
    Menu_Options_Preferences = new QAction(QIcon(":/Image/Menu/Options_Prefs.png"), tr("Preferences..."), this);
    Menu_Options_Preferences->setStatusTip(tr(""));
    connect(Menu_Options_Preferences, SIGNAL(triggered()), this, SLOT(OnMenu_Options_Preferences()));

    Menu_Options_ResetFieldSizes = new QAction(tr("Reset field sizes"), this);
    Menu_Options_ResetFieldSizes->setStatusTip(tr(""));
    connect(Menu_Options_ResetFieldSizes, SIGNAL(triggered()), this, SLOT(OnMenu_Options_ResetFieldSizes()));
    Menu_Options_ResetFieldSizes->setVisible(false);

    Menu_Options = menuBar()->addMenu(tr("&Options"));
    Menu_Options->addAction(Menu_Options_Preferences);
    Menu_Fields_Main=Menu_Options->addMenu(tr("&Temporary preferences (for this instance only)"));
    Menu_Options->addSeparator();
    Menu_Options->addAction(Menu_Options_ResetFieldSizes);

    Preferences=new GUI_Preferences(this);
    Menu_Fields_Menus=new QMenu*[Preferences->Groups_Count_Get()];
    Menu_Fields_CheckBoxes=new QAction*[Preferences->Groups_Count_Get()*options::MaxCount];
    Menu_Fields_ActionGroups=new QActionGroup*[Preferences->Groups_Count_Get()];

    for (size_t Group=0; Group<Preferences->Groups_Count_Get(); Group++)
    {
        if (Preferences->Group_Options_Count_Get((group)Group, true))
        {
            Menu_Fields_ActionGroups[Group]=new QActionGroup(this);
            Menu_Fields_Menus[Group]=Menu_Fields_Main->addMenu(QString::fromUtf8(Preferences->Group_Name_Get((group)Group).c_str()));
        }
        for (size_t Option=0; Option<Preferences->Group_Options_Count_Get((group)Group, true); Option++)
        {
            switch (Preferences->Group_Option_Type_Get((group)Group, Option))
            {
            case Type_CheckBox:
                Menu_Fields_CheckBoxes[Group*options::MaxCount+Option] = new QAction(QString::fromUtf8(Preferences->Group_Option_Description_Get((group)Group, Option).c_str()), this);
                Menu_Fields_Menus[Group]->addAction(Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]);
                Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->setCheckable(true);
            break;
            case Type_RadioButton:
                Menu_Fields_RadioButtons[Group*options::MaxCount+Option] = new QAction(QString::fromUtf8(Preferences->Group_Option_Description_Get((group)Group, Option).c_str()), this);
                Menu_Fields_ActionGroups[Group]->addAction(Menu_Fields_RadioButtons[Group*options::MaxCount+Option]);
                Menu_Fields_Menus[Group]->addAction(Menu_Fields_RadioButtons[Group*options::MaxCount+Option]);
                Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->setCheckable(true);
            break;
            }
        }
    }
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_Tech3285_Req                    ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_Tech3285_Req(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_Tech3285_Rec                    ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_Tech3285_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec               ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_CodingHistory_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Comma      ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_CodingHistory_Rec_Ex_Comma(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Analog     ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_CodingHistory_Rec_Ex_Analog(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Frequency  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_CodingHistory_Rec_Ex_Frequency(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_WordLength ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_CodingHistory_Rec_Ex_WordLength(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_OriginatorReference_Rec         ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_OriginatorReference_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_INFO_Req                        ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_INFO_Req(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_INFO_Rec                        ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_INFO_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_FADGI_Rec                       ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_FADGI_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_EBU_ISRC_Rec                    ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Rules_EBU_ISRC_Rec(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_Riff2Rf64_Reject                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_riff2rf64_Reject(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_Overwrite_Reject                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_Overwrite_Reject(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_NoPadding_Accept                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_NoPadding_Accept(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_FileNotValid_Skip                 ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_FileNotValid_Skip(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_WrongExtension_Skip               ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_WrongExtension_Skip(bool)));
    connect(Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_NewChunksAtTheEnd                 ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_NewChunksAtTheEnd(bool)));
    connect(Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Generate                            ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_GenerateMD5(bool)));
    connect(Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Verify                              ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_VerifyMD5(bool)));
    connect(Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed                               ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EmbedMD5(bool)));
    connect(Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed_AuthorizeOverWritting         ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EmbedMD5_AuthorizeOverWritting(bool)));
    connect(Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_SwapEndian                          ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_SwapMD5Endianness(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_UTF8                    ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingUTF8(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP437                   ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingCP437(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP850                   ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingCP850(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP858                   ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingCP858(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP1252                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingCP1252(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_1                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_Encoding8859_1(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_2                  ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_Encoding8859_2(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_Local                   ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingLocal(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP437 ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallbackCP437(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP850 ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallbackCP850(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP858 ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallbackCP858(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP1252], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallbackCP1252(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_1], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallback8859_1(bool)));
    connect(Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_2], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_EncodingFallback8859_2(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Ignore_File_Encoding       ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_Ignore_File_Encoding(bool)));
    connect(Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Write_CodePage             ], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_Write_CodePage(bool)));
    for (size_t Group=Group_Tech; Group<=Group_Core; Group++)
        for (size_t Option=0; Option<Preferences->Group_Options_Count_Get((group)Group, true); Option++)
            connect(Menu_Fields_CheckBoxes[Group*options::MaxCount+Option], SIGNAL(toggled(bool)), this, SLOT(OnMenu_Options_TechCore(bool)));

    for (size_t Group=0; Group<Preferences->Groups_Count_Get(); Group++)
        for (size_t Option=0; Option<Preferences->Group_Options_Count_Get((group)Group, true); Option++)
            switch (Preferences->Group_Option_Type_Get((group)Group, Option))
            {
            case Type_CheckBox:
                Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->setChecked(Preferences->Group_Option_Checked_Get((group)Group, Option));
            break;
            case Type_RadioButton:
                Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->setChecked(Preferences->Group_Option_Checked_Get((group)Group, Option));
            break;
            }

    bool Value=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec]->isChecked();
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Comma]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Analog]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Frequency]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_WordLength]->setEnabled(Value);

    //Menu Help
    Menu_Help_Help = new QAction(QIcon(":/Image/Menu/Help.png"), tr("Help..."), this);
    Menu_Help_Help->setShortcut(tr(""));
    Menu_Help_Help->setStatusTip(tr(""));
    connect(Menu_Help_Help, SIGNAL(triggered()), this, SLOT(OnMenu_Help_Help()));

    Menu_Help_About = new QAction(QIcon(":/Image/Menu/About.png"), tr("About..."), this);
    Menu_Help_About->setShortcut(tr(""));
    Menu_Help_About->setStatusTip(tr(""));
    connect(Menu_Help_About, SIGNAL(triggered()), this, SLOT(OnMenu_Help_About()));

    Menu_Help_FADGI_Website = new QAction(QIcon(":/Image/FADGI/Logo.png"), tr("FADGI website"), this);
    Menu_Help_FADGI_Website->setShortcut(tr(""));
    Menu_Help_FADGI_Website->setStatusTip(tr(""));
    connect(Menu_Help_FADGI_Website, SIGNAL(triggered()), this, SLOT(OnMenu_Help_FADGI_Website()));

    Menu_Help = menuBar()->addMenu(tr("&Help"));
    Menu_Help->addAction(Menu_Help_Help);
    Menu_Help->addSeparator();
    Menu_Help->addAction(Menu_Help_About);
    Menu_Help->addAction(Menu_Help_FADGI_Website);
}

//***************************************************************************
// Menu actions
//***************************************************************************

class SleeperThread : public QThread
{
    public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Open_Files()
{
    //User interaction
    QStringList FileNamesQ = QFileDialog::getOpenFileNames(this,
                                                           "Select one or more files to open",
                                                           QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                           "Wave files (*.wav);;All files (*.*)");
    if (FileNamesQ.empty())
        return;

    ZenLib::ZtringList FileNames;
    for (int Pos=0; Pos<FileNamesQ.size(); Pos++)
        FileNames.push_back(ZenLib::Ztring().From_UTF8(FileNamesQ[Pos].toUtf8().data()));

    //Configuring
    C->StdOut("Opening files...");
    C->Menu_File_Open_Files_Begin();
    for (size_t Pos=0; Pos<FileNames.size(); Pos++)
    {
        #ifdef WIN32
            Ztring Temp=FileNames[Pos];
            Temp.FindAndReplace(__T("/"), __T("\\"), 0, Ztring_Recursive);
            C->Menu_File_Open_Files_Continue(Temp.To_UTF8());
        #else
            C->Menu_File_Open_Files_Continue(FileNames[Pos].To_UTF8());
        #endif
    }

    //Showing
    Open_Timer_Init(Timer_Open_Files);
    C->Menu_File_Open_Files_Finish_Start();
    C->Menu_File_Open_Files_Finish_Middle_Threaded();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Open_Directory()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getExistingDirectory( this,
                                                            tr("Open Directory"),
                                                            QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                            QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
    if (FileNamesQ.isEmpty())
        return;

    //Configuring
    C->StdOut("Opening directory...");
    C->Menu_File_Open_Files_Begin();
    #ifdef _WIN32
        Ztring Temp; Temp.From_UTF8(FileNamesQ.toUtf8().data()).FindAndReplace(__T("/"), __T("\\"), 0, Ztring_Recursive);
        C->Menu_File_Open_Files_Continue(Temp.To_UTF8());
    #else
        C->Menu_File_Open_Files_Continue(FileNamesQ.toUtf8().data());
    #endif

    //Showing
    Open_Timer_Init(Timer_Open_Directory);
    C->Menu_File_Open_Files_Finish_Start();
    C->Menu_File_Open_Files_Finish_Middle_Threaded();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Close_Files()
{
    ZtringList List=C->Menu_File_Close_File_FileName_Get();
    for (size_t Pos=0; Pos<List.size(); Pos++)
    {
        if (!Close(List[Pos].To_UTF8()))
            return;
    }
    C->Menu_File_Close_File_FileName_Clear();

    //GUI
    View_Refresh();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Close_All()
{
    if (!Close())
        return;

    //GUI
    View_Refresh();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Save_Files()
{
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Save_File(const QString& FileName)
{
    C->StdOut(string("Saving file ") + FileName.toStdString() + string("..."));

    Open_Timer_Init(Timer_Save);
    C->Menu_File_Save_File_Start(FileName.toStdString());
    C->Menu_File_Save_Middle_Threaded();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Save_All()
{
    //Running
    C->StdOut("Saving files...");

    //Showing
    Open_Timer_Init(Timer_Save);
    C->Menu_File_Save_Start();
    C->Menu_File_Save_Middle_Threaded();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_File_Undo()
{
    //User interaction
    GUI_Main_UndoDialog Dialog(C, this);
    if (!Dialog.exec())
        return;

    //Running
    C->StdOut("Undoing...");
    C->Menu_File_Undo_SelectBackupFile(Dialog.ListWidget->currentRow());
    C->StdOut("Undoing, finished");

    //Showing
    if (C->Text_stderr_Updated_Get())
    {
        Menu_View_Output_stderr->setChecked(true);
        View_Refresh(View_Output_stderr);
    }
    else
        View_Refresh();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Technical_Table(bool Checked)
{
    if (!Checked)
        return;

    //Showing
    View_Refresh(View_Technical_Table);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Technical_Text(bool Checked)
{
    if (!Checked)
        return;

    //Showing
    View_Refresh(View_Technical_Text);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Core_Table(bool Checked)
{
    if (!Checked)
        return;

    //Showing
    View_Refresh(View_Core_Table);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Core_Text(bool Checked)
{
    if (!Checked)
        return;

    //Showing
    View_Refresh(View_Core_Text);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_PerFile(bool Checked)
{
    if (!Checked)
        return;

    //Showing
    View_Refresh(View_PerFile);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Output_stdall()
{
    //Showing
    View_Refresh(View_Output_stdall);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Output_stdout()
{
    //Showing
    View_Refresh(View_Output_stdout);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Output_stderr()
{
    //Showing
    View_Refresh(View_Output_stderr);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_View_Output_Trace()
{
    //Showing
    View_Refresh(View_Output_Trace);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Import_Core_File()
{
    //User interaction
    QStringList FileNamesQ = QFileDialog::getOpenFileNames(  this,
                                                            tr("Open Core Document..."),
                                                            QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                            "CSV or XML files (*.csv *.xml);;CSV files (*.csv);;XML files (*.xml);;All files (*.*)");
    if (FileNamesQ.empty())
        return;

    ZenLib::ZtringList FileNames;
    for (int Pos=0; Pos<FileNamesQ.size(); Pos++)
        FileNames.push_back(ZenLib::Ztring().From_UTF8(FileNamesQ[Pos].toUtf8().data()));

    //Configuring
    C->StdOut("Importing Core file...");
    for (size_t Pos=0; Pos<FileNames.size(); Pos++)
        C->Menu_File_Import_Core(FileNames[Pos].To_UTF8());
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

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Import_Core_XML()
{
    C->In_Core_XML=Menu_Import_Core_XML->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Import_aXML_XML()
{
    C->In_aXML_XML=Menu_Import_aXML_XML->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Import_iXML_XML()
{
    C->In_iXML_XML=Menu_Import_iXML_XML->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Unified_XML_Global()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Save Metadata Document..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        "XML files (*.xml);;All files (*.*)");
    if (FileNamesQ.isEmpty())
    {
        //Configuring
        C->Out_XML_FileName.clear();

        //Display
        Menu_Export_Unified_XML_Global->setChecked(false);
        return;
    }

    //Configuring
    C->Out_XML_FileName=FileNamesQ.toUtf8().data();

    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;

    //Clearing
    C->Out_XML_FileName.clear();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Technical_CSV_Global()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Save Technical Document..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        "CSV files (*.csv);;All files (*.*)");
    if (FileNamesQ.isEmpty())
    {
        //Configuring
        C->Out_Tech_CSV_FileName.clear();
        
        //Display
        Menu_Export_Technical_CSV_Global->setChecked(false);
        return;
    }

    //Configuring
    C->Out_Tech_CSV_FileName=FileNamesQ.toUtf8().data();
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_Tech_CSV_FileName.clear();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Technical_XML_Global()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Save Technical Metadata..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        "XML files (*.xml);;All files (*.*)");
    if (FileNamesQ.isEmpty())
    {
        //Configuring
        C->Out_Tech_XML_FileName.clear();

        //Display
        Menu_Export_Technical_XML_Global->setChecked(false);
        return;
    }

    //Configuring
    C->Out_Tech_XML_FileName=FileNamesQ.toUtf8().data();

    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;

    //Clearing
    C->Out_Tech_XML_FileName.clear();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Technical_XML_PerFile()
{
    //Configuring
    C->Out_Tech_XML=true;

    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;

    //Clearing
    C->Out_Tech_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Core_CSV_Global()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Save Core Document..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        "CSV files (*.csv);;All files (*.*)");
    if (FileNamesQ.isEmpty())
    {
        //Configuring
        C->Out_Core_CSV_FileName.clear();
        
        //Display
        Menu_Export_Core_CSV_Global->setChecked(false);
        return;
    }

    //Configuring
    C->Out_Core_CSV_FileName=FileNamesQ.toUtf8().data();
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_Core_CSV_FileName.clear();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Core_XML_Global()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(  this,
                                                        tr("Save Core Document..."),
                                                        QString::fromUtf8(C->OpenSaveFolder.c_str()),
                                                        "XML files (*.xml);;All files (*.*)");
    if (FileNamesQ.isEmpty())
    {
        //Configuring
        C->Out_Core_XML_FileName.clear();
        
        //Display
        Menu_Export_Core_XML_Global->setChecked(false);
        return;
    }

    //Configuring
    C->Out_Core_XML_FileName=FileNamesQ.toUtf8().data();
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_Core_XML_FileName.clear();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_Core_XML_PerFile()
{
    //Configuring
    C->Out_Core_XML=true;
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_Core_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Import__PMX_XML()
{
    //Configuring
    C->In__PMX_XML=true;
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->In__PMX_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export__PMX_XML_PerFile()
{
    //Configuring
    C->Out__PMX_XML=true;
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out__PMX_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_aXML_XML_PerFile()
{
    //Configuring
    C->Out_aXML_XML=true;
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_aXML_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_iXML_XML_PerFile()
{
    //Configuring
    C->Out_iXML_XML=true;
    
    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;
    
    //Clearing
    C->Out_iXML_XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Export_cue__XML_PerFile()
{
    //Configuring
    C->Out_cue__XML=true;

    //Running
    C->Simulation_Enabled=true;
    C->Batch_Launch();
    C->Simulation_Enabled=false;

    //Clearing
    C->Out_cue__XML=false;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_Tech3285_Req(bool)
{
    C->Rules.Tech3285_Req=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_Tech3285_Req]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_Tech3285_Rec(bool)
{
    if (Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_Tech3285_Rec]->isChecked())
    {
        Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_Tech3285_Req]->setChecked(true);
        C->Rules.Tech3285_Req=true;
        C->Rules.Tech3285_Rec=true;
    }
    else
        C->Rules.Tech3285_Rec=false;

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_CodingHistory_Rec(bool)
{
    C->Rules.CodingHistory_Rec=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec]->isChecked();

    bool Value=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec]->isChecked();
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Comma]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Analog]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Frequency]->setEnabled(Value);
    Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_WordLength]->setEnabled(Value);

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_CodingHistory_Rec_Ex_Comma(bool)
{
    C->Rules.CodingHistory_Rec_Ex_Comma=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Comma]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_CodingHistory_Rec_Ex_Analog(bool)
{
    C->Rules.CodingHistory_Rec_Ex_Analog=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Analog]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_CodingHistory_Rec_Ex_Frequency(bool)
{
    C->Rules.CodingHistory_Rec_Ex_Frequency=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_Frequency]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_CodingHistory_Rec_Ex_WordLength(bool)
{
    C->Rules.CodingHistory_Rec_Ex_WordLength=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_CodingHistory_Rec_Ex_WordLength]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_OriginatorReference_Rec(bool)
{
    C->Rules.OriginatorReference_Rec=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_OriginatorReference_Rec]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_INFO_Req(bool)
{
    C->Rules.INFO_Req=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_INFO_Req]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_INFO_Rec(bool)
{
    if (Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_INFO_Rec]->isChecked())
    {
        Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_INFO_Req]->setChecked(true);
        C->Rules.INFO_Req=true;
        C->Rules.INFO_Rec=true;
    }
    else
        C->Rules.INFO_Rec=false;

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_FADGI_Rec(bool)
{
    C->Rules.FADGI_Rec=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_FADGI_Rec]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Rules_EBU_ISRC_Rec(bool)
{
    C->Rules.EBU_ISRC_Rec=Menu_Fields_CheckBoxes[Group_Rules*options::MaxCount+Option_Rules_EBU_ISRC_Rec]->isChecked();

    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingUTF8(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_UTF8]->isChecked() && C->Encoding!=Encoding_UTF8)
    {
        C->Encoding=Encoding_UTF8;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingCP437(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP437]->isChecked() && C->Encoding!=Encoding_CP437)
    {
        C->Encoding=Encoding_CP437;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingCP850(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP850]->isChecked() && C->Encoding!=Encoding_CP850)
    {
        C->Encoding=Encoding_CP850;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingCP858(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP858]->isChecked() && C->Encoding!=Encoding_CP858)
    {
        C->Encoding=Encoding_CP858;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingCP1252(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_CP1252]->isChecked() && C->Encoding!=Encoding_CP1252)
    {
        C->Encoding=Encoding_CP1252;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Encoding8859_1(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_1]->isChecked() && C->Encoding!=Encoding_8859_1)
    {
        C->Encoding=Encoding_8859_1;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Encoding8859_2(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_8859_2]->isChecked() && C->Encoding!=Encoding_8859_2)
    {
        C->Encoding=Encoding_8859_2;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingLocal(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding*options::MaxCount+Option_Encoding_Local]->isChecked() && C->Encoding!=Encoding_Local)
    {
        C->Encoding=Encoding_Local;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallbackCP437(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP437]->isChecked() && C->Fallback_Encoding!=Encoding_CP437)
    {
        C->Fallback_Encoding=Encoding_CP437;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallbackCP850(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP850]->isChecked() && C->Fallback_Encoding!=Encoding_CP850)
    {
        C->Fallback_Encoding=Encoding_CP850;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallbackCP858(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP858]->isChecked() && C->Fallback_Encoding!=Encoding_CP858)
    {
        C->Fallback_Encoding=Encoding_CP858;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallbackCP1252(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_CP1252]->isChecked() && C->Fallback_Encoding!=Encoding_CP1252)
    {
        C->Fallback_Encoding=Encoding_CP1252;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallback8859_1(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_1]->isChecked() && C->Fallback_Encoding!=Encoding_8859_1)
    {
        C->Fallback_Encoding=Encoding_8859_1;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EncodingFallback8859_2(bool)
{
    if (Menu_Fields_RadioButtons[Group_Encoding_Fallback*options::MaxCount+Option_Encoding_Fallback_8859_2]->isChecked() && C->Fallback_Encoding!=Encoding_8859_2)
    {
        C->Fallback_Encoding=Encoding_8859_2;
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Ignore_File_Encoding(bool)
{
    if (C->Ignore_File_Encoding!=Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Ignore_File_Encoding]->isChecked())
    {
        C->Ignore_File_Encoding=Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Ignore_File_Encoding]->isChecked();
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Write_CodePage(bool)
{
    if (C->Write_CodePage!=Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Write_CodePage]->isChecked())
    {
        C->Write_CodePage=Menu_Fields_CheckBoxes[Group_Encoding_Options*options::MaxCount+Option_Write_CodePage]->isChecked();
        C->Menu_File_Options_Update();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_TechCore(bool)
{
    if (View==NULL)
        return;
    QEvent Event((QEvent::Type)(QEvent::User+3));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_riff2rf64_Reject(bool)
{
    C->riff2rf64_Reject=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_Riff2Rf64_Reject]->isChecked();
    C->Menu_File_Options_Update();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Overwrite_Reject(bool)
{
    if (!C->Overwrite_Reject && C->Files_Modified_NotWritten_Count>0)
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("Reject if data must be overwritten");
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
                                        C->Overwrite_Reject=true;
                                        View_Refresh();
                                        break;
            case QMessageBox::Discard : // Don't Save was clicked
                                        C->Overwrite_Reject=true;
                                        C->Menu_File_Open_Files_BackToLastSave();
                                        View_Refresh();
                                        break;
            case QMessageBox::Cancel  : // Cancel was clicked
                                        C->Overwrite_Reject=false;
                                        Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_Overwrite_Reject]->setChecked(false);
                                        break;
            default:                    ; // Should never be reached
        } 
    }
    else
    {
        C->Overwrite_Reject=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_Overwrite_Reject]->isChecked();
        View_Refresh();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_NoPadding_Accept(bool)
{
    C->NoPadding_Accept=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_NoPadding_Accept]->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_FileNotValid_Skip(bool)
{
    C->FileNotValid_Skip=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_FileNotValid_Skip]->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_WrongExtension_Skip(bool)
{
    C->WrongExtension_Skip=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_WrongExtension_Skip]->isChecked();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_NewChunksAtTheEnd(bool)
{
    C->NewChunksAtTheEnd=Menu_Fields_CheckBoxes[Group_File*options::MaxCount+Option_File_NewChunksAtTheEnd]->isChecked();
    C->Menu_File_Options_Update();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_GenerateMD5(bool)
{
    if (!C->GenerateMD5 && C->Menu_File_Open_Files_Open_Get())
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("Generate MD5 for audio data");
        MessageBox.setText("Some files are already open.");
        MessageBox.setInformativeText("Do you want to generate MD5 for already open files?");
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            #if (QT_VERSION >= 0x040300)
                MessageBox.setDefaultButton(QMessageBox::No);
            #endif // (QT_VERSION >= 0x040300)
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        switch (MessageBox.exec())
        {
            case QMessageBox::Yes     : // Yes was clicked
                                        C->GenerateMD5=true;
                                        C->Menu_File_Options_Update();
                                        Open_Timer_Init(Timer_Open_Files);
                                        C->Menu_File_Open_Files_Finish_Start();
                                        C->Menu_File_Open_Files_Finish_Middle_Threaded();
                                        break;
            case QMessageBox::No      : // No was clicked
                                        C->GenerateMD5=true;
                                        break;
            case QMessageBox::Cancel  : // Cancel was clicked
                                        C->GenerateMD5=false;
                                        Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Generate]->setChecked(false);
                                        break;
            default:                    ; // Should never be reached
        } 
    }
    else
    {
        C->GenerateMD5=Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Generate]->isChecked();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_VerifyMD5(bool)
{
    C->VerifyMD5=Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Verify]->isChecked();
    if (C->VerifyMD5==true)
    {
        C->Menu_File_Options_Update();

        //Showing
        if (C->Text_stderr_Updated_Get())
        {
            Menu_View_Output_stderr->setChecked(true);
            View_Refresh(View_Output_stderr);
        }
        else
            View_Refresh();
    }
    else
        C->Menu_File_Options_Update();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EmbedMD5(bool)
{
    C->EmbedMD5=Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed]->isChecked();
    if (C->EmbedMD5==true)
    {
        C->Menu_File_Options_Update();
        View_Refresh();
    }
    else
    {
        if (Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed_AuthorizeOverWritting]->isChecked())
        {
            QMessageBox MessageBox;
            MessageBox.setWindowTitle("Embed MD5 for audio data");
            MessageBox.setText("Know what you are doing.");
            MessageBox.setInformativeText("Manual modification of this value is NOT recommended and will cause the file to fail audio MD5 verification if the value differs from the generated value. In other words, it's not a good idea to select this option unless you have good reason to do so. Do you want to disable MD5 overwriting?");
            #if (QT_VERSION >= 0x040200)
                MessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                #if (QT_VERSION >= 0x040300)
                    MessageBox.setDefaultButton(QMessageBox::Yes);
                #endif // (QT_VERSION >= 0x040300)
            #endif // (QT_VERSION >= 0x040200)
            MessageBox.setIcon(QMessageBox::Warning);
            MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Yes     : // Yes was clicked
                                            break;
                case QMessageBox::No      : // No was clicked
                                            return;
                default:                    ; // Should never be reached
            } 
        }

        Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed_AuthorizeOverWritting]->setChecked(false);
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_EmbedMD5_AuthorizeOverWritting(bool)
{
    C->EmbedMD5_AuthorizeOverWritting=Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed_AuthorizeOverWritting]->isChecked();
    if (C->EmbedMD5_AuthorizeOverWritting==true)
    {
        if (!Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed]->isChecked())
        {
            QMessageBox MessageBox;
            MessageBox.setWindowTitle("Embed MD5 for audio data");
            MessageBox.setText("Know what you are doing.");
            MessageBox.setInformativeText("Manual modification of this value is NOT recommended and will cause the file to fail audio MD5 verification if the value differs from the generated value. In other words, it's not a good idea to select this option unless you have good reason to do so. Do you want to enable embed MD5 for audio data?");
            #if (QT_VERSION >= 0x040200)
                MessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                #if (QT_VERSION >= 0x040300)
                    MessageBox.setDefaultButton(QMessageBox::Yes);
                #endif // (QT_VERSION >= 0x040300)
            #endif // (QT_VERSION >= 0x040200)
            MessageBox.setIcon(QMessageBox::Warning);
            MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
            switch (MessageBox.exec())
            {
                case QMessageBox::Yes     : // Yes was clicked
                                            break;
                case QMessageBox::No      : // No was clicked
                                            return;
                default:                    ; // Should never be reached
            } 
        }

        Menu_Fields_CheckBoxes[Group_MD5*options::MaxCount+Option_MD5_Embed]->setChecked(true);
    }
    else
    {
        C->Menu_File_Options_Update();
        C->Menu_File_Open_Files_Finish(); //TODO: progress bar
        View_Refresh();
    }
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_SwapMD5Endianness(bool)
{
    QEvent Event((QEvent::Type)(QEvent::User+2));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_ResetFieldSizes()
{
    QEvent Event((QEvent::Type)(QEvent::User+1));
    QApplication::sendEvent(View, &Event);
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Options_Preferences()
{
    if (Preferences->isVisible())
        return;

    //Checking if there are changes
    bool HasChanged=false;
    for (size_t Group=0; Group<Preferences->Groups_Count_Get(); Group++)
        for (size_t Option=0; Option<Preferences->Group_Options_Count_Get((group)Group, true); Option++)
            switch (Preferences->Group_Option_Type_Get((group)Group, Option))
            {
            case Type_CheckBox:
                if (Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->isChecked()!=Preferences->Group_Option_Checked_Get((group)Group, Option))
                    HasChanged=true;
            break;
            case Type_RadioButton:
                if (Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->isChecked()!=Preferences->Group_Option_Checked_Get((group)Group, Option))
                    HasChanged=true;
            break;
            }

    if (HasChanged)
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("Preferences have changed");
        MessageBox.setText("Temporary preferences are not same as saved preferences.");
        MessageBox.setInformativeText("Do you want to set your temporary preferences as saved preferences?");
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            #if (QT_VERSION >= 0x040300)
                MessageBox.setDefaultButton(QMessageBox::No);
            #endif // (QT_VERSION >= 0x040300)
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        switch (MessageBox.exec())
        {
            case QMessageBox::Yes     : // Yes was clicked
                                        for (size_t Group=0; Group<Preferences->Groups_Count_Get(); Group++)
                                            for (size_t Option=0; Option<Preferences->Group_Options_Count_Get((group)Group, true); Option++)
                                                switch (Preferences->Group_Option_Type_Get((group)Group, Option))
                                                {
                                                case Type_CheckBox:
                                                    Preferences->Group_Option_Checked_Set((group)Group, Option, Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->isChecked());
                                                break;
                                                case Type_RadioButton:
                                                    Preferences->Group_Option_Checked_Set((group)Group, Option, Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->isChecked());
                                                break;
                                                }
                                        break;
            case QMessageBox::No      : // Yes was clicked
                                        break;
            case QMessageBox::Cancel  : // Cancel was clicked
                                        return;
            default:                    ; // Should never be reached
        } 
    }

    //Showing
    Preferences->show();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Help_Help()
{
    //Showing
    GUI_Help* Frame=new GUI_Help(this);
    Frame->show();
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Help_About()
{
    //Showing
    GUI_About *About = new GUI_About(this);
    About->exec();
    delete About;
}

//---------------------------------------------------------------------------
void GUI_Main::OnMenu_Help_FADGI_Website()
{
    //Showing
    QDesktopServices::openUrl(QUrl("http://www.digitizationguidelines.gov", QUrl::TolerantMode));
}

//***************************************************************************
// ToolBar creation
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main::ToolBar_Create()
{
    ToolBar=new QToolBar(tr("ToolBar"));
    ToolBar->setIconSize(QSize(32, 32));
        
    ToolBar->addAction(Menu_File_Open_Files);
    ToolBar->addAction(Menu_File_Open_Directory);
    ToolBar->addSeparator();
    ToolBar->addAction(Menu_File_Save_All);
    ToolBar->addSeparator();
    ToolBar->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_PerFile]);
    ToolBar->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Tech_Table]);
    ToolBar->addAction(Menu_Fields_RadioButtons[Group_DefaultView*options::MaxCount+Option_DefaultView_Core_Table]);
    ToolBar->addSeparator();
    ToolBar->addAction(Menu_Options_Preferences);
    ToolBar->addSeparator();
    ToolBar->addAction(Menu_Help_Help);
    ToolBar->addAction(Menu_Help_About);
    ToolBar->addAction(Menu_Help_FADGI_Website);

    #ifndef __APPLE__
        addToolBar(Qt::LeftToolBarArea, ToolBar);
    #else //__APPLE__
        addToolBar(ToolBar);
    #endif //__APPLE__
}
