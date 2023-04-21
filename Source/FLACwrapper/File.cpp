/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a zlib-style license that can
 *  be found in the License.txt file in the root of the source tree.
 */

#include "FLACwrapper/File.h"
#include <dlfcn.h>
//---------------------------------------------------------------------------

namespace FLACwrapper
{


#define PREPARE_FUNCTION_POINTERS                   \
    FLAC_available = false;                         \
    FLAChandle = dlopen("libFLAC.so",RTLD_LAZY);    \
    if(!FLAChandle) {                               \
        fprintf(stderr,"Failed to open libFLAC\n"); \
    }                                               \
    else {                                          \
        FLACversion = (const char **) dlsym(FLAChandle, "FLAC__VERSION_STRING"); \
        local_FLAC_stream_decoder_new = (FLAC__StreamDecoder * (*)()) dlsym(FLAChandle, "FLAC__stream_decoder_new"); \
        local_FLAC_stream_decoder_set_metadata_ignore_all = (FLAC__bool (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_set_metadata_ignore_all"); \
        local_FLAC_stream_decoder_set_metadata_respond_application = (FLAC__bool (*)(FLAC__StreamDecoder *, const FLAC__byte *)) dlsym(FLAChandle, "FLAC__stream_decoder_set_metadata_respond_application"); \
        local_FLAC_stream_decoder_init_file = (FLAC__StreamDecoderInitStatus (*)(FLAC__StreamDecoder *, const char *, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void *)) dlsym(FLAChandle, "FLAC__stream_decoder_init_file"); \
        local_FLAC_stream_decoder_process_until_end_of_metadata = (FLAC__bool (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_process_until_end_of_metadata"); \
        local_FLAC_stream_decoder_process_single = (FLAC__bool (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_process_single"); \
        local_FLAC_stream_decoder_finish = (FLAC__bool (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_finish"); \
        local_FLAC_stream_decoder_reset = (FLAC__bool (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_reset"); \
        local_FLAC_stream_decoder_delete = (void (*)(FLAC__StreamDecoder *)) dlsym(FLAChandle, "FLAC__stream_decoder_delete"); \
        local_FLAC_metadata_object_delete = (void (*)(FLAC__StreamMetadata *)) dlsym(FLAChandle, "FLAC__metadata_object_delete"); \
        if(dlerror() == NULL)  {                    \
            FLAC_decoder = local_FLAC_stream_decoder_new(); \
            if(FLAC_decoder != NULL)                \
                    FLAC_available = true;          \
        }                                           \
    }

// ***************************************************************************
// Constructor/Destructor
// ***************************************************************************

//---------------------------------------------------------------------------
File::File()
{
    File_Handle = ZenLib::File();
    PREPARE_FUNCTION_POINTERS
}

File::File(ZenLib::Ztring File_Name, ZenLib::File::access_t Access)
{
    File_Handle = ZenLib::File(File_Name, Access);
    PREPARE_FUNCTION_POINTERS
}


//---------------------------------------------------------------------------
File::~File()
{
    if(callbackdata.chunkbuffer != NULL)
        free(callbackdata.chunkbuffer);
    dlclose(FLAChandle);
}

// ***************************************************************************
// Open/Close
// ***************************************************************************

//---------------------------------------------------------------------------
bool File::Open (const ZenLib::tstring &File_Name_, ZenLib::File::access_t Access)
{
    ZenLib::FileName filename = File_Name_;
    if(filename.Extension_Get() == ZenLib::Ztring("flac")) {
        /* FLAC parsing */
        FLAC__byte rifftag[4] = {'r','i','f','f'};

        if(!FLAC_available)
            return false;
        if(!local_FLAC_stream_decoder_set_metadata_ignore_all(FLAC_decoder))
            return false;
        if(!local_FLAC_stream_decoder_set_metadata_respond_application(FLAC_decoder,rifftag))
            return false;
        if(local_FLAC_stream_decoder_init_file(FLAC_decoder, ZenLib::Ztring(File_Name_).To_UTF8().c_str(), FLAC_WriteCallback, FLAC_MetadataCallback, FLAC_ErrorCallback, (void*)&callbackdata) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
            return false;
        if(!local_FLAC_stream_decoder_process_until_end_of_metadata(FLAC_decoder))
            return false;
        if(callbackdata.chunkbuffer == NULL || callbackdata.chunkbuffer_data_location == 0){
            // Haven't found data chunk, or any chunks at all
            return false;
        }
        return true;
    }
    else
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
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Read(Buffer, Buffer_Size_Max);

    if(callbackdata.chunkbuffer_readpointer < callbackdata.chunkbuffer_data_location) {
        // Data to be read is all in front of audio
        size_t read_size = callbackdata.chunkbuffer_data_location - callbackdata.chunkbuffer_readpointer;
        if(read_size > Buffer_Size_Max)
            read_size = Buffer_Size_Max;

        memcpy(Buffer, callbackdata.chunkbuffer + callbackdata.chunkbuffer_readpointer, read_size);
        callbackdata.chunkbuffer_readpointer += read_size;
        return read_size;
    }
    else if(callbackdata.chunkbuffer_readpointer < callbackdata.chunkbuffer_data_location + callbackdata.chunkbuffer_data_length) {
        // Data to be read is audio
        size_t read_size = callbackdata.chunkbuffer_data_location + callbackdata.chunkbuffer_data_length - callbackdata.chunkbuffer_readpointer;
        if(read_size > Buffer_Size_Max)
            read_size = Buffer_Size_Max;
        if(callbackdata.got_error)
            return 0;
        if(callbackdata.audiobuffer_readpointer == 0) {
            if(!local_FLAC_stream_decoder_process_single(FLAC_decoder)) {
                callbackdata.got_error = true;
                return 0;
            }
        }
        if(read_size <= callbackdata.audiobuffer_sizeinuse - callbackdata.audiobuffer_readpointer) {
            read_size = callbackdata.audiobuffer_sizeinuse - callbackdata.audiobuffer_readpointer;
            memcpy(Buffer, callbackdata.audiobuffer + callbackdata.audiobuffer_readpointer, read_size);
            callbackdata.audiobuffer_readpointer = 0;
            return read_size;
        }
        else {
            memcpy(Buffer, callbackdata.audiobuffer + callbackdata.audiobuffer_readpointer, read_size);
            callbackdata.audiobuffer_readpointer += read_size;
            return read_size;
        }
    }
    else {
        // Data to be read is behind audio
        size_t read_size = callbackdata.chunkbuffer_size + callbackdata.chunkbuffer_data_length - callbackdata.chunkbuffer_readpointer;
        if(read_size > Buffer_Size_Max)
            read_size = Buffer_Size_Max;

        memcpy(Buffer, callbackdata.chunkbuffer + callbackdata.chunkbuffer_readpointer - callbackdata.chunkbuffer_data_length, read_size);
        callbackdata.chunkbuffer_readpointer += read_size;
        return read_size;
    }
}

//---------------------------------------------------------------------------
size_t File::Write (const ZenLib::int8u* Buffer, size_t Buffer_Size)
{
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Write(Buffer, Buffer_Size);
    return false;
}

//---------------------------------------------------------------------------
bool File::Truncate (ZenLib::int64u Offset)
{
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Truncate(Offset);
    return false;
}

//---------------------------------------------------------------------------
size_t File::Write (const ZenLib::Ztring &ToWrite)
{
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Write(ToWrite);
    return false;
}

// ***************************************************************************
// Moving
// ***************************************************************************

//---------------------------------------------------------------------------
bool File::GoTo (ZenLib::int64s Position_ToMove, ZenLib::File::move_t MoveMethod)
{
    ZenLib::int64u position;
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.GoTo(Position_ToMove, MoveMethod);

    if(MoveMethod == ZenLib::File::move_t::FromBegin)
        position = Position_ToMove;
    else if(MoveMethod == ZenLib::File::move_t::FromCurrent)
        callbackdata.chunkbuffer_readpointer += Position_ToMove;
    else //if(MoveMethod == ZenLib::File::move_t::FromEnd)
        callbackdata.chunkbuffer_readpointer = (ZenLib::File::move_t)callbackdata.chunkbuffer_data_length + Position_ToMove;

    // Seeks halfway audio data are not supported
    if(position > callbackdata.chunkbuffer_data_location && position < (callbackdata.chunkbuffer_data_location + callbackdata.chunkbuffer_data_length))
        return false;
    else if(position > (callbackdata.chunkbuffer_data_length + callbackdata.chunkbuffer_size))
        return false;
    callbackdata.chunkbuffer_readpointer = position;
    return true;
}

//---------------------------------------------------------------------------
ZenLib::int64u File::Position_Get ()
{
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Position_Get();

    return callbackdata.chunkbuffer_readpointer;
}

// ***************************************************************************
// Attributes
// ***************************************************************************

//---------------------------------------------------------------------------
ZenLib::int64u File::Size_Get()
{
    if(callbackdata.chunkbuffer == NULL)
        return File_Handle.Size_Get();
    return callbackdata.chunkbuffer_size + callbackdata.chunkbuffer_data_length;
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
void FLAC_MetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    File::clientdata_t * callbackdata = (File::clientdata_t *)(client_data);
    void * temp_ptr = realloc(callbackdata->chunkbuffer,callbackdata->chunkbuffer_size + metadata->length - 4);
    (void) decoder;

    if(temp_ptr == NULL) {
        callbackdata->got_error = true;
        return;
    }
    callbackdata->chunkbuffer = (char *)temp_ptr;
    memcpy(callbackdata->chunkbuffer + callbackdata->chunkbuffer_size, metadata->data.application.data,metadata->length - 4);
    callbackdata->chunkbuffer_size += metadata->length - 4;
    if(callbackdata->chunkbuffer_data_location == 0)
        // Check whether this is the data chunk
        if(memcmp(metadata->data.application.data,"data",4) == 0) {
            callbackdata->chunkbuffer_data_location = callbackdata->chunkbuffer_size;
            callbackdata->chunkbuffer_data_length = (ZenLib::int64u)(metadata->data.application.data[4]) +
                                                    ((ZenLib::int64u)(metadata->data.application.data[5]) << 8) +
                                                    ((ZenLib::int64u)(metadata->data.application.data[6]) << 16) +
                                                    ((ZenLib::int64u)(metadata->data.application.data[7]) << 24);
        }
}

void FLAC_ErrorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data) 
{
    File::clientdata_t * callbackdata = (File::clientdata_t *)(client_data);
    (void) decoder;
    (void) status;

    callbackdata->got_error = true;
}

FLAC__StreamDecoderWriteStatus FLAC_WriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data)
{
    File::clientdata_t * callbackdata = (File::clientdata_t *)(client_data);
    ZenLib::int64u channels = frame->header.channels;
    ZenLib::int64u bps = frame->header.bits_per_sample;
    ZenLib::int64u blocksize = frame->header.blocksize;
    (void) decoder;
    if(callbackdata->audiobuffer_channels == 0 && callbackdata->audiobuffer_bps == 0) {
        // First frame
        callbackdata->audiobuffer_channels = channels;
        callbackdata->audiobuffer_bps = bps;
    }
    else if(callbackdata->audiobuffer_channels != channels || callbackdata->audiobuffer_bps == bps) {
        // Change of parameters mid-stream, which we don't handle
        callbackdata->got_error = true;
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if(blocksize * channels * (bps / 8) > callbackdata->audiobuffer_size) {
        void * temp_ptr = realloc(callbackdata->audiobuffer, blocksize * channels * (bps / 8));
        if(temp_ptr == NULL) {
            callbackdata->got_error = true;
            return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
        }
        callbackdata->audiobuffer = (char *)temp_ptr;
        callbackdata->audiobuffer_size = blocksize * channels * (bps / 8);
    }

    {
        char * audiobuffer = callbackdata->audiobuffer;
	if(bps > 8) {
           for(ZenLib::int64u i = 0; i < blocksize; i++)
               for(ZenLib::int64u j = 0; j < channels; j++) {
                   memcpy(audiobuffer, &buffer[j][i], (bps / 8));
                   audiobuffer += (bps/8);
               }
        }
        else {
           for(ZenLib::int64u i = 0; i < blocksize; i++)
               for(ZenLib::int64u j = 0; j < channels; j++) {
                   int tmp = buffer[j][i] + (1 << (bps-1));
                   memcpy(audiobuffer, &tmp, (bps / 8));
                   audiobuffer += (bps/8);
               }
        }
    }
    callbackdata->audiobuffer_sizeinuse = blocksize * channels * (bps / 8);
    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

} //namespace
