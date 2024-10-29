// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef CoreH
#define CoreH
#include <string>
#include <map>
#include <vector>
//#include <tchar.h>
#include "Riff/Riff_Handler.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/File.h"
#include "ZenLib/Thread.h"
#include "ZenLib/CriticalSection.h"
#include "TinyXml2/tinyxml2.h"
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Class Core
//***************************************************************************

class Core : public Thread
{
public:
    // Types
    enum Cout_Type
    {
        Cout_None=0,
        Cout_Tech,
        Cout_Tech_XML,
        Cout_Core,
        Cout_Core_XML,
        Cout__PMX,
        Cout_aXML,
        Cout_iXML,
        Cout_cue_,
        Cout_XML, //All
    };

    //Constructor/Destructor
    Core();
    ~Core();

    //Menu
    size_t                              Menu_File_Open                  (const string &DirectoryName);
    void                                Menu_File_Open_Files_Begin      ();
    size_t                              Menu_File_Open_Files_Continue   (const string &FileName);
    size_t                              Menu_File_Open_Files_Finish     ();
    bool                                Menu_File_Open_Files_Finish_Start();  //Detailed version
    float                               Menu_File_Open_Files_Finish_Middle(); //Detailed version
    void                                Menu_File_Open_Files_Finish_Middle_Threaded(); //Launch the threaded version (with Entry())
    size_t                              Menu_File_Open_Files_Finish_End ();   //Detailed version
    size_t                              Menu_File_Open_Files_Open_Get (); //Return the count of open files
    size_t                              Menu_File_Open_Files_Modified_Get (); //return the count of modified files
    bool                                Menu_File_Open_Files_BackToLastSave();
    float                               Menu_File_Open_State            ();
    void                                Menu_File_Close_File            (const string &FileName);
    ZtringList                          Menu_File_Close_File_FileName_Get();
    void                                Menu_File_Close_File_FileName_Set(const string &FileName);
    void                                Menu_File_Close_File_FileName_Clear();
    void                                Menu_File_Close_All             ();
    bool                                Menu_File_Close_All_ShouldBeDisplayed();
    bool                                Menu_File_Close_All_ShouldBeEnabled();
    size_t                              Menu_File_Save_File             (const string &FileName);
    size_t                              Menu_File_Save_File_Start       (const string &FileName);
    size_t                              Menu_File_Save                  ();
    bool                                Menu_File_Save_Start            (); //Detailed version
    void                                Menu_File_Save_Middle_Threaded  (); //Launch the threaded version (with Entry())
    float                               Menu_File_Save_Middle           (); //Detailed version
    size_t                              Menu_File_Save_End              (); //Detailed version
    bool                                Menu_File_Undo_BackupFilesExist ();
    ZtringList                          Menu_File_Undo_ListBackupFiles  ();
    string                              Menu_File_Undo_ListModifiedFiles(size_t Pos); //Pos is the position in the list from Menu_File_Undo_ListBackupFiles()
    void                                Menu_File_Undo_SelectBackupFile (size_t Pos); //Pos is the position in the list from Menu_File_Undo_ListBackupFiles()
    int                                 Menu_File_Import_Core           (const string &FileName);
    bool                                Menu_File_Options_Update        ();

    //Per file
    string                              Get                             (const string &FileName, const string &Field);
    bool                                Set                             (const string &FileName, const string &Field, const string &Value);
    string                              History                         (const string &FileName, const string &Field);
    bool                                IsValid                         (const string &FileName, const string &Field, const string &Value, bool IgnoreCoherency=false);
    string                              IsValid_LastError               (const string &FileName);
    string                              IsValid_LastWarning             (const string &FileName);
    bool                                IsModified                      (const string &FileName, const string &Field);
    string                              FileDate_Get                    (const string &FileName);
    bool                                IsValid_Get                     (const string &FileName);
    bool                                IsModified_Get                  (const string &FileName);
    bool                                IsReadOnly_Get                  (const string &FileName);

    //Actions
    const string                       &Technical_Get                   ();
    const string                       &Core_Get                        ();
    const string                       &Cout_Get                        ();
    const string                       &Output_Trace_Get                ();
    void                                Batch_Launch                    ();
    bool                                Batch_Launch_Start              (); //Detailed version
    float                               Batch_Launch_Middle             (); //Detailed version
    void                                Batch_Launch_End                (); //Detailed version
    bool                                In_Core_Add                     (const string &FileName, const string &Field, const string &Value);
    bool                                In_Core_Add                     (const string &Field, const string &Value);
    bool                                In_Chunk_Remove                 (const string &Field);
    bool                                In_Core_Append                  (const string &FileName, const string &Field, const string &Value);
    bool                                In_Core_Append                  (const string &Field, const string &Value);
    string                              Out_Core_Read                   (const string &FileName, const string &Field);

