// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Base.h"
#include <cstring>
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Defines
//***************************************************************************

#define INTEGRITY_SIZE_ATLEAST_VOID(_BYTES)\
    if (Chunk.Content.Buffer_Offset+_BYTES>Chunk.Content.Size)\
    {\
        Chunk.Content.Buffer_Offset=(size_t)Chunk.Content.Size;\
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST_INT(_BYTES) \
    if (Chunk.Content.Buffer_Offset+_BYTES>Chunk.Content.Size) \
    { \
        Chunk.Content.Buffer_Offset=(size_t)Chunk.Content.Size; \
        Value=0; \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST_STRING(_BYTES) \
    if (Chunk.Content.Buffer_Offset+_BYTES>Chunk.Content.Size) \
    { \
        Chunk.Content.Buffer_Offset=(size_t)Chunk.Content.Size; \
        Value.clear(); \
        return; \
    } \

//***************************************************************************
// Chunk.Content.Buffer handling - Little endian values
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Get_B1 (int8u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Value=BigEndian2int8u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=1;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B1 (int8u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    int8u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=1;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_B2 (int16u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Value=BigEndian2int16u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=2;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B2 (int16u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    int16u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=2;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_B3 (int32u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Value=BigEndian2int24u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=3;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B3 (int32u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    int24u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=3;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_B4 (int32u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Value=BigEndian2int32u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B4 (int32u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    int32u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_B8 (int64u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Value=BigEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=8;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B8 (int64u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    int64u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=8;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_B16(int128u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    Value.lo=BigEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Value.hi=BigEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8);
    Chunk.Content.Buffer_Offset+=16;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_B16(int128u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    int64u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value.lo);
    int64u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8, Value.hi);
    Chunk.Content.Buffer_Offset+=16;
}

//***************************************************************************
// Chunk.Content.Buffer handling - Little endian values
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Get_L1 (int8u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Value=LittleEndian2int8u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=1;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L1 (int8u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    int8u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=1;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_L2 (int16u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Value=LittleEndian2int16u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=2;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L2 (int16u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    int16u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=2;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_L3 (int32u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Value=LittleEndian2int24u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=3;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L3 (int32u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    int24u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=3;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_L4 (int32u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Value=LittleEndian2int32u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L4 (int32u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    int32u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_L8 (int64u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Value=LittleEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=8;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L8 (int64u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    int64u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=8;
}

//---------------------------------------------------------------------------
void Riff_Base::Get_L16(int128u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    Value.lo=LittleEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Value.hi=LittleEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8);
    Chunk.Content.Buffer_Offset+=16;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_L16(int128u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    int64u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value.lo);
    int64u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8, Value.hi);
    Chunk.Content.Buffer_Offset+=16;
}

//***************************************************************************
// Chunk.Content.Buffer handling - Character codes
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Get_C4 (int32u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Value=BigEndian2int8u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset);
    Chunk.Content.Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_C4 (int32u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    int32u2LittleEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value);
    Chunk.Content.Buffer_Offset+=4;
}

//***************************************************************************
// Chunk.Content.Buffer handling - UUID
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Get_UUID(int128u &Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    Value.hi=BigEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset  );
    Value.lo=BigEndian2int64u(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8);
    Chunk.Content.Buffer_Offset+=16;
}

//---------------------------------------------------------------------------
void Riff_Base::Put_UUID(int128u Value)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    int64u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value.hi);
    int64u2BigEndian(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset+8, Value.lo);
    Chunk.Content.Buffer_Offset+=16;
}

//***************************************************************************
// Chunk.Content.Buffer handling - Strings
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Get_String(int64u Value_Size, string &Value)
{
    INTEGRITY_SIZE_ATLEAST_STRING(Value_Size);
    Value.assign((const char*)(Chunk.Content.Buffer+(size_t)Chunk.Content.Buffer_Offset), (size_t)Value_Size);
    size_t ToCut=Value.find('\0');
    if (ToCut!=string::npos)
        Value.resize(ToCut);
    Chunk.Content.Buffer_Offset+=(size_t)Value_Size;
}


//---------------------------------------------------------------------------
void Riff_Base::Put_String(int64u Value_Size, string Value)
{
    INTEGRITY_SIZE_ATLEAST_STRING(Value_Size);
    memset(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, '\0', (size_t)Value_Size);
    std::memcpy(Chunk.Content.Buffer+Chunk.Content.Buffer_Offset, Value.c_str(), Value.size()>Value_Size?(size_t)Value_Size:Value.size());
    Chunk.Content.Buffer_Offset+=(size_t)Value_Size;
}

//***************************************************************************
// Buffer handling - Other
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_Base::Skip_XX (int64u Value_Size)
{
    INTEGRITY_SIZE_ATLEAST_VOID(Value_Size);
    Chunk.Content.Buffer_Offset+=(size_t)Value_Size;
}

