// BWF MetaEdit CLI - A Command Line Interface for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include <sstream>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "CLI/CLI_Help.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Help strings
//***************************************************************************

//---------------------------------------------------------------------------
std::string Help()
{
    std::ostringstream ToDisplay;

    ToDisplay<<"Usage: \"bwfmetaedit [--Options...] FileName1 [Filename2...]\""<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"Options:"<<std::endl;
    ToDisplay<<"--Help, -h              Display this help and exit"<<std::endl;
    ToDisplay<<"--Version               Display AVWG BWF MetaEdit version and exit"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Reject file if:"<<std::endl;
    ToDisplay<<"--reject-riff2rf64      Transformation to RF64 is requested"<<std::endl;
    ToDisplay<<"--reject-overwrite      Prevent overwrite of existing data(only add)"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Accept file if:"<<std::endl;
    ToDisplay<<"--accept-nopadding      Padding byte is missing"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Continue parsing if:"<<std::endl;
    ToDisplay<<"--continue-errors       There is an error in one or more input files"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        File modification options:"<<std::endl;
    ToDisplay<<"--append, -a            Place new or expanded chunks at the end of the file"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--verbose, -v           Display more details about modified values"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--simulate, -s          Simulate only (no write)"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--specialchars          \\ is considered as an escape char (e.g. \\r and \\n)"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Extract Technical Metadata to:"<<std::endl;
    ToDisplay<<"--out-tech              Display technical data"<<std::endl;
    ToDisplay<<"--out-tech=             specified file in CSV format"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Fill files with:"<<std::endl;
    ToDisplay<<"--in-core=              data from the specified Core file"<<std::endl;
    ToDisplay<<"--in-core-remove        clear data (remove bext and INFO)"<<std::endl;
    //ToDisplay<<"--in-core-xml           data from filename.core.xml (NOT IMPLEMENTED)"<<std::endl;
    ToDisplay<<"--Description=          specified bext description"<<std::endl;
    ToDisplay<<"--Originator=           specified bext originator"<<std::endl;
    ToDisplay<<"--OriginatorReference=  specified bext originator reference"<<std::endl;
    ToDisplay<<"--OriginationDate=      specified bext origination date"<<std::endl;
    ToDisplay<<"--OriginationTime=      specified bext origination time"<<std::endl;
    ToDisplay<<"--Timereference=        specified bext time reference"<<std::endl;
    ToDisplay<<"--UMID=                 specified bext umid"<<std::endl;
    ToDisplay<<"--LoudnessValue=        specified bext loundness value"<<std::endl;
    ToDisplay<<"--LoudnessRange=        specified bext loundness range"<<std::endl;
    ToDisplay<<"--MaxTruePeakLevel=     specified bext max true peak level"<<std::endl;
    ToDisplay<<"--MaxMomentaryLoudness= specified bext max mnomentary loundness"<<std::endl;
    ToDisplay<<"--MaxShortTermLoudness= specified bext maw short term loundness"<<std::endl;
    ToDisplay<<"--History=              specified bext history"<<std::endl;
    ToDisplay<<"--IARL=                 specified INFO IARL"<<std::endl;
    ToDisplay<<"--ISFT=                 specified INFO ISFT"<<std::endl;
    ToDisplay<<"--xxxx=                 specified INFO xxxx..."<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"                        Extract Core Document to:"<<std::endl;
    ToDisplay<<"--out-core              current display (std::cout) in CSV format"<<std::endl;
    ToDisplay<<"--out-core=             specified file in CSV format"<<std::endl;
    ToDisplay<<"--out-core-xml          filename.core.xml (1 ouput per file) in XML format"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--in-XMP=               Insert XMP from the specified file"<<std::endl;
    ToDisplay<<"--in-XMP-remove         Remove XMP"<<std::endl;
    ToDisplay<<"--in-XMP-xml            Insert XMP from filename._PMX.xml"<<std::endl;
    ToDisplay<<"--out-XMP-xml           Save XMP in filename._PMX.xml"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--in-aXML=              Insert aXML from the specified file"<<std::endl;
    ToDisplay<<"--in-aXML-remove        Remove aXML"<<std::endl;
    ToDisplay<<"--in-aXML-xml           Insert aXML from filename.aXML.xml"<<std::endl;
    ToDisplay<<"--out-aXML-xml          Save aXML in filename.aXML.xml"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--in-iXML=              Insert iXML from the specified file"<<std::endl;
    ToDisplay<<"--in-iXML-remove        Remove iXML"<<std::endl;
    ToDisplay<<"--in-iXML-xml           Insert iXML from filename.iXML.xml"<<std::endl;
    ToDisplay<<"--out-iXML-xml          Save iXML in filename.iXML.xml"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"*******************************************************************************"<<std::endl;
    ToDisplay<<""<<std::endl;
    ToDisplay<<"--MD5-Generate          Generate MD5 for audio data"<<std::endl;
    ToDisplay<<"--MD5-Verify            Verify MD5 for audio data"<<std::endl;
    ToDisplay<<"--MD5-Embed             Embed MD5 for audio data"<<std::endl;
    ToDisplay<<"--MD5-Embed-Overwrite   Embed MD5 for audio data - Allow overwriting"<<std::endl;
    ToDisplay<<""<<std::endl;

    return ToDisplay.str();
}

//---------------------------------------------------------------------------
std::string Help_Nothing()
{
    std::ostringstream ToDisplay;

    ToDisplay<<"Usage: \"bwfmetaedit [-Options...] FileName1 [Filename2...]\""<<std::endl;
    ToDisplay<<"\"bwfmetaedit --Help\" for displaying more information"<<std::endl;

    return ToDisplay.str();
}

