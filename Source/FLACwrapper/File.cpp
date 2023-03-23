/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a zlib-style license that can
 *  be found in the License.txt file in the root of the source tree.
 */

#include "ZenLib/File.h"
#include "FLACwrapper/File.h"
//---------------------------------------------------------------------------

namespace FLACwrapper
{


// ***************************************************************************
// Constructor/Destructor
// ***************************************************************************

//---------------------------------------------------------------------------
File::File()
{
    File_Handle = ZenLib::File();
}

File::File(ZenLib::Ztring File_Name, ZenLib::File::access_t Access)
{
    File_Handle = ZenLib::File(File_Name, Access);
}

//---------------------------------------------------------------------------
File::~File()
{
}

// ***************************************************************************
// Open/Close
// ***************************************************************************

//---------------------------------------------------------------------------
bool File::Open (const ZenLib::tstring &File_Name_, ZenLib::File::access_t Access)
{
    return File_Handle.Open(File_Name_, Access);
}
//---------------------------------------------------------------------------
bool File::Create (const ZenLib::Ztring &File_Name_, bool OverWrite)
{
    return File_Handle.Create(File_Name_, OverWrite);
}

//---------------------------------------------------------------------------
void File::Close ()
{
    File_Handle.Close();
}

// ***************************************************************************
// Read/Write
// ***************************************************************************

//---------------------------------------------------------------------------
size_t File::Read (ZenLib::int8u* Buffer, size_t Buffer_Size_Max)
{
    return File_Handle.Read(Buffer, Buffer_Size_Max);
}

//---------------------------------------------------------------------------
size_t File::Write (const ZenLib::int8u* Buffer, size_t Buffer_Size)
{
    return File_Handle.Write(Buffer, Buffer_Size);
}

//---------------------------------------------------------------------------
bool File::Truncate (ZenLib::int64u Offset)
{
    return File_Handle.Truncate(Offset);
}

//---------------------------------------------------------------------------
size_t File::Write (const ZenLib::Ztring &ToWrite)
{
    return File_Handle.Write(ToWrite);
}

// ***************************************************************************
// Moving
// ***************************************************************************

//---------------------------------------------------------------------------
bool File::GoTo (ZenLib::int64s Position_ToMove, ZenLib::File::move_t MoveMethod)
{
    return File_Handle.GoTo(Position_ToMove, MoveMethod);
}

//---------------------------------------------------------------------------
ZenLib::int64u File::Position_Get ()
{
    return File_Handle.Position_Get();
}

// ***************************************************************************
// Attributes
// ***************************************************************************

//---------------------------------------------------------------------------
ZenLib::int64u File::Size_Get()
{
    return File_Handle.Size_Get();
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Created_Get()
{
    return File_Handle.Created_Get();
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Created_Local_Get()
{
    return File_Handle.Created_Local_Get();
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Modified_Get()
{
    return File_Handle.Modified_Get();
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Modified_Local_Get()
{
    return File_Handle.Modified_Local_Get();
}

//---------------------------------------------------------------------------
bool File::Opened_Get()
{
    return File_Handle.Opened_Get();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
ZenLib::int64u File::Size_Get(const ZenLib::Ztring &File_Name)
{
    ZenLib::File F(File_Name);
    return F.Size_Get();
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Created_Get(const ZenLib::Ztring &File_Name)
{
    ZenLib::File F(File_Name);
    return F.Created_Get(File_Name);
}

//---------------------------------------------------------------------------
ZenLib::Ztring File::Modified_Get(const ZenLib::Ztring &File_Name)
{
    ZenLib::File F(File_Name);
    return F.Modified_Get(File_Name);
}

//---------------------------------------------------------------------------
bool File::Exists(const ZenLib::Ztring &File_Name)
{
    ZenLib::File F(File_Name);
    return F.Exists(File_Name);
}

//---------------------------------------------------------------------------
bool File::Copy(const ZenLib::Ztring &Source, const ZenLib::Ztring &Destination, bool OverWrite)
{
    return ZenLib::File::Copy(Source, Destination, OverWrite);
}

//---------------------------------------------------------------------------
bool File::Move(const ZenLib::Ztring &Source, const ZenLib::Ztring &Destination, bool OverWrite)
{
    return ZenLib::File::Move(Source, Destination, OverWrite);
}

//---------------------------------------------------------------------------
bool File::Delete(const ZenLib::Ztring &File_Name)
{
    return ZenLib::File::Delete(File_Name);
}

//***************************************************************************
//
//***************************************************************************

} //namespace
