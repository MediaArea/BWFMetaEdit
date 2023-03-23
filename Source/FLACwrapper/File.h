/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a zlib-style license that can
 *  be found in the License.txt file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// File functions
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef FLACwrapper_FileH
#define FLACwrapper_FileH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
//---------------------------------------------------------------------------

namespace FLACwrapper
{

//***************************************************************************
/// @brief File manipulation
//***************************************************************************

class File
{
public :
    //Constructor/Destructor
    File  ();
    File  (ZenLib::Ztring File_Name, ZenLib::File::access_t Access=ZenLib::File::Access_Read);
    ~File ();

    //Open/close
    bool Open  (const ZenLib::tstring &File_Name, ZenLib::File::access_t Access=ZenLib::File::Access_Read);
    bool Create(const ZenLib::Ztring &File_Name, bool OverWrite=true);
    void Close ();

    //Read/Write
    size_t Read  (ZenLib::int8u* Buffer, size_t Buffer_Size);
    size_t Write (const ZenLib::int8u* Buffer, size_t Buffer_Size);
    size_t Write (const ZenLib::Ztring &ToWrite);
    bool   Truncate (ZenLib::int64u Offset=(ZenLib::int64u)-1);

    //Moving
    bool GoTo (ZenLib::int64s Position, ZenLib::File::move_t MoveMethod=ZenLib::File::FromBegin);
    ZenLib::int64u Position_Get ();

    //Attributes
    ZenLib::int64u Size_Get();
    ZenLib::Ztring Created_Get();
    ZenLib::Ztring Created_Local_Get();
    ZenLib::Ztring Modified_Get();
    ZenLib::Ztring Modified_Local_Get();
    bool   Opened_Get();

    //Helpers
    static ZenLib::int64u   Size_Get(const ZenLib::Ztring &File_Name);
    static ZenLib::Ztring   Created_Get(const ZenLib::Ztring &File_Name);
    static ZenLib::Ztring   Modified_Get(const ZenLib::Ztring &File_Name);
    static bool     Exists(const ZenLib::Ztring &File_Name);
    static bool     Copy(const ZenLib::Ztring &Source, const ZenLib::Ztring &Destination, bool OverWrite=false);
    static bool     Move(const ZenLib::Ztring &Source, const ZenLib::Ztring &Destination, bool OverWrite=false);
    static bool     Delete(const ZenLib::Ztring &File_Name);

private:
    ZenLib::File File_Handle;
};

} //NameSpace

#endif
