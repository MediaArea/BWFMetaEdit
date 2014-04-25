// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Preferences.h"
#include "Common/Core.h"
#include <QTextBrowser>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QDesktopServices>
#include <QScrollArea>
#include "ZenLib/File.h"
#include "ZenLib/Dir.h"
#include "ZenLib/ZtringListListF.h"
#ifdef _WIN32
    #undef _TEXT
    #undef __TEXT
    #include "shlobj.h"
#endif //_WIN32
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

options Groups[Group_Max]=
{
    {
        "Technical Metadata",
        18,
        {
            {"Tech_FileSize", "FileSize", Type_CheckBox, true},
            {"Tech_Format", "Format", Type_CheckBox, true},
            {"Tech_CodecID", "CodecID", Type_CheckBox, true},
            {"Tech_Channels", "Channels", Type_CheckBox, true},
            {"Tech_SampleRate", "SampleRate", Type_CheckBox, true},
            {"Tech_BitRate", "BitRate", Type_CheckBox, true},
            {"Tech_BitPerSample", "BitPerSample", Type_CheckBox, true},
            {"Tech_Duration", "Duration", Type_CheckBox, true},
            {"Tech_UnsupportedChunks", "UnsupportedChunks", Type_CheckBox, true},
            {"Tech_bext", "bext", Type_CheckBox, true},
            {"Tech_INFO", "INFO", Type_CheckBox, true},
            {"Tech_XMP", "XMP", Type_CheckBox, true},
            {"Tech_aXML", "aXML", Type_CheckBox, true},
            {"Tech_iXML", "iXML", Type_CheckBox, true},
            {"Tech_MD5Stored", "MD5Stored", Type_CheckBox, true},
            {"Tech_MD5Generated", "MD5Generated", Type_CheckBox, true},
            {"Tech_Errors", "Errors", Type_CheckBox, true},
            {"Tech_Information", "Information", Type_CheckBox, true},
        },
        true,
    },
    {
        "Core Metadata",
        15+17,
        {
            {"Core_Description", "Description", Type_CheckBox, true},
            {"Core_Originator", "Originator", Type_CheckBox, true},
            {"Core_OriginatorReference", "OriginatorReference", Type_CheckBox, true},
            {"Core_OriginationDate", "OriginationDate", Type_CheckBox, true},
            {"Core_OriginationTime", "OriginationTime", Type_CheckBox, true},
            {"Core_TimeReference_Translated", "TimeReference (translated)", Type_CheckBox, true},
            {"Core_TimeReference", "TimeReference", Type_CheckBox, true},
            {"Core_BextVersion", "BextVersion", Type_CheckBox, true},
            {"Core_UMID", "UMID", Type_CheckBox, true},
            {"Core_LoudnessValue", "LoudnessValue", Type_CheckBox, true},
            {"Core_LoudnessRange", "LoudnessRange", Type_CheckBox, true},
            {"Core_MaxTruePeakLevel", "MaxTruePeakLevel", Type_CheckBox, true},
            {"Core_MaxMomentaryLoudness", "MaxMomentaryLoudness", Type_CheckBox, true},
            {"Core_MaxShortTermLoudness", "MaxShortTermLoudness", Type_CheckBox, true},
            {"Core_CodingHistory", "CodingHistory", Type_CheckBox, true},
            {"Core_IARL", "IARL", Type_CheckBox, true}, //Archival Location
            {"Core_IART", "IART", Type_CheckBox, true}, //Artist
            {"Core_ICMS", "ICMS", Type_CheckBox, true}, //Commissioned
            {"Core_ICMT", "ICMT", Type_CheckBox, true}, //Comment
            {"Core_ICOP", "ICOP", Type_CheckBox, true}, //Copyright
            {"Core_ICRD", "ICRD", Type_CheckBox, true}, //Date Created
            {"Core_IENG", "IENG", Type_CheckBox, true}, //Engineer
            {"Core_IGNR", "IGNR", Type_CheckBox, true}, //Genre
            {"Core_IKEY", "IKEY", Type_CheckBox, true}, //Keywords
            {"Core_IMED", "IMED", Type_CheckBox, true}, //Medium
            {"Core_INAM", "INAM", Type_CheckBox, true}, //Title
            {"Core_IPRD", "IPRD", Type_CheckBox, true}, //Product
            {"Core_ISBJ", "ISBJ", Type_CheckBox, true}, //Subject
            {"Core_ISFT", "ISFT", Type_CheckBox, true}, //Software
            {"Core_ISRC", "ISRC", Type_CheckBox, true}, //Source
            {"Core_ISRF", "ISRF", Type_CheckBox, true}, //Source Form
            {"Core_ITCH", "ITCH", Type_CheckBox, true}, //Technician
        },
        true,
    },
    {
        "Rules",
        Option_Rules_Max,
        {
            {"Rules_Tech3285_Req", "BWF (EBU Tech 3285) requirements", Type_CheckBox, true},
            {"Rules_Tech3285_Rec", "BWF (EBU Tech 3285) recommendations", Type_CheckBox, false},
            {"Rules_CodingHistory_Rec", "BWF CodingHistory (EBU Tech R98-1999) recommendations", Type_CheckBox, false},
            {"Rules_OriginatorReference_Rec", "BWF OriginatorReference (EBU R99-1999) recommendations", Type_CheckBox, false},
            {"Rules_INFO_Req", "INFO (Microsoft definition) requirements", Type_CheckBox, true},
            {"Rules_INFO_Rec", "INFO (Microsoft definition) recommendations", Type_CheckBox, false},
            {"Rules_FADGI_Rec", "Federal Agencies Digitization Guidelines Initative recommendations", Type_CheckBox, false},
        },
        true,
    },
    {
        "File management",
        Option_File_Max,
        {
            {"File_Riff2Rf64_Reject", "Reject file if transformation to RF64 is requested", Type_CheckBox, false},
            {"File_Overwrite_Reject", "Prevent overwrite of existing data", Type_CheckBox, false},
            {"File_NoPadding_Accept", "Accept file if padding byte is missing", Type_CheckBox, false},
            {"File_FileNotValid_Skip", "Skip non-valid files", Type_CheckBox, false},
            {"File_WrongExtension_Skip", "Skip files with no .wav extension", Type_CheckBox, true},
            {"File_NewChunksAtTheEnd", "Place new or expanded BEXT and LIST-INFO chunks at the end of the file", Type_CheckBox, true},
        },
        true,
    },
    {
        "MD5",
        Option_MD5_Max,
        {
            {"MD5_Generate", "Generate MD5 for audio data", Type_CheckBox, false},
            {"MD5_Verify", "Verify MD5 for audio data", Type_CheckBox, false},
            {"MD5_Embed", "Embed MD5 for audio data", Type_CheckBox, false},
            {"MD5_Embed_AuthorizeOverWritting", "Allow overwriting of embedded MD5 value", Type_CheckBox, false},
        },
        true,
    },
    {
        "Default view",
        Option_DefaultView_Max,
        {
            {"DefaultView_Tech_Table", "Technical Metadata (table)", Type_RadioButton, true},
            {"DefaultView_Tech_Text", "Technical Metadata (text)", Type_RadioButton, false},
            {"DefaultView_Core_Table", "Core Metadata (table)", Type_RadioButton, false},
            {"DefaultView_Core_Text", "Core Metadata (text)", Type_RadioButton, false},
        },
        false,
    },
};
//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
GUI_Preferences::GUI_Preferences(GUI_Main* parent)
: QDialog(parent)
{
    Main=parent;
    
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("BWF MetaEdit preferences");
    Dialog=new QDialogButtonBox(QDialogButtonBox::RestoreDefaults | QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnSave()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(OnRejected()));
    connect(Dialog, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnDialogClicked(QAbstractButton*)));

    QVBoxLayout* L=new QVBoxLayout();
    Central=new QTabWidget(this);

    Create();

    L->addWidget(Central);
    L->addWidget(Dialog);

    setLayout(L);

    OnLoad();

    Central->setCurrentIndex(Group_Rules);

    resize(width(), QApplication::desktop()->screenGeometry().height()*3/4);
}

