// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Chunks.h"
extern "C"
{
#include "MD5/md5.h"
}
#include "ZenLib/Utils.h"
//---------------------------------------------------------------------------

//***************************************************************************
// WAVE data
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Read_Internal ()
{
    //Integrity
    if (Global->data)
        throw exception_valid("2 data chunks");

    //Real size
    if (Global->ds64 && Global->ds64->dataSize!=(int64u)-1)
        Chunk.Content.Size=Global->ds64->dataSize;
    
    //Filling
    Global->data=new Riff_Base::global::chunk_data;
    Global->data->File_Offset=Global->In.Position_Get();
    Global->data->Size=Chunk.Content.Size;

    //MD5
    try
    {
        Chunk.Content.Buffer=new int8u[65536];
    }
    catch(...)
    {
        throw exception_read_chunk("Problem during memory allocation");
    }

    //Reading
    if (Global->GenerateMD5)
    {
        MD5Context MD5;
        MD5Init(&MD5);
        while(Chunk.Content.Buffer_Offset<Chunk.Content.Size)
        {
            size_t ToRead=(size_t)Chunk.Content.Size-Chunk.Content.Buffer_Offset;
            if (ToRead>65536)
                ToRead=65536;
            size_t BytesRead=Global->In.Read(Chunk.Content.Buffer, ToRead);
            if (BytesRead==0)
                break; //Read is finished
            Global->CS.Enter();
            Global->Progress=(float)Global->In.Position_Get()/Global->In.Size_Get();
            if (Global->Canceling)
            {
                Global->CS.Leave();
                throw exception_canceled();
            }
            Global->CS.Leave();
            //SleeperThread::msleep(200);
            Chunk.Content.Buffer_Offset+=BytesRead;
            MD5Update(&MD5, Chunk.Content.Buffer, (unsigned int)BytesRead);
        }
        if (Chunk.Content.Buffer_Offset<Chunk.Content.Size)
            throw exception_read();
        delete[] Chunk.Content.Buffer; Chunk.Content.Buffer=NULL;
        Chunk.Content.Buffer_Offset=0;
        int8u Digest[16];
        MD5Final(Digest, &MD5);
        int128u DigestI=BigEndian2int128u(Digest);
        Global->MD5Generated=new Riff_Base::global::chunk_strings;
        Global->MD5Generated->Strings["md5generated"]=Ztring().From_Number(DigestI, 16);
        while (Global->MD5Generated->Strings["md5generated"].size()<32)
            Global->MD5Generated->Strings["md5generated"].insert(Global->MD5Generated->Strings["md5generated"].begin(), '0'); //Padding with 0, this must be a 32-byte string    
    }
}

//***************************************************************************
// Modify
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Modify_Internal ()
{
    //No modification is possible
}

//***************************************************************************
// Write
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE_data::Write_Internal ()
{
    if (Global->Out_Buffer_File_TryModification)
        Global->Out_Buffer_WriteAtEnd=true;
    else
        Riff_Base::Write_Internal();
}

