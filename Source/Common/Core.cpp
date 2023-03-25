// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"
#include "ZenLib/Ztring.h"
#include "FLACwrapper/File.h"
#include "ZenLib/Dir.h"
#include "ZenLib/OS_Utils.h"
#include "Common/Common_About.h"
#include "Riff/Riff_Handler.h"
#include <sstream>
#include <ctime>
#include <algorithm>
#ifdef _WIN32
    #undef _TEXT
    #undef __TEXT
    #include "shlobj.h"
#endif //_WIN32
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool Prepare_Xml_For_Nesting(std::string& Xml)
{
    bool UseCDATA=false;

    // tinyxml2 doesn't handle processing instructions, convert it to declarations
    string Xml_Stripped = Xml;
    for(size_t Pos=Xml_Stripped.find("<?"); Pos!=string::npos; Pos=Xml_Stripped.find("<?", Pos))
    {
            Xml_Stripped.replace(Pos, 2, "<!");
            Pos+=2;
    }

    tinyxml2::XMLDocument XmlDoc;
    if (XmlDoc.Parse(Xml_Stripped.c_str())==tinyxml2::XML_SUCCESS)
    {
        // check if xml declaration is present
        if (Xml.size()>=5 && Xml[0]=='<' && Xml[1]=='?' && tolower(Xml[2])=='x' && tolower(Xml[3])=='m' && tolower(Xml[4])=='l')
        {
            size_t End=Xml.find("?>");
            if (End!=string::npos)
            {
                // check encoding
                string XmlDecl=Xml.substr(0, End+2);
                XmlDecl.replace(End, 1, "/");
                XmlDecl.erase(1, 1);

                tinyxml2::XMLDocument Doc;
                if (Doc.Parse(XmlDecl.c_str())==tinyxml2::XML_SUCCESS && Doc.RootElement())
                {
                    const char* Enc=NULL;
                    if (Doc.RootElement()->QueryStringAttribute("encoding", &Enc)==tinyxml2::XML_SUCCESS)
                    {
                        if (!(strlen(Enc)==8
                              && tolower(Enc[0])=='u'
                              && tolower(Enc[1])=='s'
                              && tolower(Enc[2])=='-'
                              && tolower(Enc[3])=='a'
                              && tolower(Enc[4])=='s'
                              && tolower(Enc[5])=='c'
                              && tolower(Enc[6])=='i'
                              && tolower(Enc[7])=='i') &&
                            !(strlen(Enc)==5
                              && tolower(Enc[0])=='u'
                              && tolower(Enc[1])=='t'
                              && tolower(Enc[2])=='f'
                              && tolower(Enc[3])=='-'
                              && tolower(Enc[4])=='8'))
                        UseCDATA=true;
                    }
                }

                if (!UseCDATA)
                {
                    //comment out declaration for xml nesting
                    Xml.insert(End+2, "-->");
                    Xml.insert(0, "<!--");
                }
            }
            else
                UseCDATA=true;
        }
    }
    else
        UseCDATA=true;

    if (UseCDATA)
    {
        // invalid xml, put it into a CDATA
        for(size_t Pos=Xml.find("]]>"); Pos!=string::npos; Pos=Xml.find("]]>", Pos))
        {
            // Escape inner CDATA
            Xml.replace(Pos, 3, "]]]]><![CDATA[>");
            Pos+=15;
        }

        Xml.insert(0, "<![CDATA[");
        Xml.append("]]>");
    }

    return UseCDATA;
}

//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

Core::Core()
{
    //Configuration
    riff2rf64_Reject=false;
    Overwrite_Reject=false;
    NoPadding_Accept=false;
    Errors_Continue=false;
    FileNotValid_Skip=false;
    WrongExtension_Skip=false;
    NewChunksAtTheEnd=false;
    GenerateMD5=false;
    VerifyMD5=false;
    VerifyMD5_Force=false;
    EmbedMD5=false;
    EmbedMD5_AuthorizeOverWritting=false;
    Bext_DefaultVersion=0;
    Bext_MaxVersion=0;
    Simulation_Enabled=false;
    SpecialChars_Enabled=false;
    Cout=Cout_None;
    Out_Log_cout=false;

    Out_XML_Doc=NULL;

    Out_Tech_XML=false;
    Out_Tech_XML_Doc=NULL;

    In_Core_XML=false;
    In_Core_Remove=false;
    Out_Core_XML=false;
    Out_Core_XML_Doc=NULL;

    In__PMX_XML=false;
    In__PMX_Remove=false;
    Out__PMX_XML=false;
    
    In_aXML_XML=false;
    In_aXML_Remove=false;
    Out_aXML_XML=false;
    
    In_iXML_XML=false;
    In_iXML_Remove=false;
    Out_iXML_XML=false;

    In_cue__XML=false;
    In_cue__Remove=false;
    Out_cue__XML=false;

    Batch_Enabled=false;
    Batch_IsBackuping=false;

    Trace_UseDec=false;

    Encoding=Encoding_UTF8;
    Fallback_Encoding=Encoding_8859_1;
    Write_Encoding=Encoding_Max;
    Write_CodePage=false;
    Ignore_File_Encoding=false;
    In_CSET_Remove=false;

    //Status
    Text_stderr_Updated=false;
    Files_Modified_NotWritten_Count=0;
    Canceled=false;
    SaveMode=false;
    #ifdef _WIN32
        TCHAR Path[MAX_PATH];
        BOOL Result=SHGetSpecialFolderPath(NULL, Path, CSIDL_APPDATA, true);
        ApplicationFolder=Ztring(Path)+__T("\\bwfmetaedit");
    #else //_WIN32
        ApplicationFolder=Ztring().From_Local(std::getenv("HOME"))+__T("/.bwfmetaedit");
    #endif //_WIN32
}

Core::~Core()
{
}