//---------------------------------------------------------------------------
void GUI_Preferences::showEvent(QShowEvent* Event)
{
    move((QApplication::desktop()->screenGeometry().width()-780)/2, 80);
    resize(780, height());

    OnClicked();
}

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
size_t GUI_Preferences::Groups_Count_Get()
{
    return Group_Max;
}

//---------------------------------------------------------------------------
string GUI_Preferences::Group_Name_Get(group Group)
{
    return Groups[Group].Name;
}

//---------------------------------------------------------------------------
size_t GUI_Preferences::Group_Options_Count_Get(group Group, bool ForTemporaryPrefs)
{
    if (ForTemporaryPrefs && !Groups[Group].InTemporaryPrefs)
        return 0;
        
    return Groups[Group].Option_Size;
}

//---------------------------------------------------------------------------
string GUI_Preferences::Group_Option_Description_Get(group Group, size_t Option)
{
    return Groups[Group].Option[Option].Description;
}

//---------------------------------------------------------------------------
bool GUI_Preferences::Group_Option_Checked_Get(group Group, size_t Option)
{
    switch (Groups[Group].Option[Option].Type)
    {
        case Type_CheckBox      : return CheckBoxes  [Group*options::MaxCount+Option]->isChecked();
        case Type_RadioButton   : return RadioButtons[Group*options::MaxCount+Option]->isChecked();
        default                 : return false;
    }
}

