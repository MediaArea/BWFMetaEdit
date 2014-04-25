// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Common/Common_About.h"
//---------------------------------------------------------------------------

//***************************************************************************
// NameVersion
//***************************************************************************

//---------------------------------------------------------------------------
const char* NameVersion_HTML() {return
"<a href=\"http://www.digitizationguidelines.gov\">Federal Agencies Digitization Guidelines Initiative (FADGI)</a> BWF MetaEdit 1.3.1"
;}

//---------------------------------------------------------------------------
const char* NameVersion_Text() {return
"Federal Agencies Digitization Guidelines Initiative (FADGI)\n"
"BWF MetaEdit 1.3.1"
;}

//***************************************************************************
// NameVersion
//***************************************************************************

//---------------------------------------------------------------------------
const char* Description_HTML() {return
"<h3>DESCRIPTION</h3>"
""
"BWF MetaEdit was developed by the Federal Agencies Digitization Guidelines Initiative (FADGI) supported by AudioVisual Preservation Solutions.<br />"
"<br />"
"This tool permits embedding, editing, and exporting of metadata in Broadcast WAVE Format (BWF) files. This tool can also enforce metadata guidelines developed by the <a href=\"http://www.digitizationguidelines.gov/audio-visual/documents/wave_metadata.html\">Federal Agencies Audio-Visual Working Group</a>, as well as format specifications from the European Broadcasting Union (EBU), Microsoft, and IBM."
;}

//---------------------------------------------------------------------------
const char* Description_Text() {return
"DESCRIPTION\n"
"\n"
"BWF MetaEdit was developed by the Federal Agencies Digitization\n"
"Guidelines Initiative (FADGI) supported by AudioVisual Preservation\n"
"Solutions.\n"
"\n"
"This tool permits embedding, editing, and exporting of metadata in\n" 
"Broadcast WAVE Format (BWF) files. This tool can also enforce metadata\n" 
"guidelines developed by the Federal Agencies Audio-Visual Working Group,\n"
"as well as format specifications from the European Broadcasting\n"
"Union (EBU), Microsoft, and IBM.\n"
"\n"
;}

//***************************************************************************
// AuthorLicense
//***************************************************************************

//---------------------------------------------------------------------------
const char* AuthorLicense_HTML() {return
"" //now in /Source/Ressource/License.html
;}

//---------------------------------------------------------------------------
const char* AuthorLicense_Text() {return
"AUTHOR and LICENSE\n"
"\n"
"This code was created in 2010 for the Library of Congress and the other\n"
"federal government agencies participating in the Federal Agencies\n"
"Digitization Guidelines Initiative and it is in the public domain.\n"
;}

//***************************************************************************
// Contact
//***************************************************************************

//---------------------------------------------------------------------------
const char* Contact_HTML() {return
"<h3>CONTACT</h3>"
""
"Please submit questions, comments or feedback to <a href=\"http://www.digitizationguidelines.gov/contact/\">digitizationguidelines.org/contact</a> and select \"Broadcast WAVE Metadata\" as the topic."
;}

//---------------------------------------------------------------------------
const char* Contact_Text() {return
"CONTACT\n"
"\n"
"Please submit questions, comments or feedback to\n"
"http://www.digitizationguidelines.gov/contact/ and select\n"
"\"Broadcast WAVE Metadata\" as the topic.\n"
"\n"
;}

//***************************************************************************
// Websites
//***************************************************************************

//---------------------------------------------------------------------------
const char* Websites_HTML() {return
"Developed with support from <a href=\"http://avpreserve.com\">AVPS</a> and <a href=\"http://mediaarea.net\">MediaArea.net</a>"
;}

//---------------------------------------------------------------------------
const char* Websites_Text() {return
"Developed with support from http://avpreserve.com and http://mediaarea.net"
"\n"
;}

//***************************************************************************
// Columns
//***************************************************************************