//***************************************************************************
// Menu
//***************************************************************************

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open (const string& FileName)
{
    Menu_File_Open_Files_Begin();
    Menu_File_Open_Files_Continue(FileName);
    return Menu_File_Open_Files_Finish();
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Begin ()
{
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Continue (const string &FileName)
{
    ZtringList List;
    if (File::Exists(Ztring().From_UTF8(FileName)))
        List.push_back(Ztring().From_UTF8(FileName));
    else
    {
        List=Dir::GetAllFileNames(Ztring().From_UTF8(FileName), (Dir::dirlist_t)(Dir::Include_Files|Dir::Parse_SubDirs));
        if (List.empty())
            List.push_back(Ztring().From_UTF8(FileName));
    }

    for (size_t Pos=0; Pos<List.size(); Pos++)
        if (!WrongExtension_Skip || (List[Pos].size()>4 && Ztring(List[Pos]).MakeLowerCase().rfind(__T(".wav"))==List[Pos].size()-4))
            Handlers[List[Pos].To_UTF8()]; //Adding the reference

    return List.size();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Finish ()
{
    if (!Menu_File_Open_Files_Finish_Start())
        return 0;
    try
    {
        while (Menu_File_Open_Files_Finish_Middle()!=1.0);
    }
    catch (const char *)
    {
        return 0;
    }
    return Menu_File_Open_Files_Finish_End();
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Finish_Middle_Threaded ()
{
    SaveMode=false;
    RunAgain();
}

//---------------------------------------------------------------------------
void Core::Entry ()
{
    if (!SaveMode)
    {
        try
        {
            while (Menu_File_Open_Files_Finish_Middle()!=1.0);
        }
        catch (const char *)
        {
        }
    }
    else
        while (Menu_File_Save_Middle()!=1.0);
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Open_Files_Finish_Start()
{
    //Batch - Begin
    if (Batch_Enabled)
        Batch_Begin();

    CriticalSectionLocker CSL(CS);
    Menu_File_Open_Files_File_Pos=0;
    Menu_File_Open_Files_File_Total=Handlers.size();
    Handler=Handlers.begin();

    return Handler!=Handlers.end();
}

//---------------------------------------------------------------------------
float Core::Menu_File_Open_Files_Finish_Middle ()
{
    if (Handlers.empty())
        return 1.0; //No file

    try
    {
        if (Handler->second.Riff)
        {
            if (Handler->second.Riff->IsModified_Get())
                Files_Modified_NotWritten_Count--;
            delete Handler->second.Riff; Handler->second.Riff=NULL;
        }
        if (Handler->second.Riff==NULL || !Handler->second.Riff->IsValid_Get())
        {
            //Creation and configuration
            delete Handler->second.Riff; Handler->second.Riff=new Riff_Handler();
            StdClear(Handler);

            //Options
            Options_Update(Handler);
            
            //Settings - Removal
            Handler->second.In_Core_Remove=In_Core_Remove;
            Handler->second.In__PMX_Remove=In__PMX_Remove;
            Handler->second.In__PMX_XML=In__PMX_XML;
            Handler->second.In__PMX_FileName=In__PMX_FileName;
            Handler->second.In_aXML_Remove=In_aXML_Remove;
            Handler->second.In_aXML_XML=In_aXML_XML;
            Handler->second.In_aXML_FileName=In_aXML_FileName;
            Handler->second.In_iXML_Remove=In_iXML_Remove;
            Handler->second.In_iXML_XML=In_iXML_XML;
            Handler->second.In_iXML_FileName=In_iXML_FileName;
            Handler->second.In_cue__Remove=In_cue__Remove;
            Handler->second.In_cue__XML=In_cue__XML;
            Handler->second.In_cue__FileName=In_cue__FileName;
            Handler->second.In_CSET_Remove=In_CSET_Remove;

            //Settings - Adding default Core values if the Core value does not exist yet (from --xxx=)
            for (map<string, Ztring>::iterator In_Core_Item=Handler_Default.In_Core.begin(); In_Core_Item!=Handler_Default.In_Core.end(); In_Core_Item++)
            {
                map<string, Ztring>::iterator Handler_in_Core_Item=Handler->second.In_Core.find(Ztring().From_UTF8(In_Core_Item->first).MakeLowerCase().To_UTF8());
                if (Handler_in_Core_Item==Handler->second.In_Core.end())
                    Handler->second.In_Core[Ztring().From_UTF8(In_Core_Item->first).MakeLowerCase().To_UTF8()]=In_Core_Item->second;
            }
            
            //Special Characters
            if (SpecialChars_Enabled)
            {
                for (map<string, Ztring>::iterator Field=Handler->second.In_Core.begin(); Field!=Handler->second.In_Core.end(); Field++)
                {
                    Field->second.FindAndReplace(__T("\\\\"), __T("|SC1|"), 0, Ztring_Recursive);
                    Field->second.FindAndReplace(__T("\\r"), __T("\r"), 0, Ztring_Recursive);
                    Field->second.FindAndReplace(__T("\\n"), __T("\n"), 0, Ztring_Recursive);
                    Field->second.FindAndReplace(__T("\\t"), __T("\t"), 0, Ztring_Recursive);
                    Field->second.FindAndReplace(__T("\\0"), __T("\0"), 0, Ztring_Recursive);
                    Field->second.FindAndReplace(__T("|SC1|"), __T("\\"), 0, Ztring_Recursive);
                }
            }

            //Reading
            if (!Handler->second.Riff->Open(Handler->first))
            {
                StdAll(Handler);
                if ((FileNotValid_Skip && !Handler->second.Riff->IsValid_Get()) ||(!Errors_Continue && Text_stderr_Updated))
                    throw string(); //Error during the parsing, exiting
            }
            if (Handler->second.Riff->Canceled_Get())
            {
                delete Handler->second.Riff, Handler->second.Riff=NULL;
                CriticalSectionLocker CSL(CS);
                Canceled=true;
                throw string(); //Canceling
            }

            if (Handler->second.Riff->IsModified_Get())
                Files_Modified_NotWritten_Count++;

            StdAll(Handler);
            StdOut(Handler->first+": Is open");
        }

        bool IsModified_Old=Handler->second.Riff->IsModified_Get();

        //Modifying file with --xxx values
        if (!Handler->second.In_Core.empty())
        {
            for (map<string, Ztring>::iterator In_Core_Item=Handler->second.In_Core.begin(); In_Core_Item!=Handler->second.In_Core.end(); In_Core_Item++)
            {
                Handler->second.Riff->Set(In_Core_Item->first, In_Core_Item->second.To_UTF8(), Rules);
                StdAll(Handler);
            }
        }
        else if (Handler->second.In_Core_Remove)
        {
            Handler->second.Riff->Remove("core");
            StdAll(Handler);
        }
        if (!Handler->second.In__PMX_FileName.empty())
        {
            string File_Name("file://"+Handler->second.In__PMX_FileName);
            Handler->second.Riff->Set("xmp", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In__PMX_XML)
        {
            string File_Name("file://"+Handler->first+".XMP.xml");
            Handler->second.Riff->Set("xmp", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In__PMX_Remove)
        {
            Handler->second.Riff->Remove("xmp");
            StdAll(Handler);
        }
        if (!Handler->second.In_aXML_FileName.empty())
        {
            string File_Name("file://"+Handler->second.In_aXML_FileName);
            Handler->second.Riff->Set("axml", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In_aXML_XML)
        {
            string File_Name("file://"+Handler->first+".aXML.xml");
            Handler->second.Riff->Set("axml", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In_aXML_Remove)
        {
            Handler->second.Riff->Remove("axml");
            StdAll(Handler);
        }
        if (!Handler->second.In_iXML_FileName.empty())
        {
            string File_Name("file://"+Handler->second.In_iXML_FileName);
            Handler->second.Riff->Set("ixml", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In_iXML_XML)
        {
            string File_Name("file://"+Handler->first+".iXML.xml");
            Handler->second.Riff->Set("ixml", File_Name, Rules);
            StdAll(Handler);
        }
        else if (Handler->second.In_iXML_Remove)
        {
            Handler->second.Riff->Remove("ixml");
            StdAll(Handler);
        }
        if (Handler->second.In_cue__XML || !Handler->second.In_cue__FileName.empty())
        {
            Ztring File_Name=Ztring().From_UTF8(Handler->first+".cue.xml");
            if (!Handler->second.In_cue__FileName.empty())
                File_Name=Ztring().From_UTF8(Handler->second.In_cue__FileName);

            Ztring Value;
            FLACwrapper::File F;
            int64u F_Size;

            if (!F.Open(File_Name))
                Handler->second.Riff->Errors<<Handler->first<<": Malformed input (cue="<<File_Name.To_UTF8()<< ", File does not exist)"<<endl;
            else if ((F_Size=F.Size_Get())>((size_t)-1)-1)
                Handler->second.Riff->Errors<<Handler->first<<": Malformed input (cue="<<File_Name.To_UTF8()<< ", Unable to open file)"<<endl;
            else
            {
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
                    Handler->second.Riff->Errors<<Handler->first<<": Malformed input (cue="<<File_Name.To_UTF8()<< ", Unable to read file)"<<endl;
                else
                {
                    Buffer[Buffer_Offset]='\0';
                    Value.From_UTF8((const char*)Buffer, (size_t)F_Size);
                    if (!Handler->second.Riff->Set("cuexml", Value.To_UTF8(), Rules))
                        Handler->second.Riff->Errors<<Handler->first<<": Malformed input (cue="<<File_Name.To_UTF8()<< ", Unable to parse file)"<<endl;
                }
                delete[] Buffer;
            }
            StdAll(Handler);
        }
        else if (Handler->second.In_cue__Remove)
        {
            Handler->second.Riff->Set("cuexml", "", Rules);
            StdAll(Handler);
        }

        if (Handler->second.In_CSET_Remove)
        {
            Handler->second.Riff->Remove("cset");
            StdAll(Handler);
        }

        if (!Errors_Continue && Text_stderr_Updated)
            throw string(); //Error during the parsing, exiting

        //Batch - Activate
        if (Batch_Enabled)
            Batch_Launch(Handler);

        bool IsModified_New=Handler->second.Riff->IsModified_Get();
        if (!IsModified_Old && IsModified_New)
            Files_Modified_NotWritten_Count++;

        StdAll(Handler);

        CriticalSectionLocker CSL(CS);

        Handler++;
    }
    catch (const string &)
    {
        CriticalSectionLocker CSL(CS);

        StdAll(Handler);
        if (!FileNotValid_Skip)
        {
            //We handle it as a normal file
            Handler++;
        }
        else
        {
            //File is not a valid RIFF file, we keep it out from the map
            handlers::iterator Handler_ToDelete=Handler;
            Handler++;
            Handlers.erase(Handler_ToDelete);

            Menu_File_Open_Files_File_Pos--;
            Menu_File_Open_Files_File_Total--;
        }
    }

    CriticalSectionLocker CSL(CS);

    if (Canceled)
        return 1.0;

    Menu_File_Open_Files_File_Pos++;
    if (Menu_File_Open_Files_File_Total==0)
        return 1.0;
    return ((float)Menu_File_Open_Files_File_Pos)/Menu_File_Open_Files_File_Total;
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Finish_End()
{
    if (Handlers.empty())
        return 0; //No file

    if (Menu_File_Open_State()!=1)
    {
        CS.Enter();
        Canceled=true;
        Handler->second.Riff->Cancel();
        CS.Leave();
	    while(!IsExited())
		    Sleep(20);
    }
        
    CriticalSectionLocker CSL(CS);

    //In case of cancel
    if (Menu_File_Open_Files_File_Pos!=Menu_File_Open_Files_File_Total)
        for (Handler=Handlers.begin(); Handler!=Handlers.begin(); Handler++) 
        {
            if (Handler->second.Riff==NULL)
            {
                //Removing not parsed files
                handlers::iterator Handler_ToDelete=Handler;
                Handler++;
                Handlers.erase(Handler_ToDelete);

                CriticalSectionLocker CSL(CS);
                Menu_File_Open_Files_File_Pos--;
                Menu_File_Open_Files_File_Total--;
            }
        }

    //Batch - Finish
    if (Batch_Enabled)
        Batch_Finish();

    Canceled=false;
    return Handlers.size();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Open_Get()
{
    return Handlers.size();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Modified_Get()
{
    return Files_Modified_NotWritten_Count;
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Open_Files_BackToLastSave()
{
    for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
        if (Handler->second.Riff)
        {
            bool IsModified_Old=Handler->second.Riff->IsModified_Get();
            Handler->second.Riff->BackToLastSave();
            bool IsModified_New=Handler->second.Riff->IsModified_Get();

            if ( IsModified_Old && !IsModified_New)
                Files_Modified_NotWritten_Count--;
        }

    return true;
}

//---------------------------------------------------------------------------
float Core::Menu_File_Open_State ()
{
    CriticalSectionLocker CSL(CS);
    if (Menu_File_Open_Files_File_Total==0)
        return 1.0; //No file
    float ToReturn=(float)Menu_File_Open_Files_File_Pos;
    if (Handler!=Handlers.end() && Handler->second.Riff)
        ToReturn+=Handler->second.Riff->Progress_Get();
    ToReturn/=Menu_File_Open_Files_File_Total;
    return ToReturn;
}

//---------------------------------------------------------------------------
void Core::Menu_File_Close_File(const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler!=Handlers.end())
    {
        if (Handler->second.Riff && Handler->second.Riff->IsModified_Get())
            Files_Modified_NotWritten_Count--;
        Handlers.erase(Handler);
    }
}

//---------------------------------------------------------------------------
ZtringList Core::Menu_File_Close_File_FileName_Get()
{
    return Menu_Close_File_FileNames;
}

//---------------------------------------------------------------------------
void Core::Menu_File_Close_File_FileName_Set(const string &FileName)
{
    Menu_Close_File_FileNames.push_back(Ztring().From_UTF8(FileName));
}

//---------------------------------------------------------------------------
void Core::Menu_File_Close_File_FileName_Clear()
{
    Menu_Close_File_FileNames.clear();
}

//---------------------------------------------------------------------------
void Core::Menu_File_Close_All()
{
    Handlers.clear();
    Files_Modified_NotWritten_Count=0;
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Close_All_ShouldBeDisplayed()
{
    return Menu_Close_File_FileNames.size()!=Handlers.size();
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Close_All_ShouldBeEnabled()
{
    return !Handlers.empty();
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Undo_BackupFilesExist()
{
    if (!Dir::Exists(ApplicationFolder))
        return false;

    BackupFiles=Dir::GetAllFileNames(ApplicationFolder+PathSeparator+__T("Backup-*.csv"));

    return !BackupFiles.empty();
}

//---------------------------------------------------------------------------
bool Menu_File_Undo_ListBackupFiles_sort_function (const Ztring &i, const Ztring &j) {return (i.compare(j)>0);}
ZtringList Core::Menu_File_Undo_ListBackupFiles()
{
    if (!Dir::Exists(ApplicationFolder))
        return ZtringList();
    
    BackupFiles=Dir::GetAllFileNames(ApplicationFolder+PathSeparator+__T("Backup-*.csv"));
    std::sort(BackupFiles.begin(), BackupFiles.end(), Menu_File_Undo_ListBackupFiles_sort_function);
    
    ZtringList BackupFiles_ToReturn=BackupFiles;
    for (size_t Pos=0; Pos<BackupFiles_ToReturn.size(); Pos++)
    {
        ZenLib::Ztring Value=BackupFiles[Pos];
        size_t BackupPos=Value.rfind(__T("Backup-"));
        if (BackupPos!=(string::npos) && BackupPos+7+8+1+8<Value.size())
        {
            Value=Value.substr(BackupPos+7, 10+1+8);
            Value[10]=' ';
            Value[10+1+2]=':';
            Value[10+1+5]=':';
            BackupFiles_ToReturn[Pos]=Value;
        }
    }

    return BackupFiles_ToReturn;
}

//---------------------------------------------------------------------------
string Core::Menu_File_Undo_ListModifiedFiles(size_t Pos)
{
    //Retrieving file name
    if (Pos>BackupFiles.size())
        return string();
    Ztring FileName=BackupFiles[Pos];

    //Opening the file
    FLACwrapper::File F;
    if (!F.Open(FileName))
        return string();
    int64u F_Size=F.Size_Get();
    if (F_Size>((size_t)-1)-1)
        return string();

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
        return string();
    Buffer[Buffer_Offset]='\0';

    //Filling
    Ztring ModifiedContent((const char*)Buffer);
    delete[] Buffer;
    ModifiedContent.FindAndReplace(__T("\r\n"), __T("\n"), 0, Ztring_Recursive);
    ModifiedContent.FindAndReplace(__T("\r"), __T("\n"), 0, Ztring_Recursive);

    //Showing
    ZtringListList List;
    List.Separator_Set(0, __T("\n"));
    List.Separator_Set(1, __T(","));
    List.Write(ModifiedContent);

    //Elminating unuseful info from filenames
    Ztring FileName_Before;
    if (List.size()==2 && !List[1].empty())
    {
        size_t ToDelete=List[1][0].rfind(PathSeparator);
        if (ToDelete!=string::npos)
        {
            FileName_Before=List[1][0].substr(0, ToDelete+1);
            List[1][0].erase(0, ToDelete+1);
        }
    }
    else if (List.size()>1 && !List[1].empty())
    {
        size_t ToDelete;
        size_t File_Pos;
        for (ToDelete=0; ToDelete<List[1][0].size(); ToDelete++)
        {
            Char Char_File1=List[1][0][ToDelete];
            for (File_Pos=2; File_Pos<List.size(); File_Pos++)
                if (ToDelete>=List[File_Pos][0].size() || List[File_Pos][0][ToDelete]!=Char_File1)
                    break;
            if (File_Pos!=List.size())
                break;
        }

        ToDelete=List[1][0].rfind(PathSeparator, ToDelete);
        if (ToDelete!=string::npos)
        {
            FileName_Before=List[1][0].substr(0, ToDelete+1);
            for (File_Pos=1; File_Pos<List.size(); File_Pos++)
                List[File_Pos][0].erase(0, ToDelete+1);
        }
    }

    //Filling
    Ztring ToDisplay=__T("Root directory:\n")+FileName_Before+__T("\n\nModified Files:\n");
    for (size_t File_Pos=1; File_Pos<List.size(); File_Pos++)
    {
        if (!List[File_Pos].empty())
            ToDisplay+=List[File_Pos][0]+__T("\n");
    }
    
    return ToDisplay.To_UTF8();
}

//---------------------------------------------------------------------------
void Core::Menu_File_Undo_SelectBackupFile(size_t Pos)
{
    //Integrity
    if (Pos>BackupFiles.size())
        return;

    //Backuping
    Riff_Handler::rules Rules_Sav=Rules;

    //Configuring
    Rules.Tech3285_Req=false;
    Rules.Tech3285_Rec=false;
    Rules.CodingHistory_Rec=false;
    Rules.OriginatorReference_Rec=false;
    Rules.INFO_Req=false;
    Rules.INFO_Rec=false;
    Rules.FADGI_Rec=false;
    Rules.EBU_ISRC_Rec=false;

    //Launching
    Menu_File_Import_Core(BackupFiles[Pos].To_UTF8());

    //Restoring
    Rules=Rules_Sav;
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Save_File (const string &FileName)
{
    if (!Dir::Exists(ApplicationFolder))
    {
        Dir::Create(ApplicationFolder);
        if (!Dir::Exists(ApplicationFolder))
            return 0;
    }

    handlers::iterator Handler=Handlers.find(FileName);

    if( Handler==Handlers.end())
        return 0;

    //Backup
    Batch_IsBackuping=true;
    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);
    TimeS.FindAndReplace(__T(":"), __T("-"), 0, Ztring_Recursive);
    TimeS.FindAndReplace(__T(" "), __T("-"), 0, Ztring_Recursive);
    Out_Core_CSV_FileName=ApplicationFolder.To_UTF8()+"/Backup-"+TimeS.To_UTF8()+".csv";
    Batch_Launch(Handler);
    Out_Core_CSV_FileName.clear();
    Batch_IsBackuping=false;
    
    //Running
    Batch_Launch(Handler);

    return 1;
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Save ()
{
    if (!Dir::Exists(ApplicationFolder))
    {
        Dir::Create(ApplicationFolder);
        if (!Dir::Exists(ApplicationFolder))
            return 0;
    }

    //Backup
    Batch_IsBackuping=true;
    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);
    TimeS.FindAndReplace(__T(":"), __T("-"), 0, Ztring_Recursive);
    TimeS.FindAndReplace(__T(" "), __T("-"), 0, Ztring_Recursive);
    Out_Core_CSV_FileName=ApplicationFolder.To_UTF8()+"/Backup-"+TimeS.To_UTF8()+".csv";
    Batch_Launch();
    Out_Core_CSV_FileName.clear();
    Batch_IsBackuping=false;
    
    //Running
    Batch_Launch();
    
    return Handlers.size();
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Save_Start ()
{
    if (!Dir::Exists(ApplicationFolder))
    {
        Dir::Create(ApplicationFolder);
        if (!Dir::Exists(ApplicationFolder))
            return 0;
    }
    
    //Backup
    Batch_IsBackuping=true;
    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);
    TimeS.FindAndReplace(__T(":"), __T("-"), 0, Ztring_Recursive);
    TimeS.FindAndReplace(__T(" "), __T("-"), 0, Ztring_Recursive);
    Out_Core_CSV_FileName=ApplicationFolder.To_UTF8()+"/Backup-"+TimeS.To_UTF8()+".csv";
    bool Simulation_Enabled_Save=Simulation_Enabled;
    Simulation_Enabled=true;
    Batch_Launch();
    Simulation_Enabled=Simulation_Enabled_Save;
    Out_Core_CSV_FileName.clear();
    Batch_IsBackuping=false;
    
    bool ToReturn=Batch_Launch_Start();

    //Menu_File_Open_Files_File_Pos=0;
    //Menu_File_Open_Files_File_Total=Files_Modified_NotWritten_Count;
    
    return ToReturn;
}

//---------------------------------------------------------------------------
void Core::Menu_File_Save_Middle_Threaded ()
{
    SaveMode=true;
    RunAgain();
}

//---------------------------------------------------------------------------
float Core::Menu_File_Save_Middle ()
{
    Handler->second.Riff->Progress_Clear();
    return Batch_Launch_Middle();
    CriticalSectionLocker CSL(CS);
    Menu_File_Open_Files_File_Pos++;
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Save_End ()
{
    if (Menu_File_Open_State()!=1)
    {
        CS.Enter();
        Canceled=true;
        Handler->second.Riff->Cancel();
        CS.Leave();
        while(!IsExited())
            Sleep(20);
    }

    Batch_Launch_End();
    
    return Handlers.size();
}

//---------------------------------------------------------------------------
int Core::Menu_File_Import_Core(const string &FileName)
{
    int ReturnValue=0;
    bool Errors_Validity_Detected=false;

    int8u *Buffer=NULL;
    try
    {
        //Checking if file exists
        FLACwrapper::File In_Bext_File;
        if (!In_Bext_File.Open(Ztring().From_UTF8(FileName)))
            throw "--in-core=: file does not exist";
        int64u File_Size=In_Bext_File.Size_Get();
        if (File_Size>((size_t)-1)-1)
            throw "--in-core=: file is too big, it can not be loaded";

        //Creating buffer
        Buffer=new int8u[(size_t)File_Size+1];
        size_t Buffer_Offset=0;

        //Reading the file
        while(Buffer_Offset<File_Size)
        {
            size_t BytesRead=In_Bext_File.Read(Buffer+Buffer_Offset, (size_t)File_Size-Buffer_Offset);
            if (BytesRead==0)
                break; //Read is finished
            Buffer_Offset+=BytesRead;
        }
        if (Buffer_Offset<File_Size)
            throw "--in-core=: error during reading";
        Buffer[Buffer_Offset]='\0';

        //Finding the right line separator
        ZtringListList List;
        size_t CRLF_Pos=0;
        for (; CRLF_Pos<Buffer_Offset; CRLF_Pos++)
        {
            if (Buffer[CRLF_Pos]=='\n')
            {
                List.Separator_Set(0, "\n"); //Unix
                break;
            }
            else if (Buffer[CRLF_Pos]=='\r')
            {
                if (Buffer[CRLF_Pos+1]=='\n')
                    List.Separator_Set(0, "\r\n"); //Windows
                else
                    List.Separator_Set(0, "\r"); //Mac
                break;
            }
        }
        if (CRLF_Pos==Buffer_Offset)
            throw "--in-core=: not a valid file";

        //Testing if this is a valid file - Begins with "FileName"
        if (Buffer_Offset>=8
         && Buffer[0]=='F'
         && Buffer[1]=='i'
         && Buffer[2]=='l'
         && Buffer[3]=='e'
         && Buffer[4]=='N'
         && Buffer[5]=='a'
         && Buffer[6]=='m'
         && Buffer[7]=='e')
        {
            //CSV
            //Loading data in an array
            List.Separator_Set(1, ",");
            List.Write((const char*)Buffer);
            delete[] Buffer; Buffer=NULL;
            if (List.empty())
                throw "--in-core=: not a valid file";
            for (size_t Pos=0; Pos<List[0].size(); Pos++)
                List[0][Pos].MakeLowerCase();

            //Loading each file and saving Core data
            for (size_t File_Pos=1; File_Pos<List.size(); File_Pos++)
                if (!List[File_Pos].empty())
                {
                    //Adapting file names
                    #ifdef _WIN32
                        List[File_Pos][0].FindAndReplace(__T("/"), __T("\\"));
                    #else
                        List[File_Pos][0].FindAndReplace(__T("\\"), __T("/"));
                    #endif
                    
                    //Saving core items
                    for (size_t Pos=0; Pos<List[0].size(); Pos++)
                    {
                        bool Result=In_Core_Add(List[File_Pos][0].To_UTF8(), List[0][Pos].To_UTF8(), Pos<List[File_Pos].size()?List[File_Pos](Pos).To_UTF8():string());
                        if (!Result)
                            Errors_Validity_Detected=true;
                    }

                    ReturnValue++;
                }

            Menu_File_Open_Files_Finish();
        }
        else if (Buffer_Offset>=8
         && Buffer[0]=='<'
         && Buffer[1]=='?'
         && Buffer[2]=='x'
         && Buffer[3]=='m'
         && Buffer[4]=='l'
        )
        {
            tinyxml2::XMLDocument document;
            if (document.LoadFile(FileName.c_str())==tinyxml2::XML_SUCCESS)
            {
	            tinyxml2::XMLElement* Root=document.FirstChildElement("conformance_point_document");
	            if (Root)
	            {
		            tinyxml2::XMLElement* File=Root->FirstChildElement("File");
                    while (File)
	                {
                        string FileName=File->Attribute("name");
                        tinyxml2::XMLElement* Core=File->FirstChildElement("Core");
		                if (Core)
		                {
			                tinyxml2::XMLElement* Element=Core->FirstChildElement();
			                while (Element)
			                {
                                string Field=Element->Name();
                                const char* Value=Element->GetText();
                                if (!Field.empty())
                                    In_Core_Add(FileName, Field=="TimeReference_translated"?"TimeReference (translated)":Field.c_str(), Value?string(Value):string());

                                Element=Element->NextSiblingElement();
                            }
                        }
                        File=File->NextSiblingElement("File");
                    }
                }
            }

            Menu_File_Open_Files_Finish();
        }
        else
            throw "--in-core=: not a valid file";


        if (Errors_Validity_Detected)
        {
            StdErr("--in-core=: error in fields are detected");
            ReturnValue=Errors_Continue?0:-1;
        }
    }
    catch (bad_alloc &)
    {
        StdErr("--in-core=: file is too big, it can not be loaded");
        ReturnValue=Errors_Continue?0:-1;
    }
    catch (const char *Message)
    {
        StdErr(Message);
        ReturnValue=Errors_Continue?0:-1;
    }
    catch (...) {}

    delete[] Buffer; Buffer=NULL;
    
    return ReturnValue;
}

//---------------------------------------------------------------------------
bool Core::Menu_File_Options_Update()
{
    for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
        Options_Update(Handler);
    
    return true;
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
const string& Core::Technical_Get ()
{
    Cout_Type Backup=Cout;

    Cout=Cout_Tech;
    Cout_Get();
    Cout=Backup;

    return Text;
}

//---------------------------------------------------------------------------
const string& Core::Core_Get ()
{
    Cout_Type Backup=Cout;

    Cout=Cout_Core;
    Cout_Get();
    Cout=Backup;

    return Text;
}

//---------------------------------------------------------------------------
const string& Core::Cout_Get ()
{
    Text.clear();

    if (Handlers.empty())
        return Text;

    ZtringListList List;
    switch (Cout)
    {
    case Cout_None:
        break;
    case Cout_Tech:
        Text=Riff_Handler::Technical_Header();
        Text+=Ztring(EOL).To_UTF8();
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                Text+=Handler->second.Riff->Technical_Get()+Ztring(EOL).To_UTF8();
        break;
    case Cout_Tech_XML:
        Text=Out_Tech_XML_Buf;
        break;
    case Cout_Core:
        List.Separator_Set(0, EOL);
        List.Separator_Set(1, __T(","));
        List.push_back(Ztring().From_UTF8(Riff_Handler::Core_Header()));
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                List.push_back(Ztring().From_UTF8(Handler->second.Riff->Core_Get(Batch_IsBackuping)));
        Text=List.Read().To_UTF8();
        break;
    case Cout_Core_XML:
        Text=Out_Core_XML_Buf;
        break;
    case Cout__PMX:
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                Text+=Handler->second.Riff->Get("xmp")+Ztring(EOL).To_UTF8();
        break;
    case Cout_aXML:
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                Text+=Handler->second.Riff->Get("axml")+Ztring(EOL).To_UTF8();
        break;
    case Cout_iXML:
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                Text+=Handler->second.Riff->Get("ixml")+Ztring(EOL).To_UTF8();
        break;
    case Cout_cue_:
        for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
            if (Handler->second.Riff)
                Text+=Handler->second.Riff->Get("cuexml")+Ztring(EOL).To_UTF8();
        break;
    case Cout_XML:
        Text=Out_XML_Buf;
        break;
    default:
        break;
    }

    return Text;
}

//---------------------------------------------------------------------------
const string& Core::Output_Trace_Get ()
{
    Text.clear();

    for (handlers::iterator Handler=Handlers.begin(); Handler!=Handlers.end(); Handler++)
        if (Handler->second.Riff)
            Text+=Handler->first+Ztring(EOL).To_UTF8()+Handler->second.Riff->Trace_Get()+Ztring(EOL).To_UTF8()+Ztring(EOL).To_UTF8();

    return Text;
}

//---------------------------------------------------------------------------
bool Core::In_Core_Add (const string &FileName, const string &Field, const string &Value)
{
    if (Value=="NOCHANGE")
        return true;
   
    if (File::Exists(Ztring().From_UTF8(FileName)))
        Handlers[FileName].In_Core[Field]=Ztring().From_UTF8(Value);
    else
    {
        //Handling wildcards
        ZtringList List=Dir::GetAllFileNames(Ztring().From_UTF8(FileName));
        if (List.empty())
            List.push_back(Ztring().From_UTF8(FileName));

        for (size_t Pos=0; Pos<List.size(); Pos++)
            Handlers[List[Pos].To_UTF8()].In_Core[Field]=Ztring().From_UTF8(Value);
    }

    return true;
}

//---------------------------------------------------------------------------
bool Core::In_Core_Add (const string &Field, const string &Value)
{
    if (Value=="NOCHANGE")
        return true;
   
    Handler_Default.In_Core[Field]=Ztring().From_UTF8(Value);

    return true;
}

//---------------------------------------------------------------------------
string Core::Out_Core_Read (const string &FileName, const string &Field)
{
    if (Handlers[FileName].Riff==NULL)
        return string();
    return Handlers[FileName].Riff->Get(Field);
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
bool Core::Set (const string &FileName, const string &Field, const string &Value)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return true; //file is not registred

    Handler->second.In_Core[Field]=Ztring().From_UTF8(Value);
    if (Handler->second.Riff)
    {
        StdClear(Handler);
        bool IsModified_Old=IsModified_Get(FileName);
        Handler->second.Riff->Bext_DefaultVersion=Bext_DefaultVersion;
        Handler->second.Riff->Bext_MaxVersion=Bext_MaxVersion;
        if (!Handler->second.Riff->Set(Field, Value, Rules))
            return false;
        bool IsModified_New=IsModified_Get(FileName);

        if (!IsModified_Old && IsModified_New)
            Files_Modified_NotWritten_Count++;
        if ( IsModified_Old && !IsModified_New)
            Files_Modified_NotWritten_Count--;

        StdAll(Handler);
    }

    return true;
}

//---------------------------------------------------------------------------
bool Core::IsModified (const string &FileName, const string &Field)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return false; //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsModified(Field);

    return false;
}

//---------------------------------------------------------------------------
bool Core::IsValid_Get (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return false; //File is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsValid_Get();

    return false;
}

//---------------------------------------------------------------------------
bool Core::IsModified_Get (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return false; //File is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsModified_Get();

    return false;
}

//---------------------------------------------------------------------------
bool Core::IsReadOnly_Get (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return false; //File is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsReadOnly_Get();

    return false;
}
//---------------------------------------------------------------------------
bool Core::IsValid (const string &FileName, const string &Field, const string &Value, bool IgnoreCoherency)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return false; //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsValid(Ztring().From_UTF8(Field).MakeLowerCase().To_UTF8(), Value, Rules, IgnoreCoherency);

    return false;
}

//---------------------------------------------------------------------------
string Core::IsValid_LastError (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return string(); //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsValid_LastError();

    return string();
}

//---------------------------------------------------------------------------
string Core::IsValid_LastWarning (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return string(); //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->IsValid_LastWarning();

    return string();
}

//---------------------------------------------------------------------------
string Core::Get (const string &FileName, const string &Field)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return string(); //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->Get(Field);

    return string();
}

//---------------------------------------------------------------------------
string Core::History (const string &FileName, const string &Field)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return string(); //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->History(Ztring().From_UTF8(Field).MakeLowerCase().To_UTF8());

    return string();
}

//---------------------------------------------------------------------------
string Core::FileDate_Get (const string &FileName)
{
    handlers::iterator Handler=Handlers.find(FileName);
    if (Handler==Handlers.end())
        return string(); //file is not registred

    if (Handler->second.Riff)
        return Handler->second.Riff->FileDate_Get();

    return string();
}

//***************************************************************************
// Internal routines
//***************************************************************************

//---------------------------------------------------------------------------
void Core::Batch_Begin()
{
    //--out-technical-file out-technical-XML preparation
    if (!Out_Tech_CSV_FileName.empty()  || !Out_Tech_XML_FileName.empty() || Out_Tech_XML || !Out_XML_FileName.empty() || Cout==Cout_XML || Cout==Cout_Tech_XML)
    {
        Out_Tech_CSV_File_Header.Separator_Set(0, __T(","));
        Out_Tech_CSV_File_Header.Write(Ztring().From_UTF8(Riff_Handler::Technical_Header()));
        Ztring Header;
        Header+=Ztring().From_UTF8(Riff_Handler::Technical_Header());
        Header+=EOL;
        if (!Out_Tech_CSV_FileName.empty()) //Only for --out-technical-file
        {
            try
            {
                if (!Out_Tech_File.Create(Ztring().From_UTF8(Out_Tech_CSV_FileName)))
                    throw "--out-Technical-file: error during file creation";
                if (!Out_Tech_File.Write(Header))
                    throw "--out-Technical-file: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
                Out_Tech_File.Close();
                Out_Tech_CSV_FileName.clear();
            }
            catch (...) {}
        }

        if (!Out_Tech_XML_FileName.empty() || Cout==Cout_Tech_XML) //--out-Tech-XML=file preparation
        {
            Out_Tech_XML_Doc=new tinyxml2::XMLDocument();
            Out_Tech_XML_Doc->InsertEndChild(Out_Tech_XML_Doc->NewDeclaration());
            Out_Tech_XML_Doc->InsertEndChild(Out_Tech_XML_Doc->NewElement("conformance_point_document"));
        }
    }

    //--out-Core-CSV=file and --out-Core-XML=file and out-Core-XML preparation
    if (!Out_Core_CSV_FileName.empty() || !Out_Core_XML_FileName.empty() || Out_Core_XML || !Out_XML_FileName.empty() || Cout==Cout_XML || Cout==Cout_Core_XML)
    {
        Out_Core_CSV_File_Header.Separator_Set(0, __T(","));
        Out_Core_CSV_File_Header.Write(Ztring().From_UTF8(Riff_Handler::Core_Header()));

        if (!Out_Core_CSV_FileName.empty()) //Only for --out-Core-CSV=file
        {
            Ztring Header;
            Header+=Ztring().From_UTF8(Riff_Handler::Core_Header());
            Header+=EOL;
            try
            {
                if (!Out_Core_CSV_File.Create(Ztring().From_UTF8(Out_Core_CSV_FileName)))
                    throw "--out-Core-CSV=file: error during file creation";
                if (!Out_Core_CSV_File.Write(Header))
                    throw "--out-Core-CSV=file: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
                Out_Core_CSV_File.Close();
                Out_Core_CSV_FileName.clear();
            }
            catch (...) {}
        }

        if (!Out_Core_XML_FileName.empty() || Cout==Cout_Core_XML) //--out-Core-XML=file preparation
        {
            Out_Core_XML_Doc=new tinyxml2::XMLDocument();
            Out_Core_XML_Doc->InsertEndChild(Out_Core_XML_Doc->NewDeclaration());
            Out_Core_XML_Doc->InsertEndChild(Out_Core_XML_Doc->NewElement("conformance_point_document"));
        }
    }

    if (!Out_XML_FileName.empty() || Cout==Cout_XML)
    {
        Out_XML_Doc=new tinyxml2::XMLDocument();
        Out_XML_Doc->InsertEndChild(Out_XML_Doc->NewDeclaration());
        Out_XML_Doc->InsertEndChild(Out_XML_Doc->NewElement("conformance_point_document"));
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Finish()
{
    //--out-technical-file
    if (!Out_Tech_CSV_FileName.empty())
        Out_Tech_File.Close();

    //--out-Core-CSV=file
    if (!Out_Core_CSV_FileName.empty())
        Out_Core_CSV_File.Close();

    //--out-Tech-XML=file
    if (Out_Tech_XML_Doc)
    {
        if (Cout==Cout_Tech_XML)
        {
            tinyxml2::XMLPrinter Printer;
            Out_Tech_XML_Doc->Print(&Printer);
            Out_Tech_XML_Buf=Printer.CStr();
        }

        if (!Out_Tech_XML_FileName.empty())
        {
            if (Out_Tech_XML_Doc->SaveFile(Out_Tech_XML_FileName.c_str())!=tinyxml2::XML_SUCCESS)
                StdErr("--out-Tech-XML=file: error during file writing");
        }

        Out_Tech_XML_Doc->Clear();
        delete Out_Tech_XML_Doc; Out_Tech_XML_Doc=NULL;
    }

    //--out-Core-XML=file
    if (Out_Core_XML_Doc)
    {
        if (Cout==Cout_Core_XML)
        {
            tinyxml2::XMLPrinter Printer;
            Out_Core_XML_Doc->Print(&Printer);
            Out_Core_XML_Buf=Printer.CStr();
        }

        if (!Out_Core_XML_FileName.empty())
        {
            if (Out_Core_XML_Doc->SaveFile(Out_Core_XML_FileName.c_str())!=tinyxml2::XML_SUCCESS)
                StdErr("--out-Core-XML=file: error during file writing");
        }

        Out_Core_XML_Doc->Clear();
        delete Out_Core_XML_Doc; Out_Core_XML_Doc=NULL;
    }

    //--out-XML=file
    if (Out_XML_Doc)
    {
        tinyxml2::XMLPrinter Printer;
        Out_XML_Doc->Print(&Printer);
        Out_XML_Buf=Printer.CStr();

        for(size_t Pos=Out_XML_Buf.find("\n            <PLACEHOLDER><![CDATA["); Pos!=string::npos; Pos=Out_XML_Buf.find("\n            <PLACEHOLDER><![CDATA[", Pos))
        {
            Out_XML_Buf.erase(Pos, 35);
            Pos=Out_XML_Buf.find("]]></PLACEHOLDER>\n        ", Pos);
            if (Pos!=string::npos)
                Out_XML_Buf.erase(Pos, 26);
        }

        if (!Out_XML_FileName.empty())
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Out_XML_FileName)))
                    throw "--out-XML: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Out_XML_Buf)))
                    throw "--out-XML: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (Cout!=Cout_XML)
            Out_XML_Buf.clear();

        Out_XML_Doc->Clear();
        delete Out_XML_Doc; Out_XML_Doc=NULL;
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch()
{
    if (!Batch_Launch_Start())
        return;
    while (Batch_Launch_Middle()!=1.0);
    Batch_Launch_End();
}

//---------------------------------------------------------------------------
bool Core::Batch_Launch_Start()
{
    //Integrity
    if (Handlers.empty())
        return false;

    Batch_Begin();

    CriticalSectionLocker CSL(CS);
    Menu_File_Open_Files_File_Pos=0;
    Menu_File_Open_Files_File_Total=Handlers.size();
    Handler=Handlers.begin();

    return Handler!=Handlers.end();
}

//---------------------------------------------------------------------------
float Core::Batch_Launch_Middle()
{
    //Integrity
    if (Handlers.empty() || Handler->second.Riff==NULL)
        return 1.0;

    //Batch - Activate
    Handler->second.Riff->riff2rf64_Reject=riff2rf64_Reject;
    Batch_Launch(Handler);
    
    CriticalSectionLocker CSL(CS);

    Handler++;

    Menu_File_Open_Files_File_Pos++;
    if (Menu_File_Open_Files_File_Total==0)
        return 1.0;
    return ((float)Menu_File_Open_Files_File_Pos)/Menu_File_Open_Files_File_Total;
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_End()
{
    //Integrity
    if (Handlers.empty())
        return;

    Batch_Finish();
}

//---------------------------------------------------------------------------
void Core::Batch_Launch(handlers::iterator &Handler)
{
    //Unified
    if (Out_XML_Doc)
    {
        tinyxml2::XMLElement* File=Out_XML_Doc->NewElement("File");
        File->SetAttribute("name", Handler->second.Riff->FileName_Get().c_str());

        Out_XML_Doc->RootElement()->InsertEndChild(File);
    }

    //Technical
    if (!Out_Tech_CSV_FileName.empty() || Out_Tech_XML || Out_Tech_XML_Doc || Out_XML_Doc)
        Batch_Launch_Technical(Handler);

    //Bext chunk
    if (!Out_Core_CSV_FileName.empty() || Out_Core_XML || Out_Core_XML_Doc || Out_XML_Doc)
        Batch_Launch_Core(Handler);

    //XMP chunk
    if (Out__PMX_XML || !Out__PMX_FileName.empty() || Out_XML_Doc)
        Batch_Launch_PMX(Handler);

    //aXML chunk
    if (Out_aXML_XML || !Out_aXML_FileName.empty() || Out_XML_Doc)
        Batch_Launch_aXML(Handler);

    //iXML chunk
    if (Out_iXML_XML || !Out_iXML_FileName.empty() || Out_XML_Doc)
        Batch_Launch_iXML(Handler);

    //cue_ related metadata
    if (Out_cue__XML || !Out_cue__FileName.empty() || Out_XML_Doc)
        Batch_Launch_cue_(Handler);

    //Write
    if (!Simulation_Enabled)
        Batch_Launch_Write(Handler);
    else if (Handler->second.Riff->IsModified_Get() && !Batch_IsBackuping)
        StdOut(Handler->first+": would be modified (if no simulation)"); //Log
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_Technical(handlers::iterator &Handler)
{
    Ztring Technical=Ztring().From_UTF8(Handler->second.Riff->Technical_Get());

    //Technical chunk (with a Conformance Point Document)
    if (!Out_Tech_CSV_FileName.empty())
        if (!Out_Tech_File.Write(Technical+EOL)) //Saving file part
        {
            StdErr("--out-technical-file: error during file writing");
            Out_Tech_File.Close();
            Out_Tech_CSV_FileName.clear();
        }

    if (Out_Tech_XML || Out_Tech_XML_Doc || Out_XML_Doc)
    {
        //Retrieving content
        ZtringList List_Content;
        List_Content.Separator_Set(0, ",");
        List_Content.Write(Technical);

        //Preparing XML file
        Ztring Content;
        if (Technical.find(',')!=string::npos)
        {
            tinyxml2::XMLDocument Document;
            tinyxml2::XMLElement* Technical=Document.NewElement("Technical");
            for (size_t Header_Pos=1; Header_Pos<Out_Tech_CSV_File_Header.size(); Header_Pos++)
            {
                if (Header_Pos<List_Content.size() && !List_Content[Header_Pos].empty())
                {
                    Out_Tech_CSV_File_Header[Header_Pos].FindAndReplace(__T(" "), __T("_"), 0, Ztring_Recursive);
                    Out_Tech_CSV_File_Header[Header_Pos].FindAndReplace(__T("("), __T(""), 0, Ztring_Recursive);
                    Out_Tech_CSV_File_Header[Header_Pos].FindAndReplace(__T(")"), __T(""), 0, Ztring_Recursive);
                    Technical->InsertNewChildElement(Out_Tech_CSV_File_Header[Header_Pos].To_UTF8().c_str())->SetText(List_Content[Header_Pos].To_UTF8().c_str());
                }
            }

            if (Out_Tech_XML_Doc)
            {
                tinyxml2::XMLElement* File=Out_Tech_XML_Doc->NewElement("File");
                File->SetAttribute("name", List_Content[0].To_UTF8().c_str());

                tinyxml2::XMLNode* Content=Technical->DeepClone(Out_Tech_XML_Doc);
                File->InsertEndChild(Content);

                Out_Tech_XML_Doc->RootElement()->InsertEndChild(File);
            }

            if (Out_Tech_XML)
            {

                tinyxml2::XMLDocument Output_Document;
                Output_Document.InsertEndChild(Output_Document.NewDeclaration());
                Output_Document.InsertEndChild(Output_Document.NewElement("conformance_point_document"));

                tinyxml2::XMLElement* File=Output_Document.NewElement("File");
                File->SetAttribute("name", List_Content[0].To_UTF8().c_str());

                tinyxml2::XMLNode* Content=Technical->DeepClone(&Output_Document);
                File->InsertEndChild(Content);

                Output_Document.RootElement()->InsertEndChild(File);

                if (Output_Document.SaveFile(Handler->second.Riff->FileName_Get().append(".Technical.xml").c_str())!=tinyxml2::XML_SUCCESS)
                    StdErr("--out-Tech-XML: error during file writing");
            }

            if (Out_XML_Doc)
            {
                tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
                if (File)
                {
                    tinyxml2::XMLNode* Content=Technical->DeepClone(Out_XML_Doc);
                    File->InsertEndChild(Content);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_Core(handlers::iterator &Handler)
{
    Ztring Core=Ztring().From_UTF8(Handler->second.Riff->Core_Get(Batch_IsBackuping));

    //Core chunk (with a Conformance Point Document)
    if (!Out_Core_CSV_FileName.empty() && (!Batch_IsBackuping || Handler->second.Riff->IsModified_Get())) //If backuping, only if file is modified
        if (!Out_Core_CSV_File.Write(Core+EOL)) //Saving file part
        {
            StdErr("--out-Core-CSV=file: error during file writing");
            Out_Core_CSV_File.Close();
            Out_Core_CSV_FileName.clear();
        }

    if (Out_Core_XML || Out_Core_XML_Doc || Out_XML_Doc)
    {
        //Retrieving content
        ZtringList List_Content;
        List_Content.Separator_Set(0, ",");
        List_Content.Write(Core);

        //Preparing XML document
        if (Core.find(',')!=string::npos)
        {
            tinyxml2::XMLDocument Document;
            tinyxml2::XMLElement* Core=Document.NewElement("Core");
            for (size_t Header_Pos=1; Header_Pos<Out_Core_CSV_File_Header.size(); Header_Pos++)
            {
                if (Header_Pos<List_Content.size() && !List_Content[Header_Pos].empty())
                {
                    Out_Core_CSV_File_Header[Header_Pos].FindAndReplace(__T(" "), __T("_"), 0, Ztring_Recursive);
                    Out_Core_CSV_File_Header[Header_Pos].FindAndReplace(__T("("), __T(""), 0, Ztring_Recursive);
                    Out_Core_CSV_File_Header[Header_Pos].FindAndReplace(__T(")"), __T(""), 0, Ztring_Recursive);
                    Core->InsertNewChildElement(Out_Core_CSV_File_Header[Header_Pos].To_UTF8().c_str())->SetText(List_Content[Header_Pos].To_UTF8().c_str());
                }
            }

            if (Out_Core_XML_Doc)
            {
                tinyxml2::XMLElement* File=Out_Core_XML_Doc->NewElement("File");
                File->SetAttribute("name", List_Content[0].To_UTF8().c_str());

                tinyxml2::XMLNode* Content=Core->DeepClone(Out_Core_XML_Doc);
                File->InsertEndChild(Content);

                Out_Core_XML_Doc->RootElement()->InsertEndChild(File);
            }

            if (Out_Core_XML)
            {
                tinyxml2::XMLDocument Output_Document;
                Output_Document.InsertEndChild(Output_Document.NewDeclaration());
                Output_Document.InsertEndChild(Output_Document.NewElement("conformance_point_document"));

                tinyxml2::XMLElement* File=Output_Document.NewElement("File");
                File->SetAttribute("name", List_Content[0].To_UTF8().c_str());

                tinyxml2::XMLNode* Content=Core->DeepClone(&Output_Document);
                File->InsertEndChild(Content);

                Output_Document.RootElement()->InsertEndChild(File);

                if (Output_Document.SaveFile(Handler->second.Riff->FileName_Get().append(".Core.xml").c_str())!=tinyxml2::XML_SUCCESS)
                    StdErr("--out-Core-XML: error during file writing");
            }

            if (Out_XML_Doc)
            {
                tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
                if (File)
                {
                    tinyxml2::XMLNode* Content=Core->DeepClone(Out_XML_Doc);
                    File->InsertEndChild(Content);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_PMX(handlers::iterator &Handler)
{
    string Content=Handler->second.Riff->Get("xmp");

    if (!Content.empty())
    {
        //Saving file
        if (Out__PMX_XML)
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Handler->second.Riff->FileName_Get())+__T(".XMP.xml")))
                    throw "--out-XMP-XML: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-XMP-XML: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (!Out__PMX_FileName.empty())
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Out__PMX_FileName.c_str())))
                    throw "--out-XMP=: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-XMP=: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }


        if (Out_XML_Doc)
        {
            if (Prepare_Xml_For_Nesting(Content))
            {
                string Message="Invalid XMP xml for file: ";Message+=Handler->second.Riff->FileName_Get();
                StdErr(Message);
            }

            tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
            if (File)
            {
                tinyxml2::XMLElement* Parent=File->InsertNewChildElement("XMP");
                tinyxml2::XMLElement* Placeholder=Parent->InsertNewChildElement("PLACEHOLDER");
                tinyxml2::XMLText* Text=Placeholder->InsertNewText(Content.c_str());
                Text->SetCData(true);
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_aXML(handlers::iterator &Handler)
{
    string Content=Handler->second.Riff->Get("axml");
        
    if (!Content.empty())
    {
        //Saving file
        if (Out_aXML_XML)
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Handler->second.Riff->FileName_Get())+__T(".aXML.xml")))
                    throw "--out-aXML-XML: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-aXML-XML: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (!Out_aXML_FileName.empty())
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Out_aXML_FileName.c_str())))
                    throw "--out-aXML=: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-aXML=: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (Out_XML_Doc)
        {
            if (Prepare_Xml_For_Nesting(Content))
            {
                string Message="Invalid aXML xml for file: ";Message+=Handler->second.Riff->FileName_Get();
                StdErr(Message);
            }

            tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
            if (File)
            {
                tinyxml2::XMLElement* Parent=File->InsertNewChildElement("aXML");
                tinyxml2::XMLElement* Placeholder=Parent->InsertNewChildElement("PLACEHOLDER");
                tinyxml2::XMLText* Text=Placeholder->InsertNewText(Content.c_str());
                Text->SetCData(true);
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_iXML(handlers::iterator &Handler)
{
    string Content=Handler->second.Riff->Get("ixml");
    if (!Content.empty())
    {
        //Saving file
        if (Out_iXML_XML)
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Handler->second.Riff->FileName_Get())+__T(".iXML.xml")))
                    throw "--out-iXML-XML: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-iXML-XML: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (!Out_iXML_FileName.empty())
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Out_iXML_FileName.c_str())))
                    throw "--out-iXML=: error during file creation";
                if (!F.Write(Ztring().From_UTF8(Content)))
                    throw "--out-iXML=: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (Out_XML_Doc)
        {
            if (Prepare_Xml_For_Nesting(Content))
            {
                string Message="Invalid iXML xml for file: ";Message+=Handler->second.Riff->FileName_Get();
                StdErr(Message);
            }

            tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
            if (File)
            {
                tinyxml2::XMLElement* Parent=File->InsertNewChildElement("iXML");
                tinyxml2::XMLElement* Placeholder=Parent->InsertNewChildElement("PLACEHOLDER");
                tinyxml2::XMLText* Text=Placeholder->InsertNewText(Content.c_str());
                Text->SetCData(true);
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_cue_(handlers::iterator &Handler)
{
    Ztring Content=Ztring().From_UTF8(Handler->second.Riff->Get("cuexml"));

    if (!Content.empty())
    {
        if (Out_cue__XML)
        {
            //Saving file
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Handler->second.Riff->FileName_Get())+__T(".cue.xml")))
                    throw "--out-cue-XML: error during file creation";
                if (!F.Write(Content))
                    throw "--out-cue-XML: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (!Out_cue__FileName.empty())
        {
            try
            {
                FLACwrapper::File F;
                if (!F.Create(Ztring().From_UTF8(Out_cue__FileName.c_str())))
                    throw "--out-cue=: error during file creation";
                if (!F.Write(Content))
                    throw "--out-cue=: error during file writing";
            }
            catch (const char *Message)
            {
                StdErr(Message);
            }
            catch (...) {}
        }

        if (Out_XML_Doc)
        {
            tinyxml2::XMLDocument Cues;
            if (Cues.Parse(Content.To_UTF8().c_str())==tinyxml2::XML_SUCCESS && Cues.RootElement())
            {
                tinyxml2::XMLElement* File=Out_XML_Doc->RootElement()->LastChildElement();
                if (File)
                {
                    tinyxml2::XMLNode* Content=Cues.RootElement()->DeepClone(Out_XML_Doc);
                    File->InsertEndChild(Content);
                }
            }
            else
            {
                string Message="Invalid cues xml for file: ";Message+=Handler->second.Riff->FileName_Get();Message+=" ignored.";
                StdErr(Message);
            }
        }
    }
}

//---------------------------------------------------------------------------
void Core::Batch_Launch_Write(handlers::iterator &Handler)
{
    //Writing
    StdClear(Handler);
    bool WasModified=Handler->second.Riff->IsModified_Get();
    if (Handler->second.Riff->Save())
    {
        if (WasModified)
            Files_Modified_NotWritten_Count--;
    }
    StdAll(Handler);
}

//---------------------------------------------------------------------------
void Core::Options_Update(handlers::iterator &Handler)
{
    if (Handler->second.Riff)
    {
        //Options
        Handler->second.Riff->riff2rf64_Reject=riff2rf64_Reject;
        Handler->second.Riff->Overwrite_Reject=Overwrite_Reject;
        Handler->second.Riff->NoPadding_Accept=NoPadding_Accept;
        Handler->second.Riff->NewChunksAtTheEnd=NewChunksAtTheEnd;
        Handler->second.Riff->GenerateMD5=GenerateMD5;
        Handler->second.Riff->VerifyMD5=VerifyMD5;
        Handler->second.Riff->VerifyMD5_Force=VerifyMD5_Force;
        Handler->second.Riff->EmbedMD5=EmbedMD5;
        Handler->second.Riff->EmbedMD5_AuthorizeOverWritting=EmbedMD5_AuthorizeOverWritting;
        Handler->second.Riff->Bext_DefaultVersion=Bext_DefaultVersion;
        Handler->second.Riff->Bext_MaxVersion=Bext_MaxVersion;
        Handler->second.Riff->Trace_UseDec=Trace_UseDec;
        Handler->second.Riff->Encoding=Encoding;
        Handler->second.Riff->Fallback_Encoding=Fallback_Encoding;
        Handler->second.Riff->Write_Encoding=Write_Encoding;
        Handler->second.Riff->Write_CodePage=Write_CodePage;
        Handler->second.Riff->Ignore_File_Encoding=Ignore_File_Encoding;

        bool IsModified_Old=Handler->second.Riff->IsModified_Get();

        Handler->second.Riff->Options_Update();

        StdAll(Handler);

        bool IsModified_New=Handler->second.Riff->IsModified_Get();
        if (!IsModified_Old && IsModified_New)
            Files_Modified_NotWritten_Count++;
    }
}

//***************************************************************************
// Internal routines
//***************************************************************************

//---------------------------------------------------------------------------
void Core::StdClear(handlers::iterator &Handler)
{
    Handler->second.Riff->Information.str(string());
    Handler->second.Riff->Errors.str(string());
}

//---------------------------------------------------------------------------
void Core::StdAll(handlers::iterator &Handler)
{
    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);

    if (!Handler->second.Riff->Information.str().empty())
    {
        Text_stdout<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Information.str();
        Text_stdall<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Information.str();
        if (!LogFile.empty())
        {
            Ztring Data;
            Data+=TimeS;
            Data+=__T(" ");
            Data+=Ztring().From_UTF8(Handler->second.Riff->Information.str());
            FLACwrapper::File F(LogFile, File::Access_Write_Append);
            F.Write(Data);
        }
        Handler->second.Riff->Information.str(string());
    }
    if (!Handler->second.Riff->Warnings.str().empty())
    {
        Text_stdout<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Warnings.str();
        Text_stdall<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Warnings.str();
        if (!LogFile.empty())
        {
            Ztring Data;
            Data+=TimeS;
            Data+=__T(" ");
            Data+=Ztring().From_UTF8(Handler->second.Riff->Warnings.str());
            FLACwrapper::File F(LogFile, File::Access_Write_Append);
            F.Write(Data);
        }
        Handler->second.Riff->Warnings.str(string());
    }
    if (!Handler->second.Riff->Errors.str().empty())
    {
        Text_stderr<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Errors.str();
        Text_stdall<<TimeS.To_UTF8()<<" "<<Handler->second.Riff->Errors.str();
        if (!LogFile.empty())
        {
            Ztring Data;
            Data+=TimeS;
            Data+=__T(" ");
            Data+=Ztring().From_UTF8(Handler->second.Riff->Errors.str());
            FLACwrapper::File F(LogFile, File::Access_Write_Append);
            F.Write(Data);
        }
        Handler->second.Riff->Errors.str(string());
        Text_stderr_Updated=true;
    }
}

//---------------------------------------------------------------------------
void Core::StdOut(string Text)
{
    if (Text.empty())
        return;

    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);

    Text_stdout<<TimeS.To_UTF8()<<" "<<Text<<endl;
    Text_stdall<<TimeS.To_UTF8()<<" "<<Text<<endl;
    if (!LogFile.empty())
    {
        Ztring Data;
        Data+=TimeS;
        Data+=__T(" ");
        Data+=Ztring().From_UTF8(Text);
        Data+=EOL;
        FLACwrapper::File F(LogFile, File::Access_Write_Append);
        F.Write(Data);
    }
}

//---------------------------------------------------------------------------
void Core::StdErr(string Text)
{
    if (Text.empty())
        return;

    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970_Local((int32u)Time);

    Text_stderr<<TimeS.To_UTF8()<<" "<<Text<<endl;
    Text_stdall<<TimeS.To_UTF8()<<" "<<Text<<endl;
    Text_stderr_Updated=true;
}

//---------------------------------------------------------------------------
bool Core::Text_stderr_Updated_Get()
{
    bool Temp=Text_stderr_Updated;
    Text_stderr_Updated=false;
    
    return Temp;
}
