// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Handler.h"
#include "Riff/Riff_Chunks.h"
#include <sstream>
#include <iostream>
#include "ZenLib/ZtringListList.h"
#include "ZenLib/File.h"
#include "ZenLib/Dir.h"
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const Ztring Riff_Handler_EmptyZtring_Const; //Use it when we can't return a reference to a true Ztring, const version
//---------------------------------------------------------------------------

//***************************************************************************
// Const
//***************************************************************************

enum xxxx_Fields
{
    Fields_Tech,
    Fields_Bext,
    Fields_Info,
    Fields_Max,
};

size_t xxxx_Strings_Size[]=
{
    4,  //Tech
    15,  //Bext
    17, //Info
};

const char* xxxx_Strings[][17]=
{
    {
        "XMP",
        "aXML",
        "iXML",
        "MD5Stored",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
    },
    {
        "Description",
        "Originator",
        "OriginatorReference",
        "OriginationDate",
        "OriginationTime",
        "TimeReference (translated)",
        "TimeReference",
        "BextVersion",
        "UMID",
        "LoudnessValue",
        "LoudnessRange",
        "MaxTruePeakLevel",
        "MaxMomentaryLoudness",
        "MaxShortTermLoudness",
        "CodingHistory",
        "",
        "",
    },
    {
        //Note: there is a duplicate in Riff_Chunks_INFO_xxxx
        "IARL", //Archival Location
        "IART", //Artist
        "ICMS", //Commissioned
        "ICMT", //Comment
        "ICOP", //Copyright
        "ICRD", //Date Created
        "IENG", //Engineer
        "IGNR", //Genre
        "IKEY", //Keywords
        "IMED", //Medium
        "INAM", //Title
        "IPRD", //Product
        "ISBJ", //Subject
        "ISFT", //Software
        "ISRC", //Source
        "ISRF", //Source Form
        "ITCH", //Technician
    },
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Riff_Handler::Riff_Handler ()
{
    //Configuration
    riff2rf64_Reject=false;
    Overwrite_Reject=false;
    NoPadding_Accept=false;
    NewChunksAtTheEnd=false;
    GenerateMD5=false;
    VerifyMD5=false;
    EmbedMD5=false;
    EmbedMD5_AuthorizeOverWritting=false;
    Bext_DefaultVersion=0;
    Bext_MaxVersion=2;

    //Internal
    Chunks=NULL;
    File_IsValid=false;
    File_IsCanceled=false;
}

//---------------------------------------------------------------------------
Riff_Handler::~Riff_Handler ()
{
    delete Chunks; //Chunks=NULL;
}

//***************************************************************************
// I/O
//***************************************************************************

//---------------------------------------------------------------------------
bool Riff_Handler::Open(const string &FileName)
{
    //Init
    PerFile_Error.str(string());
    File_IsValid=false;
    File_IsCanceled=false;
    bool ReturnValue=true;
    
    //Global info
    delete Chunks; Chunks=new Riff();
    Chunks->Global->File_Name=FileName;

    //Opening file
    if (!File::Exists(FileName) || !Chunks->Global->In.Open(FileName))
    {
        Errors<<FileName<<": File does not exist"<<endl;
        PerFile_Error<<"File does not exist"<<endl;
        return false;
    }
    Chunks->Global->File_Size=Chunks->Global->In.Size_Get();
    Chunks->Global->File_Date=Chunks->Global->In.Created_Local_Get();
    if (Chunks->Global->File_Date.empty())
        Chunks->Global->File_Date=Chunks->Global->In.Modified_Local_Get();

    //Base
    Riff_Base::chunk Chunk;
    Chunk.Content.Size=Chunks->Global->File_Size;
    Options_Update();

    //Parsing
    try
    {
        Chunks->Read(Chunk);
        File_IsValid=true;
    }
    catch (exception_canceled &)
    {
        CriticalSectionLocker(Chunks->Global->CS);
        File_IsCanceled=true;
        Chunks->Global->Canceling=false;
        ReturnValue=false;
    }
    catch (exception_read_chunk &e)
    {
        Errors<<Chunks->Global->File_Name<<": "<<Ztring().From_CC4(Chunk.Header.Name)<<" "<<e.what()<<endl;
        PerFile_Error<<Ztring().From_CC4(Chunk.Header.Name)<<" "<<e.what()<<endl;
    }
    catch (exception &e)
    {
        Errors<<Chunks->Global->File_Name<<": "<<e.what()<<endl;
        PerFile_Error<<e.what()<<endl;
        ReturnValue=false;
    }

    //Cleanup
    Chunks->Global->In.Close();

    if (File_IsValid)
    {
        //Log
        if (Chunks->Global->NoPadding_IsCorrected)
        {
            Information<<Chunks->Global->File_Name<<": no-padding correction"<<endl;
            PerFile_Information<<"no-padding correction"<<endl;
        }

        //Saving initial values
        Core_FromFile=Core_Get();

        //MD5
        if (Chunks->Global->VerifyMD5)
        {
            //Removing all MD5 related info
            Ztring PerFile_Error_Temp=PerFile_Error.str();
            PerFile_Error_Temp.FindAndReplace(Ztring("MD5, failed verification\n"), Ztring());
            PerFile_Error.str(PerFile_Error_Temp);
            Ztring PerFile_Information_Temp=PerFile_Information.str();
            PerFile_Information_Temp.FindAndReplace(Ztring("MD5, no existing MD5 chunk\n"), Ztring());
            PerFile_Information_Temp.FindAndReplace(Ztring("MD5, verified\n"), Ztring());
            PerFile_Information.str(PerFile_Information_Temp);
            
            //Checking
            if (!(Chunks->Global->MD5Stored && !Chunks->Global->MD5Stored->Strings["md5stored"].empty()))
            {
                Information<<Chunks->Global->File_Name<<": MD5, no existing MD5 chunk"<<endl;
                PerFile_Information<<"MD5, no existing MD5 chunk"<<endl;
            }
            else if (Chunks->Global->MD5Generated && Chunks->Global->MD5Generated->Strings["md5generated"]!=Chunks->Global->MD5Stored->Strings["md5stored"])
            {
                Errors<<Chunks->Global->File_Name<<": MD5, failed verification"<<endl;
                PerFile_Error.str(string());
                PerFile_Error<<"MD5, failed verification"<<endl;
            }
            else
            {
                Information<<Chunks->Global->File_Name<<": MD5, verified"<<endl;
                PerFile_Information.str(string());
                PerFile_Information<<"MD5, verified"<<endl;
            }
            }
        if (EmbedMD5
         && Chunks->Global->MD5Generated && !Chunks->Global->MD5Generated->Strings["md5generated"].empty()
         && (!(Chunks->Global->MD5Stored && !Chunks->Global->MD5Stored->Strings["md5stored"].empty())
          || EmbedMD5_AuthorizeOverWritting))
                Set("MD5Stored", Chunks->Global->MD5Generated->Strings["md5generated"], rules());
    }

    CriticalSectionLocker(Chunks->Global->CS);
    Chunks->Global->Progress=1;
    
    return ReturnValue;
}

//---------------------------------------------------------------------------
bool Riff_Handler::Save()
{
    Chunks->Global->CS.Enter();
    Chunks->Global->Progress=(float)0.05;
    Chunks->Global->CS.Leave();
    
    //Init
    PerFile_Error.str(string());

    //Integrity
    if (Chunks==NULL)
    {
        Errors<<"(No file name): Internal error"<<endl;
        return false;
    }

    //Write only if modified
    if (!IsModified_Get())
    {
        Information<<Chunks->Global->File_Name<<": Nothing to do"<<endl;
        return false;
    }

    //Modifying the chunks in memory
    for (size_t Fields_Pos=0; Fields_Pos<Fields_Max; Fields_Pos++)
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
        {
            if (!IsOriginal(xxxx_Strings[Fields_Pos][Pos], Get(xxxx_Strings[Fields_Pos][Pos])))
                Chunks->Modify(Elements::WAVE, Chunk_Name2_Get(xxxx_Strings[Fields_Pos][Pos]), Chunk_Name3_Get(xxxx_Strings[Fields_Pos][Pos]));
        }

    //File size management
    if (riff2rf64_Reject && Chunks && Chunks->Global->ds64==NULL && Chunks->Block_Size_Get()>RIFF_Size_Limit)
    {
        Errors<<Chunks->Global->File_Name<<": File size would be too big (and --reject-riff2rf64 option)"<<endl;
        PerFile_Error<<"File size would be too big (and --reject-riff2rf64 option)"<<endl;
        return false;
    }

    //Opening files
    if (!Chunks->Global->In.Open(Chunks->Global->File_Name))
    {
        Errors<<Chunks->Global->File_Name<<": File does not exist anymore"<<endl;
        PerFile_Error<<"File does not exist anymore"<<endl;
        return false;
    }

    //Old temporary file
    if (File::Exists(Chunks->Global->File_Name+".tmp") && !File::Delete(Chunks->Global->File_Name+".tmp"))
    {
        Errors<<Chunks->Global->File_Name<<": Old temporary file can't be deleted"<<endl;
        PerFile_Error<<"Old temporary file can't be deleted"<<endl;
        return false;
    }
    
    //Parsing
    try
    {
        Chunks->Write();
    }
    catch (exception_canceled &)
    {
        Chunks->Global->Out.Close();
        File::Delete(Chunks->Global->File_Name+".tmp");
        CriticalSectionLocker(Chunks->Global->CS);
        File_IsCanceled=true;
        Chunks->Global->Canceling=false;
        return false;
    }
    catch (exception &e)
    {
        Errors<<Chunks->Global->File_Name<<": "<<e.what()<<endl;
        return false;
    }

    //Log
    Information<<(Chunks?Chunks->Global->File_Name:"")<<": Is modified"<<endl;

    //Loading the new file (we are verifying the integraty of the generated file)
    string FileName=Chunks->Global->File_Name;
    bool GenerateMD5_Temp=Chunks->Global->GenerateMD5;
    Chunks->Global->GenerateMD5=false;
    if (!Open(FileName) && Chunks==NULL) //There may be an error but file is open (eg MD5 error)
    {
        Errors<<FileName<<": WARNING, the resulting file can not be validated, file may be CORRUPTED"<<endl;
        PerFile_Error<<"WARNING, the resulting file can not be validated, file may be CORRUPTED"<<endl;
        Chunks->Global->GenerateMD5=GenerateMD5_Temp;
        return false;
    }
    Chunks->Global->GenerateMD5=GenerateMD5_Temp;

    CriticalSectionLocker(Chunks->Global->CS);
    Chunks->Global->Progress=1;
    
    return true;
}

//---------------------------------------------------------------------------
bool Riff_Handler::BackToLastSave()
{
    for (size_t Fields_Pos=0; Fields_Pos<Fields_Max; Fields_Pos++)
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
        {
            if (!IsOriginal(xxxx_Strings[Fields_Pos][Pos], Get(xxxx_Strings[Fields_Pos][Pos])))
            {
                ZtringList HistoryList; HistoryList.Write(History(xxxx_Strings[Fields_Pos][Pos]));
                if (!HistoryList.empty())
                    Set(xxxx_Strings[Fields_Pos][Pos], HistoryList[0], rules());
            }
        }

    Chunks->IsModified_Clear();
        
    return true;
}

//***************************************************************************
// Per Item
//***************************************************************************

//---------------------------------------------------------------------------
string Riff_Handler::Get(const string &Field)
{
    return Get(Field_Get(Field), *chunk_strings_Get(Field)); 
}

//---------------------------------------------------------------------------
bool Riff_Handler::Set(const string &Field_, const string &Value_, rules Rules)
{
    //Integrity
    if (Chunks==NULL)
    {
        Errors<<"(No file name): Internal error"<<endl;
        return false;
    }

    string Field=Field_Get(Field_);
    
    //Testing if useful
    if (Field=="filename"
     || Field=="version"
     || Field=="errors"
     || Field=="information"
     || Value_=="NOCHANGE")
        return true;
    
    Ztring Value=Value_; 
    Value.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
    Value.FindAndReplace("\n\r", "\n", 0, Ztring_Recursive); //Bug in v0.2.1 XML, \r\n was inverted
    Value.FindAndReplace("\r", "\n", 0, Ztring_Recursive);
    Value.FindAndReplace("\n", "\r\n", 0, Ztring_Recursive);

    if (Value.size()>7
     && Value[0]=='f'
     && Value[1]=='i'
     && Value[2]=='l'
     && Value[3]=='e'
     && Value[4]==':'
     && Value[5]=='/'
     && Value[6]=='/')
    {
        if (!Value.Assign_FromFile(Value.substr(7, string::npos)))
        {
            Errors<<Chunks->Global->File_Name<<": Malformed input ("<<Field<<"="<<Value<<", File does not exist)"<<endl;
            return false;
        }
    }

    //Legacy
    if (Field=="timereference" && !(Value.size()<12
          ||  Value[Value.size()-12]< '0' || Value[Value.size()-12]> '9' 
          ||  Value[Value.size()-11]< '0' || Value[Value.size()-11]> '9' 
          || (Value[Value.size()-10]!='-' && Value[Value.size()-10]!='_' && Value[Value.size()-10]!=':' && Value[Value.size()-10]!=' ' && Value[Value.size()-10]!='.') 
          ||  Value[Value.size()- 9]< '0' || Value[Value.size()- 9]> '5' 
          ||  Value[Value.size()- 8]< '0' || Value[Value.size()- 8]> '9' 
          || (Value[Value.size()- 7]!='-' && Value[Value.size()- 7]!='_' && Value[Value.size()- 7]!=':' && Value[Value.size()- 7]!=' ' && Value[Value.size()- 7]!='.') 
          ||  Value[Value.size()- 6]< '0' || Value[Value.size()- 6]> '5' 
          ||  Value[Value.size()- 5]< '0' || Value[Value.size()- 5]> '9' 
          || (Value[Value.size()- 4]!='-' && Value[Value.size()- 4]!='_' && Value[Value.size()- 4]!=':' && Value[Value.size()- 4]!=' ' && Value[Value.size()- 4]!='.') 
          ||  Value[Value.size()- 3]< '0' || Value[Value.size()- 3]> '9' 
          ||  Value[Value.size()- 2]< '0' || Value[Value.size()- 2]> '9' 
          ||  Value[Value.size()- 1]< '0' || Value[Value.size()- 1]> '9')) 
        Field="timereference (translated)";

    //Testing validity
    if (!IsValid(Field, Value, Rules)
     && !IsOriginal(Field, Value))
        return false;

    //Special cases - before
    if (Field=="timereference (translated)")
    {
        if (Value.empty())
            return Set("timereference", Ztring(), Rules);    
        else if (Value.size()>=12 && Chunks && Chunks->Global && Chunks->Global->fmt_ && Chunks->Global->fmt_->sampleRate)
        {
            int64u HH=Ztring(Value.substr(0, Value.size()-10)).To_int64u(); 
            int64u MM=Ztring(Value.substr(Value.size()-9, 2 )).To_int64u(); 
            int64u SS=Ztring(Value.substr(Value.size()-6, 2 )).To_int64u(); 
            int64u MS=Ztring(Value.substr(Value.size()-3, 3 )).To_int64u();
            int64u TimeReference;
            TimeReference=HH*60*60*1000
                        + MM*   60*1000
                        + SS*      1000
                        + MS;
            TimeReference=(int64u)(((float64)TimeReference)/1000*Chunks->Global->fmt_->sampleRate);
            
            if (Value!=Get("timereference (translated)"))
                return Set("timereference", Ztring().From_Number(TimeReference), Rules);
            else
                return true;
        }
    }
    if (Field=="timereference" && Value=="0")
        Value.clear();
    if (Field=="bext" && (Value=="0" || Value=="1"))
        return Set("bextversion", Value, Rules);

    //Setting it
    bool ToReturn=Set(Field, Value, *chunk_strings_Get(Field), Chunk_Name2_Get(Field), Chunk_Name3_Get(Field)); 
    
    //Special cases - After
    if (ToReturn && Field=="originator")
    {
        if (Rules.FADGI_Rec && Chunks && Chunks->Global && Chunks->Global->INFO)
            Set("IARL", Value, Rules); //If INFO is present, IARL is filled with the same value
    }

    return ToReturn;
}

//---------------------------------------------------------------------------
bool Riff_Handler::Remove(const string &Field)
{
    //Integrity
    if (Chunks==NULL)
    {
        Errors<<"(No file name): Internal error"<<endl;
        return false;
    }

    //bext + INFO
    if (Ztring(Field).MakeLowerCase()=="core")
        for (size_t Fields_Pos=Fields_Bext; Fields_Pos<=Fields_Info; Fields_Pos++) //Only Bext and Info
            for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
            {
                bool ToReturn=true;
                if (!Set(Field_Get(xxxx_Strings[Fields_Pos][Pos]), string(), *chunk_strings_Get(xxxx_Strings[Fields_Pos][Pos]), Chunk_Name2_Get(xxxx_Strings[Fields_Pos][Pos]), Chunk_Name3_Get(xxxx_Strings[Fields_Pos][Pos])))
                    ToReturn=false;
                return ToReturn;
            }

    return Set(Field, string(), *chunk_strings_Get(Field), Chunk_Name2_Get(Field), Chunk_Name3_Get(Field)); 
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsModified(const string &Field)
{
    //Special cases
    if (Field_Get(Field)=="bext")
    {
        bool ToReturn=false;
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Bext]; Pos++)
             if (IsModified(xxxx_Strings[Fields_Bext][Pos]))
                 ToReturn=true;
        if (!ToReturn && IsModified("bextversion"))
             ToReturn=true;
        return ToReturn;
    }
    if (Field_Get(Field)=="INFO")
    {
        bool ToReturn=false;
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Info]; Pos++)
             if (IsModified(xxxx_Strings[Fields_Info][Pos]))
                 ToReturn=true;
        return ToReturn;
    }
    
    return IsModified(Field_Get(Field=="timereference (translated)"?Ztring("timereference"):Field), *chunk_strings_Get(Field)); 
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsValid(const string &Field_, const string &Value_, rules Rules)
{
    //Reformating
    IsValid_Errors.str(string());
    Ztring Field=Field_Get(Field_);
    Ztring Value=Value_;
    Value.FindAndReplace("\r\n", "\n", 0, Ztring_Recursive);
    Value.FindAndReplace("\n\r", "\n", 0, Ztring_Recursive); //Bug in v0.2.1 XML, \r\n was inverted
    Value.FindAndReplace("\r", "\n", 0, Ztring_Recursive);
    Value.FindAndReplace("\n", "\r\n", 0, Ztring_Recursive);

    //Rules
    if (Rules.FADGI_Rec)
        Rules.Tech3285_Rec=true;
    if (Rules.Tech3285_Rec)
        Rules.Tech3285_Req=true;
    if (Rules.INFO_Rec)
        Rules.INFO_Req=true;

    //FileName
    if (Field=="filename")
    {
        //Test
        string Message;
        if (!PerFile_Error.str().empty())
            Message=PerFile_Error.str();

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed file, "<<Message;
    }

    //MD5Stored
    if (Field=="md5stored")
    {
        //Test
        string Message;
        for (size_t Pos=0; Pos<Value.size(); Pos++)
            if (!((Value[Pos]>='0' && Value[Pos]<='9') || (Value[Pos]>='a' && Value[Pos]<='f') || (Value[Pos]>='A' && Value[Pos]<='F')))
                Message="allowed characters are 0-9, a-z, A-Z";
        if (Message.empty())
        {
            if (!Value.empty() && Value.size()!=32)
                Message="must be 16 byte long hexadecimal coded text (32 byte long) value";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, MD5Stored "<<Message;
    }

    //MD5Stored
    else if (Field=="md5generated")
    {
        //Test
        string Message;
        for (size_t Pos=0; Pos<Value.size(); Pos++)
            if (!((Value[Pos]>='0' && Value[Pos]<='9') || (Value[Pos]>='A' && Value[Pos]<='F') || (Value[Pos]>='A' && Value[Pos]<='F')))
                Message="allowed characters are 0-9, a-z, A-Z";
        if (Message.empty())
        {
            if (!Value.empty() && Value.size()!=32)
                Message="must be 16 byte long hexadecimal coded text (32 byte long) value";
            else if (!Value.empty() && !Get("MD5Stored").empty() && Value!=Get("MD5Stored"))
                Message="Failed verification";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, MD5Generated "<<Message;
    }

    //Description
    else if (Field=="description")
    {
        //Test
        string Message;
        if (Value.size()>256) //Limitation from the format
            Message="length is maximum 256 characters (format limitation)";
        if (Rules.FADGI_Rec && Value.empty())
            Message="must not be empty (FADGI recommandations)";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, Description "<<Message;
    }

    //Originator
    else if (Field=="originator")
    {
        //Test
        string Message;
        if (Value.size()>32) //Limitation from the format
            Message="length is maximum 32 characters (format limitation)";
        else if (Rules.FADGI_Rec)
        {
            if (Value.empty())
                Message="must not be empty (FADGI recommandations)";
            else if (Value.size()>0 && (Value[0] < 'A' || Value[0] > 'Z'))
                Message="1st character must be between 'A' and 'Z' (FADGI recommandations)";
            else if (Value.size()>1 && (Value[1] < 'A' || Value[1] > 'Z'))
                Message="2nd character must be between 'A' and 'Z' (FADGI recommandations)";
            else if (Value.size()>2 && Value[2] !=',')
                Message="3rd character must be a comma (FADGI recommandations)";
            else if (Value.size()>3 && Value[3] !=' ')
                Message="4th character must be a space (FADGI recommandations)";
            else if (Value.size()<5)
                Message="length is minimum 5 characters (FADGI recommandations)";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, Originator "<<Message;
    }

    //OriginatorReference
    else if (Field=="originatorreference")
    {
        //Test
        string Message;
        if (Value.size()>32) //Limitation from the format
            Message="length is maximum 32 characters (format limitation)";
        else if (Rules.FADGI_Rec && Value.empty())
            Message="must not be empty (FADGI recommandations)";
        else if (Rules.OriginatorReference_Rec)
        {
                 if (Value[0] < 'A' || Value[0] > 'Z')  //Country code
                Message="1st character (Country code) must be between 'A' and 'Z' (BWF OriginatorReference recommandations)";
            else if (Value[1] < 'A' || Value[1] > 'Z')  //Country code
                Message="2nd character (Country code) must be between 'A' and 'Z' (BWF OriginatorReference recommandations)";
            else if (Value[2] < '0' || (Value[2] > '9' && Value[2] < 'A') || Value[2] > 'Z')  //Organisation code
                Message="3rd character (Organisation code) must be between '0' and '9' or 'A' and 'Z' (BWF OriginatorReference recommandations)";
            else if (Value[3] < '0' || (Value[3] > '9' && Value[3] < 'A') || Value[3] > 'Z')  //Organisation code
                Message="4th character (Organisation code) must be between '0' and '9' or 'A' and 'Z' (BWF OriginatorReference recommandations)";
            else if (Value[4] < '0' || (Value[4] > '9' && Value[4] < 'A') || Value[4] > 'Z')  //Organisation code
                Message="5th character (Organisation code) must be between '0' and '9' or 'A' and 'Z' (BWF OriginatorReference recommandations)";
            else if (Value[17]< '0' || Value[17]> '2')  //OriginationTime
                Message="18th and 19th character (OriginationTime) must be between '00' and '23' (BWF OriginatorReference recommandations)";
            else if (Value[18]< '0' || (Value[18]> (Value[16]=='2'?'3':'9'))) //Only 00-23 //OriginationTime
                Message="18th and 19th character (OriginationTime) must be between '00' and '23' (BWF OriginatorReference recommandations)";
            else if (Value[19]< '0' || Value[19]> '5')  //OriginationTime 
                Message="20th and 21st character (OriginationTime) must be between '00' and '59' (BWF OriginatorReference recommandations)";
            else if (Value[20]< '0' || Value[20]> '9')  //OriginationTime 
                Message="20th and 21st character (OriginationTime) must be between '00' and '59' (BWF OriginatorReference recommandations)";
            else if (Value[21]< '0' || Value[21]> '9')  //OriginationTime 
                Message="22nd and 23rd character (OriginationTime) must be between '00' and '59' (BWF OriginatorReference recommandations)";
            else if (Value[22]< '0' || Value[22]> '9')  //OriginationTime 
                Message="22nd and 23rd character (OriginationTime) must be between '00' and '59' (BWF OriginatorReference recommandations)";
            else if (Value[23]< '0' || Value[23]> '9')  //Random Number 
                Message="24th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[24]< '0' || Value[24]> '9')  //Random Number 
                Message="25th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[25]< '0' || Value[25]> '9')  //Random Number 
                Message="26th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[26]< '0' || Value[26]> '9')  //Random Number 
                Message="27th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[27]< '0' || Value[27]> '9')  //Random Number 
                Message="28th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[28]< '0' || Value[28]> '9')  //Random Number 
                Message="29th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[29]< '0' || Value[29]> '9')  //Random Number 
                Message="30th character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[30]< '0' || Value[30]> '9')  //Random Number 
                Message="31st character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
            else if (Value[31]< '0' || Value[31]> '9')  //Random Number 
                Message="32nd character (Random Number) must be between '0' and '9' (BWF OriginatorReference recommandations)";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, OriginatorReference "<<Message;
    }

    //OriginationDate
    else if (Field=="originationdate")
    {
        //Test
        string Message;
        if (Value.size()>10)
            Message="length is maximum 10 characters (format limitation)";
        else if (Rules.FADGI_Rec)
        {
            if (Value.empty())
                Message="must not be empty (FADGI recommandations)";
            else if (Value.size()!=4 && Value.size()!=7 && Value.size()!=10)
                Message="must be YYYY, YYYY-MM, or YYYY-MM-DD (FADGI recommandations)";
			else
            {
                     if (Value[0]< '0' || Value[0]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
                else if (Value[1]< '0' || Value[1]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
                else if (Value[2]< '0' || Value[2]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
                else if (Value[3]< '0' || Value[3]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
                else if (Value.size()>=7) //YYYY-DD
                {
                         if (Value[4]!='-') //Hyphen
                        Message="5th character must be '-' (FADGI recommandations)";
                    else if (Value[5]< '0' || Value[5]> '1') //Month
                        Message="6th and 7th characters (Month) must be between '01' and '12' (BWF requirements)";
                    else if ((Value[6]< (Value[5]=='0'?'1':'0')) || (Value[6]> (Value[5]=='1'?'2':'9'))) //Only 01-12 //Month
                        Message="6th and 7th characters (Month) must be between '01' and '12' (BWF requirements)";
                    else if (Value.size()==10) //YYYY-DD-MM
                    {
                             if (Value[7]!='-') //Hyphen
                            Message="8th character must be '-' (FADGI recommandations)";
                        else if (Value[8]< '0' || Value[8]> '3') //Day
                            Message="9th and 10th characters (Day) must be between '01' and '23' (BWF requirements)";
                        else if ((Value[9]< (Value[8]=='0'?'1':'0')) || (Value[9]> (Value[8]=='3'?'1':'9'))) //Only 01-31 //Day
                            Message="9th and 10th characters (Day) must be between '01' and '23' (BWF requirements)";
                    }
                }
            }
        }
        else if (Rules.Tech3285_Req && !Value.empty())
        {
                 if (Value.size()!=10)
                Message="must be YYYY-MM-DD (BWF requirements)";
            else if (Value[0]< '0' || Value[0]> '9') //Year
                Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
            else if (Value[1]< '0' || Value[1]> '9') //Year
                Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
            else if (Value[2]< '0' || Value[2]> '9') //Year
                Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
            else if (Value[3]< '0' || Value[3]> '9') //Year
                Message="1st to 4th characters (Year) must be between '0000' and '9999' (BWF requirements)";
            else if (Rules.Tech3285_Rec && Value[4]!='-' && Value[4]!='_' && Value[4]!=':' && Value[4]!=' ' && Value[4]!='.') //Hyphen 
                Message="5th character must be '-', '_', ':', ' ', or '.' (BWF recommandation)";
            else if (Value[5]< '0' || Value[5]> '1') //Month
                Message="6th and 7th characters (Month) must be between '01' and '12' (BWF requirements)";
            else if ((Value[6]< (Value[5]=='0'?'1':'0')) || (Value[6]> (Value[5]=='1'?'2':'9'))) //Only 01-12 //Month
                Message="6th and 7th characters (Month) must be between '01' and '12' (BWF requirements)";
            else if (Rules.Tech3285_Rec && Value[7]!='-' && Value[7]!='_' && Value[7]!=':' && Value[7]!=' ' && Value[7]!='.') //Hyphen 
                Message="8th character must be '-', '_', ':', ' ', or '.' (BWF recommendations)";
            else if (Value[8]< '0' || Value[8]> '3') //Day
                Message="9th and 10th characters (Day) must be between '01' and '23' (BWF requirements)";
            else if ((Value[9]< (Value[8]=='0'?'1':'0')) || (Value[9]> (Value[8]=='3'?'1':'9'))) //Only 01-31 //Day
                Message="9th and 10th characters (Day) must be between '01' and '23' (BWF requirements)";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, OriginationDate "<<Message;
    }

    else if (Field=="originationtime")
    {
        //Test
        string Message;
        if (Value.size()>8)
            Message="length is maximum 8 characters (format limitation)";
		else if (Rules.FADGI_Rec && !Value.empty())
        {
        //    if (Value.empty())
        //        Message="must not be empty (FADGI recommandations)";
            if (Value.size()!=2 && Value.size()!=5 && Value.size()!=8)
                Message="must be HH, HH:MM, or HH:MM:SS (FADGI recommandations)";
            else
            {
                     if (Value[0]< '0' || Value[0]> '2') //Hours
                    Message="1st and 2nd characters (Hours) must be between '00' and '23' (FADGI recommandations)";
                else if (Value[1]< '0' || (Value[1]> (Value[0]=='2'?'3':'9'))) //Only 00-23 //Hours
                    Message="1st and 2nd characters (Hours) must be between '00' and '23' (FADGI recommandations)";
                else if (Value.size()>=5) //HH:MM
                {
                         if (Value[2]!=':') //Separator
                        Message="3rd character must be ':' (FADGI recommandations)";
                    else if (Value[3]< '0' || Value[3]> '5') //Minutes
                        Message="4th and 5th characters (Minutes) must be between '00' and '59' (FADGI recommandations)";
                    else if (Value[4]< '0' || Value[4]> '9' ) //Minutes
                        Message="4th and 5th characters (Minutes) must be between '00' and '59' (FADGI recommandations)";
                    else if (Value.size()==8) //HH:MM:SS
                    {
                             if (Value[5]!=':') //Separator
                            Message="6th character must be ':' (FADGI recommandations)";
                        else if (Value[6]< '0' || Value[6]> '5') //Seconds
                            Message="7th and 8th characters (Seconds) must be between '00' and '59' (FADGI recommandations)";
                        else if (Value[7]< '0' || Value[7]> '9' ) //Seconds
                            Message="7th and 8th characters (Seconds) must be between '00' and '59' (FADGI recommandations)";
                    }
                }
            }
        }
        else if (Rules.Tech3285_Req && !Value.empty())
        {
                 if (Value.size()!=8)
                     Message="must be HH:MM:SS";
            else if (Value[0]< '0' || Value[0]> '2') //Hours
                Message="1st and 2nd characters (Hours) must be between '00' and '23'";
            else if (Value[1]< '0' || (Value[1]> (Value[0]=='2'?'3':'9'))) //Only 00-23 //Hours
                Message="1st and 2nd characters (Hours) must be between '00' and '23'";
            else if (Rules.Tech3285_Rec && Value[2]!='-' && Value[2]!='_' && Value[2]!=':' && Value[2]!=' ' && Value[2]!='.') //Separator 
                Message="3rd character must be '-', '_', ':', ' ', or '.' (BWF recommendations)";
            else if (Value[3]< '0' || Value[3]> '5') //Minutes
                Message="4th and 5th characters (Minutes) must be between '00' and '59'";
            else if (Value[4]< '0' || Value[4]> '9' ) //Minutes
                Message="4th and 5th characters (Minutes) must be between '00' and '59'";
            else if (Rules.Tech3285_Rec && Value[5]!='-' && Value[5]!='_' && Value[5]!=':' && Value[5]!=' ' && Value[5]!='.') //Separator 
                Message="6th character must be '-', '_', ':', ' ', or '.' (BWF recommendations)";
            else if (Value[6]< '0' || Value[6]> '5') //Seconds
                Message="7th and 8th characters (Seconds) must be between '00' and '59'";
            else if (Value[7]< '0' || Value[7]> '9' ) //Seconds
                Message="7th and 8th characters (Seconds) must be between '00' and '59'";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, OriginationTime "<<Message;
    }

    else if (Field=="timereference (translated)")
    {
        //Test
        string Message;
        if (Value.empty())
            {}
        else if (Value.size()<12
          ||  Value[Value.size()-12]< '0' || Value[Value.size()-12]> '9' 
          ||  Value[Value.size()-11]< '0' || Value[Value.size()-11]> '9' 
          || (Value[Value.size()-10]!='-' && Value[Value.size()-10]!='_' && Value[Value.size()-10]!=':' && Value[Value.size()-10]!=' ' && Value[Value.size()-10]!='.') 
          ||  Value[Value.size()- 9]< '0' || Value[Value.size()- 9]> '5' 
          ||  Value[Value.size()- 8]< '0' || Value[Value.size()- 8]> '9' 
          || (Value[Value.size()- 7]!='-' && Value[Value.size()- 7]!='_' && Value[Value.size()- 7]!=':' && Value[Value.size()- 7]!=' ' && Value[Value.size()- 7]!='.') 
          ||  Value[Value.size()- 6]< '0' || Value[Value.size()- 6]> '5' 
          ||  Value[Value.size()- 5]< '0' || Value[Value.size()- 5]> '9' 
          || (Value[Value.size()- 4]!='-' && Value[Value.size()- 4]!='_' && Value[Value.size()- 4]!=':' && Value[Value.size()- 4]!=' ' && Value[Value.size()- 4]!='.') 
          ||  Value[Value.size()- 3]< '0' || Value[Value.size()- 3]> '9' 
          ||  Value[Value.size()- 2]< '0' || Value[Value.size()- 2]> '9' 
          ||  Value[Value.size()- 1]< '0' || Value[Value.size()- 1]> '9')
                Message="format must be HH:MM:SS.mmm";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, TimeReference (translated) "<<Message;
    }

    else if (Field=="timereference")
    {
        //Test
        string Message;
        if (Value.size()>18)
            Message="must be a number less than 18 digits";
        else
            for (size_t Pos=0; Pos<Value.size(); Pos++)
                if (Value[Pos]<'0' || Value[Pos]>'9')
                    Message="must be a number";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, TimeReference "<<Message;
    }

    else if (Field=="BextVersion")
    {
        //Test
        string Message;
        if (!Value.empty() && Value!="0" && Value!="1" && Value!="2")
            Message="must be empty, 0, 1 or 2";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, BextVersion "<<Message;
    }

    else if (Field=="umid")
    {
        //Test
        string Message;
        /* Old method disabled
        if (Value.find('-')!=string::npos)
        {
            //Old method
            if (!Value.empty() && Ztring(Value).To_UUID()==0 && Value!="00000000-0000-0000-0000-000000000000")
                Message="must be XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX and it must contain only 0-9 and A-F (legacy method)";
        }
        else */
        {
            if (!Value.empty() && Value.size()!=64 && Value.size()!=128)
                Message="must be 0-, 32- or 64-byte long";
            else
            {
                for (size_t Pos=0; Pos<Value.size(); Pos++)
                    if (!((Value[Pos]>='0' && Value[Pos]<='9') || (Value[Pos]>='a' && Value[Pos]<='f') || (Value[Pos]>='A' && Value[Pos]<='F')))
                    {
                        Message="must contain only 0-9 and A-F (hexadecimal)";
                        break;
                    }
                if (Message.empty()) 
                {
                    
                }
            }
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, UMID "<<Message;
    }

    else if (Field=="bext_version")
    {
        //Test
        string Message;
        if (!Value.empty() && Value!="0" && Value!="1" && Value!="2")
            Message="must be empty, 0, 1 or 2";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, bext version "<<Message;
    }

    else if (Field=="loudnessvalue" || Field=="loudnessrange" || Field=="maxtruepeaklevel" || Field=="maxmomentaryloudness" || Field=="maxshorttermloudness")
    {
        //Test
        string Message;
        size_t Minus=(!Value.empty() && Value[0]=='-')?1:0;
        size_t Decimal=Value.find('.');
        if (Value.empty())
            ;
        else if (Rules.Tech3285_Req)
        {
            if (Value.size()>=Minus+1 && (Value[Minus]<'0' || Value[Minus]>'9'))
                Message="must be XX.XX or -XX.XX";
            if (Value.size()>=Minus+2 && Decimal!=Minus+2 && (Value[Minus+1]<'0' && Value[Minus+1]>'9'))
                Message="must be XX.XX or -XX.XX";
            if (Value.size()>=Minus+3 && Decimal!=Minus+3 && (Value[Minus+2]<'0' && Value[Minus+2]>'9'))
                Message="must be XX.XX or -XX.XX";
            if (Value.size()>=Minus+4 && (Value[Minus+3]<'0' && Value[Minus+3]>'9'))
                Message="must be XX.XX or -XX.XX";
            if (Value.size()>=Minus+5 && (Value[Minus+4]<'0' && Value[Minus+4]>'9'))
                Message="must be XX.XX or -XX.XX";
            if (Value.size()>=Minus+6)
                Message="must be XX.XX or -XX.XX";
            if (Field=="loudnessrange" && Minus)
                Message="must be XX.XX or -XX.XX";
        }
        else
        {
            if (Value.size()>=Minus+1 && (Value[Minus]<'0' || Value[Minus]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+2 && Decimal!=Minus+2 && (Value[Minus+1]<'0' && Value[Minus+1]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+3 && Decimal!=Minus+3 && (Value[Minus+2]<'0' && Value[Minus+2]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+4 && Decimal!=Minus+4 && (Value[Minus+3]<'0' && Value[Minus+3]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+5 && (Value[Minus+4]<'0' && Value[Minus+4]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+6 && (Value[Minus+5]<'0' && Value[Minus+5]>'9'))
                Message="must be XXX.XX or -XXX.XX";
            if (Value.size()>=Minus+7)
                Message="must be XXX.XX or -XXX.XX";
            if (Field=="loudnessrange" && Minus)
                Message="must be positive value";
            if (Message.empty())
            {
                float32 Float=Ztring(Value).To_float32();
                if (Float<=-327.68 || Float>=327.69)
                {
                    if (Field=="loudnessrange")
                        Message="must be between 0 and 327.68";
                    else
                        Message="must be between -655.35 and 327.68";
                }
                int32s SavedValue=float32_int32s(Float*100);
                if (SavedValue<-32767 || SavedValue>32768)
                {
                    if (Field=="loudnessrange")
                        Message="must be 0 or 327.68";
                    else
                        Message="must be -327.67 or 327.68";
                }
            }
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, "<<Field<<" "<<Message;
    }

    else if (Field=="codinghistory")
    {
        //Test
        string Message;
        if (Rules.CodingHistory_Rec)
        {
            //Loading
            bool Wrong=false;
            ZtringListList List;
            List.Separator_Set(0, "\r\n");
            List.Separator_Set(1, ",");
            List.Write(Value);

            for (size_t Line_Pos=0; Line_Pos<List.size(); Line_Pos++)
            {
                for (size_t Data_Pos=0; Data_Pos<List[Line_Pos].size(); Data_Pos++)
                {
                    int Column=-1;
                    Ztring &Value=List[Line_Pos][Data_Pos];
                    if (Value.size()>=2 && Value[1]=='=')
                    {
                        switch (Value[0])
                        {
                            case 'A' : Column=0; break;
                            case 'F' : Column=1; break;
                            case 'B' : Column=2; break;
                            case 'W' : Column=3; break;
                            case 'M' : Column=4; break;
                            case 'T' : Column=5; break;
                            default  : Wrong=true;
                        }
                    }
                    else
                        Wrong=true;
                    if (Column==Data_Pos)
                    {
                        Value.erase(0, 2);
                        switch (Column)
                        {
                            case 0 :    if (Value!=""
                                         && Value!="ANALOGUE"
                                         && Value!="PCM"
                                         && Value!="MPEG1L1"
                                         && Value!="MPEG1L2"
                                         && Value!="MPEG1L3"
                                         && Value!="MPEG2L1"
                                         && Value!="MPEG2L2"
                                         && Value!="MPEG2L3")
                                        Wrong=true;
                                        break;
                                     

                            case 1 :    if (Value!=""
                                         && Value!="11000"
                                         && Value!="22050"
                                         && Value!="24000"
                                         && Value!="32000"
                                         && Value!="44100"
                                         && Value!="48000"
                                         && Value!="64000"
                                         && Value!="88200"
                                         && Value!="96000")
                                        Wrong=true;
                                        break;

                            case 2 :    if (Value!=""
                                         && Value!="8"
                                         && Value!="16"
                                         && Value!="24"
                                         && Value!="32"
                                         && Value!="40"
                                         && Value!="48"
                                         && Value!="56"
                                         && Value!="64"
                                         && Value!="80"
                                         && Value!="96"
                                         && Value!="112"
                                         && Value!="128"
                                         && Value!="144"
                                         && Value!="160"
                                         && Value!="176"
                                         && Value!="192"
                                         && Value!="224"
                                         && Value!="256"
                                         && Value!="320"
                                         && Value!="352"
                                         && Value!="384"
                                         && Value!="416"
                                         && Value!="448")
                                        Wrong=true;
                                        break;

                            case 3 :    if (Value!=""
                                         && Value!="8"
                                         && Value!="12"
                                         && Value!="14"
                                         && Value!="16"
                                         && Value!="18"
                                         && Value!="20"
                                         && Value!="22"
                                         && Value!="24"
                                         && Value!="32")
                                        Wrong=true;
                                        break;

                            case 4 :    if (Value!=""
                                         && Value!="mono"
                                         && Value!="stereo"
                                         && Value!="dual-mono"
                                         && Value!="joint-stereo")
                                        Wrong=true;
                                        break;
                            default :   ;
                        }
                    }
                    else if (Column!=-1 || Data_Pos>5)
                    {
                        List[Line_Pos].insert(List[Line_Pos].begin()+Data_Pos, Ztring());
                        if (List[Line_Pos].size()>6)
                            Wrong=true;
                    }
                }
            }
            
            if (Wrong)
                Message="does not respect rules ";
            else if (!Value.empty() && (Value.size()<2 || Value[Value.size()-2]!=_T('\r') || Value[Value.size()-1]!=_T('\n') ))
                Message="does not terminate with \\r\\n";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, CodingHistory "<<Message;
    }

    else if (Field=="IARL")
    {
        //Test
        string Message;
        if (Rules.FADGI_Rec && Chunks)
        {
                 if (Value.empty() && Chunks->Global->bext && !Chunks->Global->bext->Strings["originator"].empty())
                Message="must equal originator (FADGI recommandations)";
            else if (!Value.empty() && !Chunks->Global->bext)
                Message="must equal originator (FADGI recommandations)";
            else if (!Value.empty() && Chunks->Global->bext && Value!=Chunks->Global->bext->Strings["originator"])
                Message="must equal originator (FADGI recommandations)";
            else if (Value.empty() && Chunks->Global->INFO)
                Message="must not be empty (FADGI recommandations)";
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input (IARL "<<Message<<")";
    }

    else if (Field=="ICMT" && Rules.INFO_Req)
    {
        if (Value.find_first_of("\r\n")!=string::npos)
        {
            Errors<<(Chunks?Chunks->Global->File_Name:"")<<": malformed input (ICMT="<<Value<<", carriage return are not acceptable)"<<endl;
            return false;
        }
    }

    //ICRD
    else if (Field=="ICRD")
    {
        //Test
        string Message;
        if (Rules.INFO_Rec)
        {
            if (Value.empty())
                {}
            else if (Value.size()<=10 && Value.size()!=4 && Value.size()!=7 && Value.size()!=10)
                Message="must be YYYY, YYYY-MM, or YYYY-MM-DD or YYYY-MM-DD+junk (INFO Recommandations)";
            else
            {
                     if (Value[0]< '0' || Value[0]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (INFO Recommandations)";
                else if (Value[1]< '0' || Value[1]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (INFO Recommandations)";
                else if (Value[2]< '0' || Value[2]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (INFO Recommandations)";
                else if (Value[3]< '0' || Value[3]> '9') //Year
                    Message="1st to 4th characters (Year) must be between '0000' and '9999' (INFO Recommandations)";
                else if (Value.size()>=7) //YYYY-DD
                {
                         if (Value[4]!='-') //Hyphen
                        Message="5th character must be '-' (INFO Recommandations)";
                    else if (Value[5]< '0' || Value[5]> '1') //Month
                        Message="6th and 7th characters (Month) must be between '01' and '12' (INFO Recommandations)";
                    else if ((Value[6]< (Value[5]=='0'?'1':'0')) || (Value[6]> (Value[5]=='1'?'2':'9'))) //Only 01-12 //Month
                        Message="6th and 7th characters (Month) must be between '01' and '12' (INFO Recommandations)";
                    else if (Value.size()>=10) //YYYY-DD-MM
                    {
                             if (Value[7]!='-') //Hyphen
                            Message="8th character must be '-' (INFO Recommandations)";
                        else if (Value[8]< '0' || Value[8]> '3') //Day
                            Message="9th and 10th characters (Day) must be between '01' and '23' (INFO Recommandations)";
                        else if ((Value[9]< (Value[8]=='0'?'1':'0')) || (Value[9]> (Value[8]=='3'?'1':'9'))) //Only 01-31 //Day
                            Message="9th and 10th characters (Day) must be between '01' and '23' (INFO Recommandations)";
                    }
                }
            }
        }

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, ICRD "<<Message;
    }

    //ICRD
    else if (Field=="md5generated")
    {
        //Test
        string Message;
        if (!(Chunks->Global->MD5Generated && !Chunks->Global->MD5Generated->Strings["md5generated"].empty()) && !(Chunks->Global->MD5Stored && !Chunks->Global->MD5Stored->Strings["md5stored"].empty()) && Chunks->Global->MD5Generated->Strings["md5generated"]!=Chunks->Global->MD5Stored->Strings["md5stored"])
            Message="does not equal MD5Stored";

        //If error
        if (!Message.empty()) 
            IsValid_Errors<<"malformed input, MD5Generated "<<Message;
    }

    else if (Field=="errors")
    {
        return PerFile_Error.str().empty();
    }

    else if (Field=="information")
    {
        return true;
    }

    if (!IsValid_Errors.str().empty())
    {
        Errors<<(Chunks?Chunks->Global->File_Name:"")<<": "<<IsValid_Errors.str()<<endl;
        return false;
    }
    else
        return true;
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsOriginal(const string &Field, const string &Value)
{
    return IsOriginal(Field_Get(Field=="timereference (translated)"?Ztring("timereference"):Field), Value, *chunk_strings_Get(Field)); 
}

//---------------------------------------------------------------------------
string Riff_Handler::History(const string &Field)
{
    return History(Field_Get(Field), *chunk_strings_Get(Field)); 
}

//***************************************************************************
// Global
//***************************************************************************

//---------------------------------------------------------------------------
string Riff_Handler::Core_Header()
{
    string ToReturn;
    ToReturn+="FileName,";
    for (size_t Fields_Pos=Fields_Bext; Fields_Pos<=Fields_Info; Fields_Pos++) //Only Bext and Info
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
        {
            ToReturn+=xxxx_Strings[Fields_Pos][Pos];
            ToReturn+=',';
        }

    //Remove the extra ","
    if (ToReturn.empty())
        ToReturn.resize(ToReturn.size()-1);

    return ToReturn;
}

//---------------------------------------------------------------------------
string Riff_Handler::Core_Get(bool Batch_IsBackuping)
{
    //FromFile
    if (Batch_IsBackuping)
        return Core_FromFile;
    
    ZtringList List;
    List.Separator_Set(0, ",");

    List.push_back(Chunks->Global->File_Name);
    for (size_t Fields_Pos=Fields_Bext; Fields_Pos<=Fields_Info; Fields_Pos++) //Only Bext and Info
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
             List.push_back(Get(xxxx_Strings[Fields_Pos][Pos]));

    return List.Read();
}

//---------------------------------------------------------------------------
string Riff_Handler::Technical_Header()
{
    ostringstream ToReturn;
    ToReturn<<"FileName"<<',';
    ToReturn<<"FileSize"<<',';
    ToReturn<<"Format"<<',';
    ToReturn<<"CodecID"<<',';
    ToReturn<<"Channels"<<',';
    ToReturn<<"SampleRate"<<',';
    ToReturn<<"BitRate"<<',';
    ToReturn<<"BitPerSample"<<',';
    ToReturn<<"Duration"<<',';
    ToReturn<<"UnsupportedChunks"<<",";
    ToReturn<<"bext"<<',';
    ToReturn<<"INFO"<<',';
    ToReturn<<"XMP"<<',';
    ToReturn<<"aXML"<<',';
    ToReturn<<"iXML"<<',';
    ToReturn<<"MD5Stored"<<',';
    ToReturn<<"MD5Generated"<<',';
    ToReturn<<"Errors"<<',';
    ToReturn<<"Information";

    return ToReturn.str();
}

//---------------------------------------------------------------------------
string Riff_Handler::Technical_Get()
{
    ZtringList List;
    List.Separator_Set(0, _T(","));
    List.push_back(Chunks->Global->File_Name);
    List.push_back(Ztring::ToZtring(Chunks->Global->File_Size));
    if (File_IsValid)
    {
        List.push_back(Chunks->Global->IsRF64?"Wave (RF64)":"Wave");
        List.push_back( Chunks->Global->fmt_==NULL                                            ?"":Ztring().From_CC2(Chunks->Global->fmt_->formatType      ));
        List.push_back(((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->channelCount  ==0)?"":Ztring::ToZtring(Chunks->Global->fmt_->channelCount    )));
        List.push_back(((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->sampleRate    ==0)?"":Ztring::ToZtring(Chunks->Global->fmt_->sampleRate      )));
        List.push_back(((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->bytesPerSecond==0)?"":Ztring::ToZtring(Chunks->Global->fmt_->bytesPerSecond*8)));
        List.push_back(((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->bitsPerSample ==0)?"":Ztring::ToZtring(Chunks->Global->fmt_->bitsPerSample   )));
        List.push_back(((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->bytesPerSecond==0 || Chunks->Global->data==NULL || Chunks->Global->data->Size==(int64u)-1)?"":Ztring().Duration_From_Milliseconds(Chunks->Global->data->Size*1000/Chunks->Global->fmt_->bytesPerSecond)));
        List.push_back(Chunks->Global->UnsupportedChunks);
        if (Chunks->Global->bext!=NULL && !Chunks->Global->bext->Strings["bextversion"].empty())
            List.push_back(Get("bext").empty()?"No":("Version "+Chunks->Global->bext->Strings["bextversion"]));
        else
            List.push_back("No");
        List.push_back(Get("INFO").empty()?"No":"Yes");
        List.push_back(Get("XMP").empty()?"No":"Yes");
        List.push_back(Get("aXML").empty()?"No":"Yes");
        List.push_back(Get("iXML").empty()?"No":"Yes");
        List.push_back(Get("MD5Stored"));
        List.push_back(Get("MD5Generated"));
    }
    else
        List.resize(17);
    string Errors_Temp=PerFile_Error.str();
    if (!Errors_Temp.empty())
        Errors_Temp.resize(Errors_Temp.size()-1);
    List.push_back(Errors_Temp);
    string Information_Temp=PerFile_Information.str();
    if (!Information_Temp.empty())
        Information_Temp.resize(Information_Temp.size()-1);
    List.push_back(Information_Temp);
    return List.Read();
}

//***************************************************************************
// Info
//***************************************************************************

//---------------------------------------------------------------------------
string Riff_Handler::Trace_Get()
{
    return Chunks->Global->Trace.str();
}

//---------------------------------------------------------------------------
string Riff_Handler::FileName_Get()
{
    return Chunks->Global->File_Name;
}

//---------------------------------------------------------------------------
string Riff_Handler::FileDate_Get()
{
    return Chunks->Global->File_Date;
}

//---------------------------------------------------------------------------
float Riff_Handler::Progress_Get()
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return 0;    
    CriticalSectionLocker(Chunks->Global->CS);
    if (Chunks->Global->Progress==1)
        return (float)0.99; //Must finish opening, see Open()
    return Chunks->Global->Progress;
}

//---------------------------------------------------------------------------
void Riff_Handler::Progress_Clear()
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return;

    CriticalSectionLocker(Chunks->Global->CS);
    Chunks->Global->Progress=0;
}

//---------------------------------------------------------------------------
bool Riff_Handler::Canceled_Get()
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return false;    
    CriticalSectionLocker(Chunks->Global->CS);
    return File_IsCanceled;
}

//---------------------------------------------------------------------------
void Riff_Handler::Cancel()
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return;    
    CriticalSectionLocker(Chunks->Global->CS);
    Chunks->Global->Canceling=true;
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsModified_Get()
{
    bool ToReturn=false;
    for (size_t Fields_Pos=0; Fields_Pos<Fields_Max; Fields_Pos++)
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Pos]; Pos++)
            if (IsModified(xxxx_Strings[Fields_Pos][Pos]))
                ToReturn=true;

    if (!ToReturn && Chunks)
        ToReturn=Chunks->IsModified();

    return ToReturn;
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsValid_Get()
{
    return File_IsValid;
}

//***************************************************************************
// Helpers - Per item
//***************************************************************************

//---------------------------------------------------------------------------
string Riff_Handler::Get(const string &Field, Riff_Base::global::chunk_strings* &Chunk_Strings)
{
    if (Field=="errors")
        return PerFile_Error.str();
    if (Field=="information")
        return PerFile_Information.str();
    if (Field=="sample rate" ||Field=="samplerate")
        return (((Chunks->Global->fmt_==NULL || Chunks->Global->fmt_->sampleRate    ==0)?"":Ztring::ToZtring(Chunks->Global->fmt_->sampleRate      )));

    if (!File_IsValid)
        return string();

    //Special cases
    if (Field=="bext" && &Chunk_Strings && Chunk_Strings)
    {
        bool timereference_Display=true;
        if (Chunk_Strings->Strings["description"].empty()
         && Chunk_Strings->Strings["originator"].empty()   
         && Chunk_Strings->Strings["originatorreference"].empty()   
         && Chunk_Strings->Strings["originationdate"].empty()   
         && Chunk_Strings->Strings["originationtime"].empty()   
         && Chunk_Strings->Strings["timereference"].empty()   
         && Chunk_Strings->Strings["umid"].empty()   
         && Chunk_Strings->Strings["loudnessvalue"].empty()
         && Chunk_Strings->Strings["loudnessrange"].empty()
         && Chunk_Strings->Strings["maxtruepeaklevel"].empty()
         && Chunk_Strings->Strings["maxmomentaryloudness"].empty()
         && Chunk_Strings->Strings["maxshorttermloudness"].empty()
         && Chunk_Strings->Strings["codinghistory"].empty())
            timereference_Display=false;
        ZtringList List;
        List.Separator_Set(0, _T(","));
        List.push_back(Chunk_Strings->Strings["description"]);
        List.push_back(Chunk_Strings->Strings["originator"]);
        List.push_back(Chunk_Strings->Strings["originatorreference"]);
        List.push_back(Chunk_Strings->Strings["originationdate"]);
        List.push_back(Chunk_Strings->Strings["originationtime"]);
        List.push_back(Chunk_Strings->Strings["timereference (translated)"].empty()?(timereference_Display?Ztring("00:00:00.000"):Ztring()):Chunk_Strings->Strings["timereference (translated)"]);
        List.push_back(Chunk_Strings->Strings["timereference"].empty()?(timereference_Display?Ztring("0"):Ztring()):Chunk_Strings->Strings["timereference"]);
        List.push_back(timereference_Display?Chunk_Strings->Strings["bextversion"]:Ztring());
        List.push_back(Chunk_Strings->Strings["umid"]);
        List.push_back(Chunk_Strings->Strings["loudnessvalue"]);
        List.push_back(Chunk_Strings->Strings["loudnessrange"]);
        List.push_back(Chunk_Strings->Strings["maxtruepeaklevel"]);
        List.push_back(Chunk_Strings->Strings["maxmomentaryloudness"]);
        List.push_back(Chunk_Strings->Strings["maxshorttermloudness"]);
        List.push_back(Chunk_Strings->Strings["codinghistory"]);
        return List.Read();
    }
    //Special cases
    if (Field=="INFO" && &Chunk_Strings && Chunk_Strings)
    {
        ZtringList List;
        List.Separator_Set(0, _T(","));
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Info]; Pos++)
             List.push_back(Chunk_Strings->Strings[xxxx_Strings[Fields_Info][Pos]]);
        return List.Read();
    }
    if ((Field=="timereference (translated)" || Field=="timereference") && Chunk_Strings)
    {
        bool timereference_Display=false;
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Bext]; Pos++)
             if (!Chunk_Strings->Strings[Field_Get(xxxx_Strings[Fields_Bext][Pos])].empty() && Ztring(xxxx_Strings[Fields_Bext][Pos])!="BextVersion")
                timereference_Display=true;
        if (Field=="timereference (translated)")
            return Chunk_Strings->Strings["timereference"].empty()?(timereference_Display?Ztring("00:00:00.000"):Ztring()):Ztring().Duration_From_Milliseconds((int64u)(((float64)Chunk_Strings->Strings["timereference"].To_int64u())*1000/Chunks->Global->fmt_->sampleRate));
        else
            return Chunk_Strings->Strings["timereference"].empty()?(timereference_Display?Ztring("0"):Ztring()):Chunk_Strings->Strings["timereference"];
    }
    if (Field=="bextversion" && Chunk_Strings)
    {
        bool bextversion_Display=false;
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Bext]; Pos++)
             if (!Chunk_Strings->Strings[Field_Get(xxxx_Strings[Fields_Bext][Pos])].empty())
                bextversion_Display=true;
        return bextversion_Display?Chunk_Strings->Strings["bextversion"]:Ztring();
    }

    if (&Chunk_Strings && Chunk_Strings && Chunk_Strings->Strings.find(Field)!=Chunk_Strings->Strings.end())
        return Chunk_Strings->Strings[Field];
    else
        return Riff_Handler_EmptyZtring_Const;
}

//---------------------------------------------------------------------------
bool Riff_Handler::Set(const string &Field, const string &Value, Riff_Base::global::chunk_strings* &Chunk_Strings, int32u Chunk_Name2, int32u Chunk_Name3)
{
    if (!File_IsValid
     || &Chunk_Strings==NULL) 
        return false;
    
    if ((Chunk_Strings!=NULL && Value==Chunk_Strings->Strings[Field])
     || (Chunk_Strings==NULL && Value.empty()))
        return true; //Nothing to do

    //Overwrite_Reject
    if (Overwrite_Reject && Chunk_Strings!=NULL && !Chunk_Strings->Strings[Field].empty())
    {
        Errors<<(Chunks?Chunks->Global->File_Name:"")<<": overwriting is not authorized ("<<Field<<")"<<endl;
        return false;
    }

    //Log
    Ztring Value_ToDisplay=Value;
    Value_ToDisplay.FindAndReplace("\r", " ", 0, Ztring_Recursive);
    Value_ToDisplay.FindAndReplace("\n", " ", 0, Ztring_Recursive);
    Information<<(Chunks?Chunks->Global->File_Name:"")<<": "<<Field<<", "<<((Chunk_Strings==NULL || Chunk_Strings->Strings[Field].empty())?"(empty)":((Field=="xmp" || Field=="axml" || Field=="ixml")?"(XML data)":Chunk_Strings->Strings[Field].c_str()))<<" --> "<<(Value.empty()?"(removed)":((Field=="xmp" || Field=="axml" || Field=="ixml")?"(XML data)":Value_ToDisplay.c_str()))<<endl;
       
    //Special cases - Before
    if (Chunk_Strings==NULL)
        Chunk_Strings=new Riff_Base::global::chunk_strings();
    if (&Chunk_Strings==&Chunks->Global->bext && Field!="bextversion")
    {
        if (Field=="umid" && !Value.empty() && Ztring(Get("bextversion")).To_int16u()<1)
            Set("bextversion", "1", Chunk_Strings, Chunk_Name2, Chunk_Name3);
        if ((Field=="loudnessvalue" || Field=="loudnessrange" || Field=="maxtruepeaklevel" || Field=="maxmomentaryloudness" || Field=="maxshorttermloudness") && !Value.empty() && Ztring(Get("bextversion")).To_int16u()<2)
            Set("bextversion", "2", Chunk_Strings, Chunk_Name2, Chunk_Name3);
        if (!Value.empty() && Chunk_Strings->Strings["bextversion"].empty())
            Set("bextversion", Ztring::ToZtring(Bext_DefaultVersion), Chunk_Strings, Chunk_Name2, Chunk_Name3);
    }

    //Filling
    bool Alreadyexists=false;
    for (size_t Pos=0; Pos<Chunk_Strings->Histories[Field].size(); Pos++)
        if (Chunk_Strings->Histories[Field][Pos]==Chunk_Strings->Strings[Field])
            Alreadyexists=true;
    if (!Alreadyexists)
        Chunk_Strings->Histories[Field].push_back(Chunk_Strings->Strings[Field]);
    Chunk_Strings->Strings[Field]=Value;

    //Special cases - After
    if (Chunk_Strings && &Chunk_Strings==&Chunks->Global->bext && Field!="bextversion")
    {
        bool bextversion_Delete=true;
        for (size_t Pos=0; Pos<xxxx_Strings_Size[Fields_Bext]; Pos++)
             if (!Chunk_Strings->Strings[Field_Get(xxxx_Strings[Fields_Bext][Pos])].empty() && Ztring(xxxx_Strings[Fields_Bext][Pos])!="BextVersion")
                bextversion_Delete=false;
        if (bextversion_Delete)
            Set("bextversion", Ztring(), Chunk_Strings, Chunk_Name2, Chunk_Name3);
    }

    return true;
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsOriginal(const string &Field, const string &Value, Riff_Base::global::chunk_strings* &Chunk_Strings)
{
    if (!File_IsValid || &Chunk_Strings==NULL || Chunk_Strings==NULL)
        return Value.empty();
   
    //Special cases
    if (Field=="timereference (translated)" && &Chunk_Strings && Chunk_Strings && Chunk_Strings->Strings.find("timereference")!=Chunk_Strings->Strings.end())
        return IsOriginal("timereference", Get("timereference"));

    if (Chunk_Strings->Histories[Field].empty())
        return Value==Chunk_Strings->Strings[Field];
   
    return Value==Chunk_Strings->Histories[Field][0];
}

//---------------------------------------------------------------------------
bool Riff_Handler::IsModified(const string &Field, Riff_Base::global::chunk_strings* &Chunk_Strings)
{
    if (!File_IsValid)
        return false;

    //Special cases
    if (Field=="timereference (translated)" && &Chunk_Strings && Chunk_Strings && Chunk_Strings->Strings.find("timereference")!=Chunk_Strings->Strings.end())
        return IsModified("timereference");

    if (&Chunk_Strings!=NULL && Chunk_Strings && Chunk_Strings->Histories.find(Field)!=Chunk_Strings->Histories.end())
    {
        //Special cases
        if (Field=="bextversion")
            return !Chunk_Strings->Histories["bextversion"].empty() && !(Chunk_Strings->Strings["bextversion"]=="0" || Chunk_Strings->Histories["bextversion"][0]==Chunk_Strings->Strings["bextversion"]);

        return !Chunk_Strings->Histories[Field].empty() && Chunk_Strings->Histories[Field][0]!=Chunk_Strings->Strings[Field];
    }
    else
        return false;
}

//---------------------------------------------------------------------------
string Riff_Handler::History(const string &Field, Riff_Base::global::chunk_strings* &Chunk_Strings)
{
    if (!File_IsValid)
        return string();

    //Special cases
    if (Field=="timereference (translated)" && &Chunk_Strings && Chunk_Strings && Chunk_Strings->Strings.find("timereference")!=Chunk_Strings->Strings.end() && Chunks->Global->fmt_ && Chunks->Global->fmt_->sampleRate)
    {
        ZtringList List; List.Write(History("timereference", Chunk_Strings));
        for (size_t Pos=0; Pos<List.size(); Pos++)
            List[Pos].Duration_From_Milliseconds((int64u)(((float64)List[Pos].To_int64u())*1000/Chunks->Global->fmt_->sampleRate));
        
        //Removing Duplicate (Zero only, others are already done in timereference)
        bool ZeroAlreadyDone=false;
        for (size_t Pos=0; Pos<List.size(); Pos++)
        {
            if (List[Pos]=="00:00:00.000")
            {
                if (ZeroAlreadyDone)
                {
                    List.erase(List.begin()+Pos);
                    Pos--;
                }
                else
                    ZeroAlreadyDone=true;
            }
        }
        return List.Read();
    }

    if (&Chunk_Strings!=NULL && Chunk_Strings && Chunk_Strings->Strings.find(Field)!=Chunk_Strings->Strings.end())
        return Chunk_Strings->Histories[Field].Read();
    else
        return Riff_Handler_EmptyZtring_Const;
}

//***************************************************************************
// Configuration
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Handler::Options_Update()
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return;

    Chunks->Global->NoPadding_Accept=NoPadding_Accept;
    Chunks->Global->NewChunksAtTheEnd=NewChunksAtTheEnd;
    Chunks->Global->GenerateMD5=GenerateMD5;
    Chunks->Global->VerifyMD5=VerifyMD5;
    Chunks->Global->EmbedMD5=EmbedMD5;
    Chunks->Global->EmbedMD5_AuthorizeOverWritting=EmbedMD5_AuthorizeOverWritting;

    //MD5
    if (Chunks->Global->VerifyMD5)
    {
        //Removing all MD5 related info
        Ztring PerFile_Error_Temp=PerFile_Error.str();
        PerFile_Error_Temp.FindAndReplace(Ztring("MD5, failed verification\n"), Ztring());
        PerFile_Error.str(PerFile_Error_Temp);
        Ztring PerFile_Information_Temp=PerFile_Information.str();
        PerFile_Information_Temp.FindAndReplace(Ztring("MD5, no existing MD5 chunk\n"), Ztring());
        PerFile_Information_Temp.FindAndReplace(Ztring("MD5, verified\n"), Ztring());
        PerFile_Information.str(PerFile_Information_Temp);
        
        //Checking
        if (!(Chunks->Global->MD5Stored && !Chunks->Global->MD5Stored->Strings["md5stored"].empty()))
        {
            Information<<Chunks->Global->File_Name<<": MD5, no existing MD5 chunk"<<endl;
            PerFile_Information<<"MD5, no existing MD5 chunk"<<endl;
        }
        else if (Chunks->Global->MD5Generated && Chunks->Global->MD5Generated->Strings["md5generated"]!=Chunks->Global->MD5Stored->Strings["md5stored"])
        {
            Errors<<Chunks->Global->File_Name<<": MD5, failed verification"<<endl;
            PerFile_Error.str(string());
            PerFile_Error<<"MD5, failed verification"<<endl;
        }
        else
        {
            Information<<Chunks->Global->File_Name<<": MD5, verified"<<endl;
            PerFile_Information.str(string());
            PerFile_Information<<"MD5, verified"<<endl;
        }
    }
    if (EmbedMD5
     && Chunks->Global->MD5Generated && !Chunks->Global->MD5Generated->Strings["md5generated"].empty()
     && (!(Chunks->Global->MD5Stored && !Chunks->Global->MD5Stored->Strings["md5stored"].empty())
      || EmbedMD5_AuthorizeOverWritting))
            Set("MD5Stored", Chunks->Global->MD5Generated->Strings["md5generated"], rules());
}

//***************************************************************************
// Helpers - Retrieval of chunks info
//***************************************************************************

//---------------------------------------------------------------------------
Riff_Base::global::chunk_strings** Riff_Handler::chunk_strings_Get(const string &Field)
{
    if (Chunks==NULL || Chunks->Global==NULL)
        return NULL;    
        
    Ztring Field_Lowered=Ztring(Field).MakeLowerCase();
    if (Field_Lowered=="history")
        Field_Lowered="codinghistory";

    //bext
    if (Field_Lowered=="bext"
     || Field_Lowered=="description"
     || Field_Lowered=="originator"
     || Field_Lowered=="originatorreference"
     || Field_Lowered=="originationdate"
     || Field_Lowered=="originationtime"
     || Field_Lowered=="timereference (translated)"
     || Field_Lowered=="timereference"
     || Field_Lowered=="bextversion"
     || Field_Lowered=="umid"
     || Field_Lowered=="loudnessvalue"
     || Field_Lowered=="loudnessrange"
     || Field_Lowered=="maxtruepeaklevel"
     || Field_Lowered=="maxmomentaryloudness"
     || Field_Lowered=="maxshorttermloudness"
     || Field_Lowered=="codinghistory")
        return &Chunks->Global->bext;

    //MXP
    else if (Field_Lowered=="xmp")
        return &Chunks->Global->XMP;

    //aXML
    else if (Field_Lowered=="axml")
        return &Chunks->Global->aXML;
    
    //iXML
    else if (Field_Lowered=="ixml")
        return &Chunks->Global->iXML;
    
    //MD5Stored
    else if (Field_Lowered=="md5stored")
        return &Chunks->Global->MD5Stored;
    
    //MD5Stored
    else if (Field_Lowered=="md5generated")
        return &Chunks->Global->MD5Generated;
    
    //INFO
    else if (Field.size()==4)
        return &Chunks->Global->INFO;
    
    //Unknown
    else
        return NULL;
}

//---------------------------------------------------------------------------
string Riff_Handler::Field_Get(const string &Field)
{
    Ztring Field_Lowered=Ztring(Field).MakeLowerCase();
    if (Field_Lowered=="history")
        Field_Lowered="codinghistory";

    if (Field_Lowered=="filename"
     || Field_Lowered=="bext"
     || Field_Lowered=="description"
     || Field_Lowered=="originator"
     || Field_Lowered=="originatorreference"
     || Field_Lowered=="originationdate"
     || Field_Lowered=="originationtime"
     || Field_Lowered=="timereference (translated)"
     || Field_Lowered=="timereference"
     || Field_Lowered=="bextversion"
     || Field_Lowered=="umid"
     || Field_Lowered=="loudnessvalue"
     || Field_Lowered=="loudnessrange"
     || Field_Lowered=="maxtruepeaklevel"
     || Field_Lowered=="maxmomentaryloudness"
     || Field_Lowered=="maxshorttermloudness"
     || Field_Lowered=="codinghistory"
     || Field_Lowered=="xmp"
     || Field_Lowered=="axml"
     || Field_Lowered=="ixml"
     || Field_Lowered=="md5stored"
     || Field_Lowered=="md5generated")
        return Field_Lowered; 

    //Unknown 4 chars --> In INFO chunk, in uppercase
    if (Field.size()==4)
        return Ztring(Field).MakeUpperCase();

    //Unknown --> In INFO chunk, in uppercase
    return Field_Lowered;
}

//---------------------------------------------------------------------------
int32u Riff_Handler::Chunk_Name2_Get(const string &Field)
{
    Ztring Field_Lowered=Ztring(Field).MakeLowerCase();
    if (Field_Lowered=="history")
        Field_Lowered="codinghistory";
        
    //bext
    if (Field_Lowered=="bext"
     || Field_Lowered=="description"
     || Field_Lowered=="originator"
     || Field_Lowered=="originatorreference"
     || Field_Lowered=="originationdate"
     || Field_Lowered=="originationtime"
     || Field_Lowered=="timereference (translated)"
     || Field_Lowered=="timereference"
     || Field_Lowered=="bextversion"
     || Field_Lowered=="umid"
     || Field_Lowered=="loudnessvalue"
     || Field_Lowered=="loudnessrange"
     || Field_Lowered=="maxtruepeaklevel"
     || Field_Lowered=="maxmomentaryloudness"
     || Field_Lowered=="maxshorttermloudness"
     || Field_Lowered=="codinghistory")
        return Elements::WAVE_bext; 

    //XMP
    else if (Field_Lowered=="xmp")
        return Elements::WAVE__PMX; 

    //aXML
    else if (Field_Lowered=="axml")
        return Elements::WAVE_axml; 

    //iXML
    else if (Field_Lowered=="ixml")
        return Elements::WAVE_iXML; 

    //MD5Stored
    else if (Field_Lowered=="md5stored")
        return Elements::WAVE_MD5_; 

    //MD5Generated
    else if (Field_Lowered=="md5generated")
        return Elements::WAVE_MD5_; 

    //INFO
    else if (Field.size()==4)
        return Elements::WAVE_INFO;
    
    //Unknown
    return 0x00000000;
}

//---------------------------------------------------------------------------
int32u Riff_Handler::Chunk_Name3_Get(const string &Field)
{
    //INFO
    if (Chunk_Name2_Get(Field)==Elements::WAVE_INFO)
        return Ztring(Field).MakeUpperCase().To_CC4(); 
    
    //Unknown / not needed
    return 0x00000000;
}