struct column_text
{
    char* Name;
    char* ToolTip;
};
const size_t Columns_Text_Size=18+15+17;
column_text Columns[]=
{
    {"FileSize", "In order to provide a concise presentation the filepaths are displayed from the directory path were the files diverge. Thus if two files are opened from /HardDrive1/audiofiles/music/audiofile.wav and /HardDrive1/audiofiles/spokenword/audiofileB.wav then these will be displayed in the table view as '/music/audiofile.wav' and '/spokenword/audiofileB.wav' since the '/HardDrive1/audiofiles' section of the Filepath is common to the open files. The CSV version of the Technical Metadata view displays the full filepath to each file."},
    {"Format", "The size of the audio file in bytes."},
    {"CodecID", "This value represents the type of compression utilized within the audio data."},
    {"Channels", "The number of audio signals that are encoded in the audio data."},
    {"SampleRate", "The number of audio samples per second."},
    {"BitRate", "The number of bits utilized per second of audio."},
    {"BitPerSample", "The number of bits utilized to produce one sample of audio."},
    {"Duration", "The total playing time for the audio data, displayed in HH:MM:SS.mmm (hours, minutes, seconds, milliseconds)"},
    {"UnsupportedChunks", "Unsupported chunks"},
    {"bext", "This column notes the existence of the <bext> chunk by either stating 'No', 'Version 0', 'Version 1' or 'Version 2' (indicated of the version of BEXT chunk utilized)."},
    {"INFO", "Notes the existence of the INFO chunk"},
    {"XMP", "Notes the existence of the XMP chunk"},
    {"aXML", "Notes the existence of the aXML chunk"},
    {"iXML", "Notes the existence of the iXML chunk"},
    {"MD5Stored", "If an MD5 chunk exists, this column will show the stored value. This MD5 value is a checksum of the audio data only (starting after the <data> chunk identifier and size) and does not include the file's metadata or any other chunk."},
    {"MD5Generated", "If the MD5 is generated it will appear in this column. Any conflict between this column and the 'MD5' column indicates a change in the audio data portion of the file between the current evaluation and the stored evaluation."},
    {"Errors", "Provides statements about potential structural problems with a given audio files: for instance if the RIFF size statement conflicts with the actual file size or if the audio files does not utilize padding bytes to follow odd byte length chunks."},
    {"Information", "Used for status notes and technical information during operation."},

    {"Description", "EBU Tech 3285 definition:\nASCII string (maximum 256 characters) containing a free description of the sequence.\nTo help applications which only display a short description, it is recommended that a resume of the description is contained in the first 64 characters, and the last 192 characters are use for details.\n\nFADGI recommendations:\nTwo capital letters which must be derived from ISO 3166 Country Codes\nOne comma\nOne space\nAcronym. The great majority of acronyms are one word in all caps but there are some acronyms that are more than one word. The resource GovSpeak: A Guide to Government Acronyms and Abbreviations is suggested as a source but not required.\nIf string entered is under 32 characters, allow free-text string of up to 32 characters. End field with a null character.\nThis string should be automatically entered into the IARL data space."},
    {"Originator", "EBU Tech 3285 definition: ASCII string (maximum 32 characters) containing the name of the originator/producer of the audio file.\nIf the length of the string is less than 32 characters, the field is ended by a null character.\n\nFADGI Application:\nThis element contains the entity responsible for the 'archiving' (creation, maintenance, preservation) of this digital item.\nEntity designations should be as specific as possible including a two-character county code to avoid the potential for conflict in the archiving entity's name.\nIf space permits within the 32 character limit, the archival entity should be identified at the most specific level within the institution.\nUse a standard abbreviation of entity names such as those found in the Guide to Government Acronyms & Abbreviations.\nIf an entity is not on this list, use a familiar abbreviation.\nUse the standard two-character ISO 3166 alpha 2 country code list.\n\nFADGI Requirements:\nStrongly recommended (if the Working Group had authority: 'required')\n\nFADGI Examples:\n US, LOC/RSS\n US, NARA\n US, EPA"},
    {"OriginatorReference", "EBU Tech 3285 definition: ASCII string (maximum 32 characters) containing a non ambiguous reference allocated by the originating organization.\nIf the length of the string is less than 32 characters, the field is ended by a null character.\n\nNote: The EBU has defined a format for the OriginatorReference field. See EBU Technical Recommendation R99-1999.\n\nFADGI recommendations:\nThis element contains the principal identifier or the 'best' identifier which uniquely differentiates one object from another, preferably at the file level.\nIf the principal identifier string is less than 32 characters, enter the entire identifier string.\nIf the principal identifier string is longer than 32 characters, enter this text: 'See Description for identifiers.'\nDo not embed identifiers which could pose a possible security risk, e.g., by exposing exact pathnames.\nNote: The Working Group perceived value in the practice of repeating the principal identifier as the first identifier in the BEXT Description element (which has 256 available characters)\nbut did not make this detail a strong recommendation. Comments from readers are welcome.\n\nFADGI Requirements:\nStrongly recommended (if the Working Group had authority: 'required')\n\nFADGI Examples:\n US, LOC/RSS\n US, NARA\n US, EPA"},
    {"OriginationDate", "EBU Tech 3285 definition:\n Ten ASCII characters containing the date of creation of the audio sequence. The format is YYYY-MM-DD (year-month-day).\n Year is defined from 0000 to 9999 Month is define from 1 to 12 Day is defined from 1 to 28,29,30 or 31\n The separator between the items can be anything but it is recommended that one of the following characters is used: hyphen, underscore, colon, space, or stop (period).\n\nFADGI recommendations:\nThis element contains the file creation date. This is understood to mean the local date in the timezone for the archival entity;\nthe structure of the BEXT chunk does not permit ISO 8601 datetime indication, which unambiguously indicates date and time in terms of UTC (Coordinated Universal Time or Temps Universel Coordonne).\n Year only: YYYY (eg 1997)\n Year and month: YYYY-MM (eg 1997-07)\n Complete date: YYYY-MM-DD (eg 1997-07-16)\n Where YYYY = four-digit year\n Where MM = two-digit month (01=January, etc.) Month is defined from 01 to 12 (use leading zeroes if less than 10);\n Where DD = two-digit day of month (01 through 31) Day is defined from 01 to 28, 29, 30 or 31 (use leading zeroes if less than 10).\n Use a hyphen as a separator between data elements.\nIf string entered is under 10 characters, end field with a null character.\n\nFADGI Formatting:\nISO 8601 YYYY-MM-DD. Year is defined from 0000 to 9999; Month is defined from 01 to 12 (use leading zeroes if less than 10); Day is defined from 01 to 28, 29, 30 or 31 (use leading zeroes if less than 10).\nThe separator between the items is a hyphen [-] with no spaces.\nNote: the Working Group adheres to the ISO 8601 truncation convention: the year should always be given but, if unknown, values for month and day are omitted.\nThus the string length may be four, seven, or ten. If the string entered is less than ten characters, end it with a null character.\n\nFADGI Requirements:\nStrongly recommended (if the Working Group had authority: 'required')"},
    {"OriginationTime", "EBU Tech 3285 definition: Eight ASCII characters containing the time of creation of the audio sequence. The format is HH-MM-SS (hours-minutes-seconds).\nHour is defined from 0 to 23. Minute and second are defined from 0 to 59.\nThe separator between the items can be anything but it is recommended that one of the following characters is used: hyphen, underscore, colon, space, or stop (period).\n\nFADGI recommendations:\nThis element contains the file creation time. This is understood to mean the local time in the timezone for the archival entity;\nthe structure of the BEXT chunk does not permit ISO 8601 datetime indication, which unambiguously indicates date and time in terms of UTC (Coordinated Universal Time or Temps Universel Coordonne).\nFollow the ISO 8601 model which allows for some flexibility depending upon how much time information is available:\n Hours and Minutes only: hh:mm\n Hours, minutes and seconds: hh:mm:ss\n Where hh = two digits of hour (00 through 23) (am/pm NOT allowed) Hour is defined from 00 to 23 (use leading zeroes if less than 10).\n Where mm = two digits of minute (00 through 59)\n Where ss = two digits of second (00 through 59) Minute and second are defined from 00 to 59 (use leading zeroes if less than 10).\nIf string entered is under 8 characters, end field with a null character\n\nFADGI Requirements:\nOptional"},
    {"TimeReference (translated)", "Same as TimeReference but converted to HH:MM:SS.mmm format (hours minutes seconds milliseconds)"},
    {"TimeReference", "EBU Tech 3285 definition:\nThis field contains the timecode of the sequence.\nIt is a 64-bit value which contains the first sample count since midnight.\nThe number of samples per second depends on the sample frequency which is defined in the field <nSamplesPerSec> from the <format chunk>\n\nFADGI recommendations:\nThe Working Group believes that TimeReference can be used in a manner that supports the management of files that are parts of multitrack or multisegment works and plans to provide recommendations in the future.\n\nFADGI Requirements:\nOptional but recommended for complex recordings"},
    {"BextVersion", "Use to express whether the Bext Chunk follows the Rules of EBU Technical document 3285 as published in 1997 (version '0') or the 2001 update which created version '1' or the 2011 update which created version '2'.\nVersion '1' adds an additional field for UMID, Version '2' adds an additional field for loudness. . Example '1'."},
    {"UMID", "UMID: Unique Material Identifier"},
    {"LoudnessValue", "Integrated Loudness Value of the file in LUFS"},
    {"LoudnessRange", "Loudness Range of the file in LU "},
    {"MaxTruePeakLevel", "Maximum True Peak Level of the file expressed as dBTP"},
    {"MaxMomentaryLoudness", "Highest value of the Momentary Loudness Level of the file in LUFS"},
    {"MaxShortTermLoudness", "Highest value of the Short-Term Loudness Level of the file in LUFS"},
    {"CodingHistory", "EBU Tech 3285 definition:\nNon-restricted ASCII characters, containing a collection of strings terminated by CR/LF.\nEach string contains a description of a coding process applied to the audio data.\nEach new coding application is required to add a new string with the appropriate information.\nThis information must contain the type of sound (PCM or MPEG) with its specific parameters:\n PCM : mode (mono, stereo), size of the sample (8, 16 bits) and sample frequency:\n MPEG : sample frequency, bit-rate, layer (I or II) and the mode (mono, stereo, joint stereo or dual channel).\nIt is recommended that the manufacturers of the coders provide an ASCII string for use in the coding history.\nNote: The EBU has defined a format for CodingHistory which will simplify the interpretation of the information provided in this field. See EBU Technical Recommendation R98-1999\n\nFADGI recommendations:\nThis element is designed to hold data on the digitizing process including signal chain specifics, sample rate and bit depth, and other elements.\nIt is defined as a collection of strings, each presented on a separate line, containing a history of the coding processes applied to the file.\nThe first line documents the analog source recording, the second line contains data on the capture process, the third line of data records information on the storage of the file.\nA new line is added when the coding history related to the file is changed.\n\nFADGI Requirements:\nOptional"},

    {"IARL", "Archival Location\nIndicates where the subject of the file is archived.\n\nFADGI Application:\nThis element is an approximate equivalent to the Originator element in the BEXT chunk which records the entity responsible for the 'archiving' (creation, maintenance, preservation) of this digital item.\nIf used, this element could repeat the data from the Originator element in the same structured format.\nIn some applications, it is automatically carried over to the MP3 ID3 element 'ArchivalLocation'.\nOf all the info list chunk elements, this is the most important for Federal Agencies because it is vital to document the archival responsibility for a content item.\nEntity designations should be as specific as possible including a two-character county code to avoid the potential for conflict in the archiving entity's name.\nThe archival entity should be identified at the most specific level within the institution.\n\nFADGI Requirement:\nStrongly recommended ('required') if using LIST-INFO chunk.\n\nFADGI Formatting:\nTwo capital letters which must be derived from ISO 3166 Country Codes,\nOne comma,\nOne space,\nInstitutional Acronym.\nAlmost always one word all caps but there are some acronyms that are more than one word. The resource GovSpeak: A Guide to Government Acronyms and Abbreviations is suggested as a source but not required.\nThis string should be identical to what is in the Originator data space.\nIf any fields in LIST-INFO are used, this data should automatically carry over from Originator.\n\nFADGI Examples:\n US, NARA\n US, LOC/RSS"},
    {"IART", "Artist\nLists the artist of the original subject of the file. For example, Michaelangelo."},
    {"ICMS", "Commissioned\nLists the name of the person or organization that commissioned the subject of the file. For example, Pope Julian II."},
    {"ICMT", "Comment\nProvides general comments about the file or the subject of the file. If the comment is several sentences long, end each sentence with a period. Do not include newline characters.\n\nFADGI Application:\nIf used, this element should repeat the principal and other identifiers from the OriginatorReference and/or Description element in the BEXT chunk.\nEach identifier will be labeled as to its origin or purpose using the 'type' and 'comment' qualifiers.\nUsing the tag 'local' in the 'type' qualifier implies the entity identified in the BEXT Originator element.\nIn some applications, this info list element may carry forward automatically to the MP3 ID3 tagged element 'Comment'.\nNote: The Working Group perceived value in the practices of labeling identifiers (see examples) but wished to leave this as an optional practice.\n\nFADGI Requirement:\nOptional\n\nFADGI Formatting:\nIf labeled: Identifier [comma space] type [comma space] comment [semicolon-space if more than one identifier]\nIf no labeling: Identifier\n\nFADGI Examples:\nNARA: 58979818, local, principal ID original filename\nNARA: 306-MUSA-9658B, local, RG-Series-Item Number\nLC: 1201566-2-1, local, system-generated number; RYI 6039, local, source location number\nLC: http://hdl.loc.gov/loc.mbrsmi/westhpp.2033, URL, principal ID Handle\nDigital Library: ark.cdlib.org.org/ark:/13030/tf5p30086k, URL, ARK\nLC: http://lccn.loc.gov/mp76000002"},
    {"ICOP", "Copyright\nRecords the copyright information for the file. For example, Copyright Encyclopedia International 1991. If there are multiple copyrights, separate them by a semicolon followed by a space.\n\nFADGI Application:\nInformation about copyright and other restrictions (donor, privacy, etc.).\nUsage by federal agencies will often refer to the documentation of restrictions provided by other, non- embedded metadata.\nThis element has no equivalent in the BEXT chunk.\nIn some applications, this info list element may carry forward automatically to MP3 ID3 tagged element \"Copyright.\"\n\nIf used, this element may contain the information as known at the time of embedding.\nThe Working Group understands that metadata may be updated over time.\nUsers who refer to an embedded block of metadata should understand that this may not represent the latest and best information,\nand they should be counseled to follow one of the identifiers in order to obtain later and/or better data.\n\nFADGI Requirement:\nOptional\n\nFADGI Examples:\n LC: Publication and other forms of distribution may be restricted. For details, contact the Recorded Sound Section of the Library of Congress. [Preferred wording from LC, RSS]\nLC: (p) Rhino Records 2002.\nLC: 1201566-2-1, local, system-generated number; RYI 6039, local, source location number\nLC<: See Copyright Restriction Statement. [Used by LC, American Folklife Center]"},
    {"ICRD", "Creation date\nSpecifies the date the subject of the file was created. List dates in year-month-day format, padding one-digit months and days with a zero on the left. For example: 1553-05-03 for May 3, 1553.\nThe year should always be given using four digits.\n\nFADGI Application: This element is an equivalent to the OriginationDate in the BEXT chunk.\nIf used, this element could repeat the information from the OriginationDate element in the BEXT chunk using the same structured format.\nIn some applications, this info list element may carry forward automatically to MP3 ID3 tagged element \"CreationDate.\"\n\nFADGI Requirement:\nOptional.\n\nFADGI Example:\n2005-11-30"},
    {"IENG", "Engineer\nStores the name of the engineer who worked on the file. If there are multiple engineers, separate the names by a semicolon and a blank.\n\nFADGI example:\nSmith, John; Adams, Joe."},
    {"IGNR", "Genre\nDescribes the original work, such as jazz, classical, rock, etc."},
    {"IKEY", "Keywords\nProvides a list of keywords that refer to the file or subject of the file. Separate multiple keywords with a semicolon and a blank.\n\nFADGI example:\n Seattle; zoology; The Civil War."},
    {"IMED", "Medium\nDescribes the original subject of the file, such as record, CD and so forth."},
    {"INAM", "Name (Title)\nStores the title of the subject of the file, such as Seattle From Above.\n\nFADGI Application:\nIf used, this element should contain a working title.\nThis phrase is intended to convey the Working Group's understanding that metadata may be updated over time.\nUsers who refer to elements like titles in an embedded block of metadata should understand that this may not represent the latest and best information,\nand they should be counseled to follow one of the identifiers in order to obtain later and/or better data.\n\nFADGI Requirement:\nOptional.\n\nFADGI Examples:\nLC: Symphony no. 3 in A minor, op. 56\nLC: Interview with Bo Dollis at WWOZ, New Orleans, 1999-03-24 [title assigned by cataloger]\nLC: Adventures in research. No. 587, Sawing off Manhattan Island [episode in a radio series]"},
    {"IPRD", "Product (Album)\nSpecifies the name of the title the file was originally intended for, such as The Civil War: The Musical."},
    {"ISBJ", "Subject\nDescribes the contents of the file, such as Metadata Management."},
    {"ISFT", "Software\nIdentifies the name of the software package used to create the file, such as Audacity 1.3.9."},
    {"ISRC", "Source\nIdentifies the name of the person or organization who supplied the original subject of the file. For example: NARA."},
    {"ISRF", "Source Form\nIdentifies the original form of the material that was digitized, such as record, sampling CD, TV sound track and so forth. This is not necessarily the same as IMED."},
    {"ITCH", "Technician\nIdentifies the technician who sampled the subject file.\nFor example: Smith, John."},
};

//---------------------------------------------------------------------------
const char* Columns_ToolTip(std::string Name)
{
    for (size_t Pos=0; Pos<Columns_Text_Size; Pos++)
        if (Name==Columns[Pos].Name)
            return Columns[Pos].ToolTip; 
    return ""; //no info
}