//---------------------------------------------------------------------------
bool GUI_Preferences::Group_Option_Checked_Set(group Group, size_t Option, bool Value)
{
    switch (Groups[Group].Option[Option].Type)
    {
        case Type_CheckBox      : CheckBoxes  [Group*options::MaxCount+Option]->setChecked(Value);
        case Type_RadioButton   : break; //RadioButtons[Group*options::MaxCount+Option]->isChecked();
        default                 : ;
    }

    return true;
}

//***************************************************************************
// Load/Save
//***************************************************************************

//---------------------------------------------------------------------------
std::string GUI_Preferences::ConfigDirectory_Get()
{
    //Getting application folder
    Ztring ApplicationFolder;
    #ifdef _WIN32
        CHAR Path[MAX_PATH];
        BOOL Result=SHGetSpecialFolderPath(NULL, Path, CSIDL_APPDATA, true);
        ApplicationFolder=Ztring(Path)+"\\bwfmetaedit";
    #else //_WIN32
        ApplicationFolder=Ztring().From_Local(std::getenv("HOME"))+"/.bwfmetaedit";
    #endif //_WIN32
    
    //Creating the directory
    if (!Dir::Exists(ApplicationFolder))
        Dir::Create(ApplicationFolder);
    
    //Testing
    if (!Dir::Exists(ApplicationFolder))
        return string();

    return ApplicationFolder;
}

