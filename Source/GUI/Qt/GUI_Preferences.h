// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_PreferencesH
#define GUI_PreferencesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main.h"
#include <QDialog>
#include <QDialog>
#include <string>
class QTabWidget;
class QPushButton;
class QUrl;
class QCheckBox;
class QRadioButton;
class QLineEdit;
class QDoubleSpinBox;
class QDialogButtonBox;
class QAbstractButton;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main
//***************************************************************************

enum group
{
    Group_Tech,
    Group_Core,
    Group_Rules,
    Group_File,
    Group_MD5,
    Group_DefaultView,
    Group_Max
};

enum option_rules
{
    Option_Rules_Tech3285_Req,
    Option_Rules_Tech3285_Rec,
    Option_Rules_CodingHistory_Rec,
    Option_Rules_OriginatorReference_Rec,
    Option_Rules_INFO_Req,
    Option_Rules_INFO_Rec,
    Option_Rules_FADGI_Rec,
    Option_Rules_Max
};

enum option_file
{
    Option_File_Riff2Rf64_Reject,
    Option_File_Overwrite_Reject,
    Option_File_NoPadding_Accept,
    Option_File_FileNotValid_Skip,
    Option_File_WrongExtension_Skip,
    Option_File_NewChunksAtTheEnd,
    Option_File_Max
};

enum option_md5
{
    Option_MD5_Generate,
    Option_MD5_Verify,
    Option_MD5_Embed,
    Option_MD5_Embed_AuthorizeOverWritting,
    Option_MD5_Max
};

enum option_defaultview
{
    Option_DefaultView_Tech_Table,
    Option_DefaultView_Tech_Text,
    Option_DefaultView_Core_Table,
    Option_DefaultView_Core_Text,
    Option_DefaultView_Max
};

enum option_directories
{
    Option_Directories_BackupDefault,
    Option_Directories_Backup,
    Option_Directories_Max
};


enum type
{
    Type_CheckBox,
    Type_RadioButton,
    Type_Max,
};

struct options
{
    static const size_t MaxCount=64;

    struct option
    {
        char*   UniqueName;
        char*   Description;
        type    Type;
        bool    DefaultConfigValue;
    };
    char*       Name;
    size_t      Option_Size;
    option      Option[MaxCount];
    bool        InTemporaryPrefs;
};

class GUI_Preferences : public QDialog
{
    Q_OBJECT

public:
    GUI_Preferences (GUI_Main* parent);

    size_t      Groups_Count_Get();
    std::string Group_Name_Get(group Group);
    size_t      Group_Options_Count_Get(group Group, bool ForTemporaryPrefs=false);
    std::string Group_Option_Description_Get(group Group, size_t Option);
    bool        Group_Option_Checked_Get(group Group, size_t Option);
    bool        Group_Option_Checked_Set(group Group, size_t Option, bool Value);

private:
    //GUI
    QTabWidget*     Central;

    //Preferences
    QCheckBox**     CheckBoxes;
    QRadioButton**  RadioButtons;

 private Q_SLOTS:
    void OnLoad();
    void OnSave();
    void OnRejected();
    void OnDialogClicked(QAbstractButton*);
    void OnClicked();
    void OnExtra_BackupDirectory_Specific_RadioToggled(bool Checked);
    void OnExtra_LogFile_Activated_RadioToggled(bool Checked);

private:
    void Create();
    void LoadOriginalConfig();
    std::string ConfigDirectory_Get();
    std::string ConfigFileName_Get();

    void showEvent(QShowEvent* Event);

    GUI_Main* Main;
    QDialogButtonBox* Dialog;

    QRadioButton*   Extra_BackupDirectory_Default;
    QRadioButton*   Extra_BackupDirectory_Specific_Radio;
    QLineEdit*      Extra_BackupDirectory_Specific;
    QRadioButton*   Extra_LogFile_Deactivated;
    QRadioButton*   Extra_LogFile_Activated_Radio;
    QLineEdit*      Extra_LogFile_Activated;
    QDoubleSpinBox* Extra_Bext_DefaultVersion;
    QDoubleSpinBox* Extra_Bext_MaxVersion;
    QCheckBox*      Extra_Bext_Toggle;
};

#endif
