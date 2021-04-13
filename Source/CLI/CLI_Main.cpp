// BWF MetaEdit CLI - A Command Line Interface for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <algorithm>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "CLI/CommandLine_Parser.h"
#include "CLI/CLI_Help.h"
#include "Common/Core.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
extern ZtringList In_Core_File_List;
//---------------------------------------------------------------------------

//***************************************************************************
// Main
//***************************************************************************

int main(int argc, char* argv[])
{
    #ifdef _WIN32
    // Retrieve Windows command line
    int ArgcW;
    LPWSTR* ArgvW=NULL;
    ArgvW=CommandLineToArgvW(GetCommandLineW(), &ArgcW);

    SetConsoleOutputCP(CP_UTF8);
    #else
    //Localisation
    setlocale(LC_ALL, """""");
    #endif

    //Configure core
    Core C;

    //Retrieve command line (mainly for Unicode) and parse it
    ZtringList Files;
    #ifdef _WIN32
    for (int Pos=1; Pos<ArgcW; Pos++)
    #else
    for (int Pos=1; Pos<argc; Pos++)
    #endif
    {
        //First part of argument (before "=") should be case insensitive
        #ifdef _WIN32
        string Argument(Ztring().From_Unicode(ArgvW[Pos]).To_UTF8());
        #else
        string Argument(argv[Pos]);
        #endif
        size_t Egal_Pos=Argument.find(__T('='));
        if (Egal_Pos==string::npos)
            Egal_Pos=Argument.size();
        transform(Argument.begin(), Argument.begin()+Egal_Pos, Argument.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix
        int Return=Parse (C, Argument);
        if (Return>=0)
        {
            #ifdef _WIN32
            LocalFree(ArgvW);
            #endif
            return Return; //no more tasks to do
        }
        if (Return==-1)
            #ifdef _WIN32
            Files.push_back(Ztring().From_Unicode(ArgvW[Pos]));
            #else
            Files.push_back(Ztring().From_Local(argv[Pos]));
            #endif
    }

    #ifdef _WIN32
    LocalFree(ArgvW);
    #endif
     
    //Parsing
    C.Menu_File_Open_Files_Begin();
    for (size_t Pos=0; Pos<In_Core_File_List.size(); Pos++)
        if (C.Menu_File_Import_Core(In_Core_File_List[Pos].To_UTF8())<0)
        {
            //Errors
            std::cerr<<C.Text_stderr.str()<<std::endl;
            std::cerr<<"Correct errors before trying again"<<std::endl;
            return 0; //Stopping on error is requested
        }
    for (size_t Pos=0; Pos<Files.size(); Pos++)
        C.Menu_File_Open_Files_Continue(Files[Pos].To_UTF8());

    //If no filenames (and no options with filenames)
    if (C.Menu_File_Open_Files_Open_Get()==0)
    {
        std::cout<<Help_Nothing();
        return 0;
    }

    //Batch
    C.Batch_Enabled=true;
    if (C.Menu_File_Open_Files_Finish()==0)
    {
        //Errors
        std::cerr<<C.Text_stderr.str()<<std::endl;
        std::cerr<<"Correct errors before trying again"<<std::endl;
        return 0;
    }

    //Technical
    if (C.Out_Tech_cout)
        std::cout<<C.Technical_Get()<<std::endl;

    //Core
    if (C.Out_Core_cout)
        std::cout<<C.Core_Get()<<std::endl;

    //Log and errors
    if (C.Out_Log_cout)
        std::cout<<C.Text_stdall.str()<<std::endl;
    else
        std::cerr<<C.Text_stderr.str()<<std::endl;

    if (C.Text_stderr_Updated_Get())
        return 1;

    return 0;
}
//---------------------------------------------------------------------------