    //Configuration
    bool                                riff2rf64_Reject;
    bool                                Overwrite_Reject;
    bool                                NoPadding_Accept;
    bool                                Errors_Continue;
    bool                                FileNotValid_Skip;
    bool                                WrongExtension_Skip;
    bool                                NewChunksAtTheEnd;
    bool                                GenerateMD5;
    bool                                VerifyMD5;
    bool                                VerifyMD5_Force;
    bool                                EmbedMD5;
    bool                                EmbedMD5_AuthorizeOverWritting;
    int8u                               Bext_DefaultVersion;
    int8u                               Bext_MaxVersion;
    bool                                Simulation_Enabled;
    bool                                SpecialChars_Enabled;
    string                              Out_XML_FileName;
    Cout_Type                           Cout;
    string                              Out_Tech_CSV_FileName;
    string                              Out_Tech_XML_FileName;
    bool                                Out_Tech_XML;
    bool                                In_Core_Remove;
    bool                                In_Core_XML;
    string                              Out_Core_CSV_FileName;
    string                              Out_Core_XML_FileName;
    bool                                Out_Core_XML;
    string                              In__PMX_FileName;
    bool                                In__PMX_XML;
    bool                                In__PMX_Remove;
    string                              Out__PMX_FileName;
    bool                                Out__PMX_XML;
    string                              In_aXML_FileName;
    bool                                In_aXML_Remove;
    bool                                In_aXML_XML;
    string                              Out_aXML_FileName;
    bool                                Out_aXML_XML;
    string                              In_iXML_FileName;
    bool                                In_iXML_XML;
    bool                                In_iXML_Remove;
    string                              Out_iXML_FileName;
    bool                                Out_iXML_XML;
    string                              In_cue__FileName;
    bool                                In_cue__XML;
    bool                                In_cue__Remove;
    string                              Out_cue__FileName;
    bool                                Out_cue__XML;
    bool                                Batch_Enabled;
    bool                                Batch_IsBackuping; //Does not read modifications, only data from the file
    bool                                Out_Log_cout;
    Riff_Handler::rules                 Rules;
    string                              OpenSaveFolder;
    Ztring                              ApplicationFolder;
    Ztring                              LogFile;
    bool                                Trace_UseDec;
    Riff_Encoding                       Encoding;
    Riff_Encoding                       Fallback_Encoding;
    Riff_Encoding                       Write_Encoding;
    bool                                Write_CodePage;
    bool                                Ignore_File_Encoding;
    bool                                In_CSET_Remove;

    //Status
    void                                StdOut(string Text);
    void                                StdErr(string Text);
    ostringstream                       Text_stdall;
    ostringstream                       Text_stdout;
    ostringstream                       Text_stderr;
    bool                                Text_stderr_Updated_Get();
    size_t                              Files_Modified_NotWritten_Count;

protected:
    struct handler
    {
        Riff_Handler       *Riff;
        map<string, Ztring> In_Core;
        map<string, Ztring> In_Core_Append;
        bool                In_Core_Remove;
        bool                In__PMX_Remove;
        bool                In__PMX_XML;
        string              In__PMX_FileName;
        bool                In_aXML_Remove;
        bool                In_aXML_XML;
        string              In_aXML_FileName;
        bool                In_iXML_Remove;
        bool                In_iXML_XML;
        string              In_iXML_FileName;
        bool                In_cue__Remove;
        bool                In_cue__XML;
        string              In_cue__FileName;
        bool                In_CSET_Remove;
        ZtringList          In_Chunks_Remove;

        handler()
        {
            Riff=NULL;
            In_Core_Remove=false;
            In__PMX_Remove=false;
            In__PMX_XML=false;
            In_aXML_Remove=false;
            In_aXML_XML=false;
            In_iXML_Remove=false;
            In_iXML_XML=false;
            In_cue__Remove=false;
            In_cue__XML=false;
            In_CSET_Remove=false;
        }

        ~handler()
        {
            delete Riff; //Riff=NULL;
        }
    };
    typedef map<string, handler> handlers;
    handlers                            Handlers; //Key is the file name
    handler                             Handler_Default;
    string                              Text;

    //Internal routines
    string                              Out_XML_Buf;
    tinyxml2::XMLDocument*              Out_XML_Doc;
    File                                Out_Tech_File;
    ZtringList                          Out_Tech_CSV_File_Header;
    tinyxml2::XMLDocument*              Out_Tech_XML_Doc;
    string                              Out_Tech_XML_Buf;
    File                                Out_Core_CSV_File;
    ZtringList                          Out_Core_CSV_File_Header;
    tinyxml2::XMLDocument*              Out_Core_XML_Doc;
    string                              Out_Core_XML_Buf;
    void Batch_Begin                    ();
    void Batch_Finish                   ();
    void Batch_Launch                   (handlers::iterator &Handler);
    void Batch_Launch_Technical         (handlers::iterator &Handler);
    void Batch_Launch_Core              (handlers::iterator &Handler);
    void Batch_Launch_PMX               (handlers::iterator &Handler);
    void Batch_Launch_aXML              (handlers::iterator &Handler);
    void Batch_Launch_iXML              (handlers::iterator &Handler);
    void Batch_Launch_cue_              (handlers::iterator &Handler);
    void Batch_Launch_Write             (handlers::iterator &Handler);
    void Options_Update                 (handlers::iterator &Handler);
    void Entry();

    //Status
    void                                StdClear(handlers::iterator &Handler);
    void                                StdAll(handlers::iterator &Handler);
    bool                                Text_stderr_Updated;

    //Temp
    size_t                              Menu_File_Open_Files_File_Pos;
    size_t                              Menu_File_Open_Files_File_Total;
    CriticalSection                     CS;
    bool                                Canceled;
    handlers::iterator                  Handler;
    ZtringList                          BackupFiles;
    ZtringList                          Menu_Close_File_FileNames;
    bool                                SaveMode;
    bool                                SaveMode_OneFile;
};

#endif
