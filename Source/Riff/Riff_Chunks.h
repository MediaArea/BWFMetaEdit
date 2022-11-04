// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Riff_ChunksH
#define Riff_ChunksH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Riff/Riff_Base.h"
//#include <tchar.h>
//---------------------------------------------------------------------------

//***************************************************************************
// 
//***************************************************************************

class Riff : public Riff_Base
{
public:
    //Constructor/Destructor
    Riff();
    ~Riff();

protected:
    //Read/Write
    void Read_Internal();
};

//***************************************************************************
// Constants
//***************************************************************************

namespace Elements
{
    const int32u LIST=0x4C495354;
    const int32u RIFF=0x52494646;
    const int32u RF64=0x52463634;

    const int32u WAVE=0x57415645;
    const int32u WAVE__PMX=0x5F504D58;
    const int32u WAVE_aXML=0x61584D4C;
    const int32u WAVE_axml=0x61786D6C;
    const int32u WAVE_bext=0x62657874;
    const int32u WAVE_data=0x64617461;
    const int32u WAVE_ds64=0x64733634;
    const int32u WAVE_fact=0x66616374;
    const int32u WAVE_filr=0x66696C72;
    const int32u WAVE_FILR=0x46494C52;
    const int32u WAVE_fllr=0x666C6C72;
    const int32u WAVE_FLLR=0x464C4C52;
    const int32u WAVE_fmt_=0x666D7420;
    const int32u WAVE_junk=0x6A756E6B;
    const int32u WAVE_JUNK=0x4A554E4B;
    const int32u WAVE_junq=0x6A756E71;
    const int32u WAVE_JUNQ=0x4A554E51;
    const int32u WAVE_pad_=0x70616420;
    const int32u WAVE_PAD_=0x50414420;
    const int32u WAVE_ID3_=0x49443320;
    const int32u WAVE_iXML=0x69584D4C;
    const int32u WAVE_INFO=0x494E464F;
    const int32u WAVE_INFO_IARL=0x4941524C;
    const int32u WAVE_INFO_xxxx=0xFFFFFFFF;
    const int32u WAVE_MD5_=0x4D443520;
    const int32u WAVE_cue_=0x63756520;
    const int32u WAVE_adtl=0x6164746C;
    const int32u WAVE_adtl_labl=0x6C61626C;
    const int32u WAVE_adtl_note=0x6E6F7465;
    const int32u WAVE_adtl_ltxt=0x6C747874;
    const int32u WAVE_CSET=0x43534554;
}

//***************************************************************************
// List of chunks
//***************************************************************************

//Chunk without modification
#define CHUNK__(_Level, _Name) \
class Riff_##_Name : public Riff_Base \
{ \
public: \
    Riff_##_Name(global* Global):Riff_Base(Global, _Level) {Chunk.Header.Name=Elements::_Name;} \
protected: \
    void Read_Internal(); \
}; \

//Chunk with insertion
#define CHUNK_I(_Level, _Name) \
class Riff_##_Name : public Riff_Base \
{ \
public: \
    Riff_##_Name(global* Global):Riff_Base(Global, _Level) {Chunk.Header.List=Elements::LIST; Chunk.Header.Name=Elements::_Name;} \
protected: \
    void Read_Internal(); \
    size_t Insert_Internal(int32u Chunk_Name_Insert); \
}; \

//Chunk with modification/write
#define CHUNK_W(_Level, _Name) \
class Riff_##_Name : public Riff_Base \
{ \
public: \
    Riff_##_Name(global* Global):Riff_Base(Global, _Level) {Chunk.Header.Name=Elements::_Name;} \
protected: \
    void Read_Internal(); \
    void Modify_Internal(); \
    void Write_Internal(); \
}; \

CHUNK_I(1, WAVE);
CHUNK_W(2, WAVE__PMX);
CHUNK_W(2, WAVE_axml);
CHUNK_W(2, WAVE_bext);
CHUNK_W(2, WAVE_data);
CHUNK_W(2, WAVE_ds64);
CHUNK__(2, WAVE_fmt_);
CHUNK_W(2, WAVE_FLLR);
CHUNK_I(2, WAVE_INFO);
CHUNK_W(3, WAVE_INFO_xxxx);
CHUNK_W(2, WAVE_iXML);
CHUNK_W(2, WAVE_MD5_);
CHUNK_W(2, WAVE_cue_);
CHUNK_I(2, WAVE_adtl);
CHUNK_W(3, WAVE_adtl_labl);
CHUNK_W(3, WAVE_adtl_note);
CHUNK_W(3, WAVE_adtl_ltxt);
CHUNK_W(2, WAVE_CSET);

#endif