//---------------------------------------------------------------------------
std::string GUI_Preferences::ConfigFileName_Get()
{
    //Getting application folder
    Ztring ApplicationFolder=ConfigDirectory_Get();
    if (ApplicationFolder.empty())
        return string();

    return ApplicationFolder+"/bwfmetaedit.cfg";
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnLoad()
{
    //Getting application folder
    string ConfigFileName=ConfigFileName_Get();
    if (ConfigFileName.empty())
        return; //There is a problem

    ZtringListListF Config;
    Config.Load(ConfigFileName);
    if (Config.empty())
    {
        //No config file, creating a default one
        LoadOriginalConfig();
        
        OnSave();
        Config.Load();
        if (Config.empty())
            return; //There is a problem
    }

    //Legacy
    if (Config("MD5Generated").empty())
    {
        Config("MD5Generated")=Config("MD5Generated");
        Config.erase(Config.begin()+Config.Find("MD5Generated"));
        Config.Save();
    }

    //Setting
    for (size_t Group=0; Group<Group_Max; Group++)
        for (size_t Option=0; Option<Groups[Group].Option_Size; Option++)
        {
            if (Config(Groups[Group].Option[Option].UniqueName).empty())
                Config(Groups[Group].Option[Option].UniqueName).From_Number(Groups[Group].Option[Option].DefaultConfigValue);    
            switch (Groups[Group].Option[Option].Type)
            {
                case Type_CheckBox     : CheckBoxes  [Group*options::MaxCount+Option]->setChecked((bool)Config(Groups[Group].Option[Option].UniqueName).To_int64u()); connect(CheckBoxes  [Group*options::MaxCount+Option], SIGNAL(clicked()), this, SLOT(OnClicked())); break;
                case Type_RadioButton  : RadioButtons[Group*options::MaxCount+Option]->setChecked((bool)Config(Groups[Group].Option[Option].UniqueName).To_int64u()); connect(RadioButtons[Group*options::MaxCount+Option], SIGNAL(clicked()), this, SLOT(OnClicked())); break;
                default                 : ;
            }
        }

    //Extra
    if (Config("Extra_BackupDirectory").empty())
    {
        Extra_BackupDirectory_Default->setChecked(true);
        Extra_BackupDirectory_Specific->setText(ConfigDirectory_Get().c_str());
        Extra_BackupDirectory_Specific->setEnabled(false);
    }
    else
    {
        Extra_BackupDirectory_Specific_Radio->setChecked(true);
        Extra_BackupDirectory_Specific->setText(Config("Extra_BackupDirectory").c_str());
        Extra_BackupDirectory_Specific->setEnabled(true);
    }
    Main->BackupDirectory_Set(Extra_BackupDirectory_Specific->text().toLocal8Bit().data());
    if (Config("Extra_LogFile").empty())
    {
        Extra_LogFile_Deactivated->setChecked(true);
        Extra_LogFile_Activated->setText(QString());
        Extra_LogFile_Activated->setEnabled(false);
    }
    else
    {
        Extra_LogFile_Activated_Radio->setChecked(true);
        Extra_LogFile_Activated->setText(Config("Extra_LogFile").c_str());
        Extra_LogFile_Activated->setEnabled(true);
    }
    Main->LogFile_Set(Extra_LogFile_Activated->text().toLocal8Bit().data());
    if (Config("Extra_Bext_DefaultVersion").empty())
    {
        Extra_Bext_DefaultVersion->setValue(0);
    }
    else
    {
        int64u DefaultVersion=Config("Extra_Bext_DefaultVersion").To_int64u();
        if (DefaultVersion<=0xFFFF)
            Extra_Bext_DefaultVersion->setValue((int8u)DefaultVersion);
        else
            Extra_Bext_DefaultVersion->setValue(0);
    }
    Main->Bext_DefaultVersion_Set(Extra_Bext_DefaultVersion->value());
    if (Config("Extra_Bext_MaxVersion").empty())
    {
        Extra_Bext_MaxVersion->setValue(2);
    }
    else
    {
        int64u MaxVersion=Config("Extra_Bext_MaxVersion").To_int64u();
        if (MaxVersion<=0xFFFF)
            Extra_Bext_MaxVersion->setValue((int16u)MaxVersion);
        else
            Extra_Bext_MaxVersion->setValue(2);
    }
    Main->Bext_MaxVersion_Set(Extra_Bext_MaxVersion->value());
    Extra_Bext_Toggle->setChecked(Config("Extra_Bext_Toggle").To_int64u()?true:false);
    Main->Bext_Toggle_Set(Extra_Bext_Toggle->isChecked());

    close();
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnSave()
{
    //Getting application folder
    string ConfigFileName=ConfigFileName_Get();
    if (ConfigFileName.empty())
    {
        close();
        return; //There is a problem
    }

    File Prefs;
    Prefs.Create(ConfigFileName);
    for (size_t Group=0; Group<Group_Max; Group++)
    {
        for (size_t Option=0; Option<Groups[Group].Option_Size; Option++)
        {
            Ztring Content;
            Content+=Groups[Group].Option[Option].UniqueName;
            Content+=" = ";
            switch (Groups[Group].Option[Option].Type)
            {
                case Type_CheckBox     : Content+=CheckBoxes  [Group*options::MaxCount+Option]->isChecked()?'1':'0'; break;
                case Type_RadioButton  : Content+=RadioButtons[Group*options::MaxCount+Option]->isChecked()?'1':'0'; break;
                default                 : ;
            }
            
            Content+=EOL;
            Prefs.Write(Content);
        }
    }

    //Extra
    {
        Ztring Content;
        Content+="Extra_BackupDirectory";
        Content+=" = ";
        if (Extra_BackupDirectory_Specific_Radio->isChecked())
            Content+=Ztring().From_Local(Extra_BackupDirectory_Specific->text().toLocal8Bit().data());
        Content+=EOL;
        Prefs.Write(Content);
        Main->BackupDirectory_Set(Extra_BackupDirectory_Specific->text().toLocal8Bit().data());
    }
    {
        Ztring Content;
        Content+="Extra_LogFile";
        Content+=" = ";
        if (Extra_LogFile_Activated_Radio->isChecked())
            Content+=Ztring().From_Local(Extra_LogFile_Activated->text().toLocal8Bit().data());
        Content+=EOL;
        Prefs.Write(Content);
        Main->LogFile_Set(Extra_LogFile_Activated->text().toLocal8Bit().data());
    }
    {
        Ztring Content;
        Content+="Extra_Bext_DefaultVersion";
        Content+=" = ";
        Ztring Data=Ztring().From_Number(Extra_Bext_DefaultVersion->value());
        Content+=Data;
        Content+=EOL;
        Prefs.Write(Content);
        Main->Bext_DefaultVersion_Set(Data.To_int8u());
    }
    {
        Ztring Content;
        Content+="Extra_Bext_MaxVersion";
        Content+=" = ";
        Ztring Data=Ztring().From_Number(Extra_Bext_MaxVersion->value());
        Content+=Data;
        Content+=EOL;
        Prefs.Write(Content);
        Main->Bext_MaxVersion_Set(Data.To_int8u());
    }
    {
        Ztring Content;
        Content+="Extra_Bext_Toggle";
        Content+=" = ";
        Ztring Data=Ztring().From_Number(Extra_Bext_Toggle->isChecked());
        Content+=Data;
        Content+=EOL;
        Prefs.Write(Content);
        Main->Bext_Toggle_Set(Data.To_int8u()?true:false);
    }
        
    //Menu
    Main->Menu_Update();

    close();
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnRejected()
{
    //Loading the original config file 
    OnLoad();
    
    //Configuring the main view
    OnClicked();
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnDialogClicked(QAbstractButton* Button)
{
    if (Dialog->buttonRole(Button)==QDialogButtonBox::ResetRole)
    {
        LoadOriginalConfig();
        OnClicked();
    }
}

//***************************************************************************
// Menu
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Preferences::OnClicked ()
{
    //Searching what has changed
    for (size_t Group=0; Group<Group_Max; Group++)
        for (size_t Option=0; Option<Groups[Group].Option_Size; Option++)
            switch (Groups[Group].Option[Option].Type)
            {
                case Type_CheckBox      :   if (Groups[Group].InTemporaryPrefs && CheckBoxes  [Group*options::MaxCount+Option]->isChecked()!=Main->Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->isChecked())
                                            {
                                                //Setting Main view preferences
                                                Main->Menu_Fields_CheckBoxes[Group*options::MaxCount+Option]->setChecked(CheckBoxes[Group*options::MaxCount+Option]->isChecked());

                                                //Loading new config
                                                for (size_t Option2=0; Option2<Groups[Group].Option_Size; Option2++)
                                                    CheckBoxes[Group*options::MaxCount+Option2]->setChecked(Main->Menu_Fields_CheckBoxes[Group*options::MaxCount+Option2]->isChecked());
                                            }
                                            break;
                case Type_RadioButton   :   if (RadioButtons[Group*options::MaxCount+Option]->isChecked()!=Main->Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->isChecked())
                                            {
                                                //Setting Main view preferences
                                                Main->Menu_Fields_RadioButtons[Group*options::MaxCount+Option]->setChecked(RadioButtons[Group*options::MaxCount+Option]->isChecked());
                                            }
                                            break;
                default                 : ;
            }
    
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnExtra_BackupDirectory_Specific_RadioToggled (bool Checked)
{
    if (Checked)
        Extra_BackupDirectory_Specific->setEnabled(true);
    else
    {
        Extra_BackupDirectory_Specific->setText(ConfigDirectory_Get().c_str());
        Extra_BackupDirectory_Specific->setEnabled(false);
    }
}

//---------------------------------------------------------------------------
void GUI_Preferences::OnExtra_LogFile_Activated_RadioToggled (bool Checked)
{
    if (Checked)
    {
        Extra_LogFile_Activated->setText((ConfigDirectory_Get()+PathSeparator+"LogFile.txt").c_str());
        Extra_LogFile_Activated->setEnabled(true);
    }
    else
    {
        Extra_LogFile_Activated->setText(QString());
        Extra_LogFile_Activated->setEnabled(false);
    }
}

//---------------------------------------------------------------------------
void GUI_Preferences::Create()
{
    CheckBoxes=new QCheckBox*[Group_Max*options::MaxCount];
    RadioButtons=new QRadioButton*[Group_Max*options::MaxCount];
    
    for (size_t Kind=0; Kind<Group_Max; Kind++)
    {
        QVBoxLayout* Columns=new QVBoxLayout();
        
        switch (Kind)
        {
            case Group_Tech     : Columns->addWidget(new QLabel("Select which technical values should appear on the 'Tech' table view of BWF MetaEdit, others will be hidden.")); break;
            case Group_Core     : Columns->addWidget(new QLabel("Select which technical values should appear on the 'Core' table view of BWF MetaEdit, others will be hidden.\nThese options affect only the displayed table and not the handling of imported or exported Core documents.\nBe aware that even if a column is hidden, metadata can be imported, exported and saved within these fields.")); break;
            case Group_Rules    : Columns->addWidget(new QLabel("Select which standards and rule sets to follow during use of BWF MetaEdit.\nSelection of rule sets will constrained the allowed data entry and may add additional metadata requirements.\nSee documentation on BWF MetaEdit Rules within the Help documentation.")); break;
        }
        
        for (size_t Option=0; Option<Groups[Kind].Option_Size; Option++)
        {
            switch (Groups[Kind].Option[Option].Type)
            {
                case Type_CheckBox              : CheckBoxes[Kind*options::MaxCount+Option]=new QCheckBox(Groups[Kind].Option[Option].Description); Columns->addWidget(CheckBoxes[Kind*options::MaxCount+Option]); break;
                case Type_RadioButton           : RadioButtons[Kind*options::MaxCount+Option]=new QRadioButton(Groups[Kind].Option[Option].Description); Columns->addWidget(RadioButtons[Kind*options::MaxCount+Option]); break;
                default                         : ;
            }
        }
        QWidget* Columns_Widget=new QWidget();
        Columns->addStretch();
        Columns_Widget->setLayout(Columns);
        QScrollArea* ScrollArea=new QScrollArea();
        ScrollArea->setWidget(Columns_Widget);
        Central->addTab(ScrollArea, tr(Groups[Kind].Name));
    }

    //Extra - BackupDirectory
    Extra_BackupDirectory_Default=new QRadioButton("Default backup directory");
    Extra_BackupDirectory_Specific_Radio=new QRadioButton("Specific directory: ");
    connect(Extra_BackupDirectory_Specific_Radio, SIGNAL(toggled(bool)), this, SLOT(OnExtra_BackupDirectory_Specific_RadioToggled(bool)));
    Extra_BackupDirectory_Specific=new QLineEdit();

    QGridLayout* Extra_BackupDirectory_Layout=new QGridLayout();
    Extra_BackupDirectory_Layout->addWidget(Extra_BackupDirectory_Default, 0, 0);
    Extra_BackupDirectory_Layout->addWidget(Extra_BackupDirectory_Specific_Radio, 1, 0);
    Extra_BackupDirectory_Layout->addWidget(Extra_BackupDirectory_Specific, 1, 1);

    QGroupBox* Extra_BackupDirectory=new QGroupBox("Backup directory");
    Extra_BackupDirectory->setLayout(Extra_BackupDirectory_Layout);

    //Extra - LogFile
    Extra_LogFile_Deactivated=new QRadioButton("No log file");
    Extra_LogFile_Activated_Radio=new QRadioButton("Log file is save into: ");
    connect(Extra_LogFile_Activated_Radio, SIGNAL(toggled(bool)), this, SLOT(OnExtra_LogFile_Activated_RadioToggled(bool)));
    Extra_LogFile_Activated=new QLineEdit();

    QGridLayout* Extra_LogFile_Layout=new QGridLayout();
    Extra_LogFile_Layout->addWidget(Extra_LogFile_Deactivated, 0, 0);
    Extra_LogFile_Layout->addWidget(Extra_LogFile_Activated_Radio, 1, 0);
    Extra_LogFile_Layout->addWidget(Extra_LogFile_Activated, 1, 1);

    QGroupBox* Extra_LogFile=new QGroupBox("Log file");
    Extra_LogFile->setLayout(Extra_LogFile_Layout);

    //Extra - Bext
    Extra_Bext_DefaultVersion=new QDoubleSpinBox();
    Extra_Bext_DefaultVersion->setMinimum(0);
    Extra_Bext_DefaultVersion->setMaximum(0xFFFF);
    Extra_Bext_DefaultVersion->setDecimals(0);
    QLabel* Extra_Bext_DefaultVersion_Label=new QLabel("Default bext version:");

    Extra_Bext_MaxVersion=new QDoubleSpinBox();
    Extra_Bext_MaxVersion->setMinimum(2);
    Extra_Bext_MaxVersion->setMaximum(0xFFFF);
    Extra_Bext_MaxVersion->setDecimals(0);
    QLabel* Extra_Bext_MaxVersion_Label=new QLabel("Max bext version:");

    Extra_Bext_Toggle=new QCheckBox();
    Extra_Bext_Toggle->setText("Use a toggle instead of a question window");

    QGridLayout* Extra_Bext_Layout=new QGridLayout();
    Extra_Bext_Layout->addWidget(Extra_Bext_DefaultVersion_Label, 0, 0);
    Extra_Bext_Layout->addWidget(Extra_Bext_DefaultVersion, 0, 1);
    Extra_Bext_Layout->addWidget(Extra_Bext_MaxVersion_Label, 1, 0);
    Extra_Bext_Layout->addWidget(Extra_Bext_MaxVersion, 1, 1);
    Extra_Bext_Layout->addWidget(Extra_Bext_Toggle, 2, 0, 1, 2);

    QGroupBox* Extra_Bext=new QGroupBox("bext");
    Extra_Bext->setLayout(Extra_Bext_Layout);

    //Extra
    QVBoxLayout* Extra=new QVBoxLayout();
    Extra->addWidget(Extra_BackupDirectory);
    Extra->addWidget(Extra_LogFile);
    Extra->addWidget(Extra_Bext);
    Extra->addStretch();
    QWidget* Extra_Widget=new QWidget();
    Extra_Widget->setLayout(Extra);
    Central->addTab(Extra_Widget, "Extra");
}

//---------------------------------------------------------------------------
void GUI_Preferences::LoadOriginalConfig()
{
    for (size_t Kind=0; Kind<Group_Max; Kind++)
        for (size_t Content_Pos=0; Content_Pos<Groups[Kind].Option_Size; Content_Pos++)
            switch (Groups[Kind].Option[Content_Pos].Type)
            {
                case Type_CheckBox      : CheckBoxes  [Kind*options::MaxCount+Content_Pos]->setChecked(Groups[Kind].Option[Content_Pos].DefaultConfigValue); break;
                case Type_RadioButton   : RadioButtons[Kind*options::MaxCount+Content_Pos]->setChecked(Groups[Kind].Option[Content_Pos].DefaultConfigValue); break;
                default                 : ;
            }
}
