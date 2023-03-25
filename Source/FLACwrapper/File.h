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
#include "ZenLib/FileName.h"
#include "FLAC/metadata.h"
#include "FLAC/stream_decoder.h"
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

    //Callback client_data
    struct clientdata_t {
        char * chunkbuffer;
        ZenLib::int64u chunkbuffer_size;
        ZenLib::int64u chunkbuffer_readpointer;
        ZenLib::int64u chunkbuffer_data_location;
        ZenLib::int64u chunkbuffer_data_length;
        bool chunkbuffer_complete;
        char * audiobuffer;
        ZenLib::int64u audiobuffer_size;
        ZenLib::int64u audiobuffer_sizeinuse;
        ZenLib::int64u audiobuffer_readpointer;
        ZenLib::int64u audiobuffer_channels;
        ZenLib::int64u audiobuffer_bps;
        bool got_error;
    };

private:
    ZenLib::File File_Handle;
    clientdata_t callbackdata = {NULL, 0, 0, 0, 0, false, NULL, 0, 0, 0, 0, 0, false};
    FLAC__StreamDecoder * FLAC_decoder = NULL;
    bool FLAC_available;

    // Function pointers
    void * FLAChandle;
    const char ** FLACversion;
    FLAC__StreamDecoder *               (*local_FLAC_stream_decoder_new)();
    FLAC__bool                          (*local_FLAC_stream_decoder_set_metadata_ignore_all)(FLAC__StreamDecoder *);
    FLAC__bool                          (*local_FLAC_stream_decoder_set_metadata_respond_application)(FLAC__StreamDecoder *, const FLAC__byte *);
    FLAC__StreamDecoderInitStatus       (*local_FLAC_stream_decoder_init_file)(FLAC__StreamDecoder *, const char *, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void *);
    FLAC__bool                          (*local_FLAC_stream_decoder_process_until_end_of_metadata)(FLAC__StreamDecoder *);
    FLAC__bool                          (*local_FLAC_stream_decoder_process_single)(FLAC__StreamDecoder *);
    FLAC__bool                          (*local_FLAC_stream_decoder_finish)(FLAC__StreamDecoder *);
    FLAC__bool                          (*local_FLAC_stream_decoder_reset)(FLAC__StreamDecoder *);
    void                                (*local_FLAC_stream_decoder_delete)(FLAC__StreamDecoder *);
    void                                (*local_FLAC_metadata_object_delete)(FLAC__StreamMetadata *);
};

// Callback functions

FLAC__StreamDecoderWriteStatus FLAC_WriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data);
void FLAC_MetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
void FLAC_ErrorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

} //NameSpace

#endif
