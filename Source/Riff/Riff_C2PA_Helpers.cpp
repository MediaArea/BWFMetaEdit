//---------------------------------------------------------------------------
#include "Riff/Riff_Base.h"
#include "Riff/Riff_C2PA_Helpers.h"

//---------------------------------------------------------------------------
#include "ThirdParty/c2pa/c2pa.h"
#include "ThirdParty/json/json.h"
#include <cstdlib>
#include <cstdio>

#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"

using namespace ZenLib;
using namespace std;


struct C2PA_FileStreamContext
{
    FILE* F;
};

//***************************************************************************
// Callbacks
//***************************************************************************

//---------------------------------------------------------------------------
static intptr_t C2PA_ReadCallback(StreamContext* Context, uint8_t* Data, intptr_t Len)
{
    if (!Context || !Data || Len < 0)
        return -1;

    C2PA_FileStreamContext* FileContext=reinterpret_cast<C2PA_FileStreamContext*>(Context);
    if (!FileContext->F)
        return -1;

    size_t ReadSize=fread(Data, 1, (size_t)Len, FileContext->F);
    if (ReadSize==0 && ferror(FileContext->F))
        return -1;

    return (intptr_t)ReadSize;
}

//---------------------------------------------------------------------------
static intptr_t C2PA_SeekCallback(StreamContext* Context, intptr_t Offset, C2paSeekMode Mode)
{
    if (!Context)
        return -1;

    C2PA_FileStreamContext* FileContext=reinterpret_cast<C2PA_FileStreamContext*>(Context);
    if (!FileContext->F)
        return -1;

#if defined(_WIN32)
    if (_fseeki64(FileContext->F, (__int64)Offset, Mode) != 0)
        return -1;

    __int64 Position = _ftelli64(FileContext->F);
    if (Position < 0)
        return -1;

    return (intptr_t)Position;
#else
    if (fseeko(FileContext->F, (off_t)Offset, Mode) != 0)
        return -1;

    off_t Position = ftello(FileContext->F);
    if (Position < 0)
        return -1;

    return (intptr_t)Position;
#endif
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void C2PA_Validate(Riff_Handler* Handler, Riff_Base::global* Global)
{
    if (!Handler || !Global || !Global->C2PA)
        return;

    string FileName=Global->File_Name.To_UTF8();
    if (FileName.empty())
        return;

    C2PA_FileStreamContext FileContext;
    FileContext.F=fopen(FileName.c_str(), "rb");
    if (!FileContext.F)
        return;

    C2paContext* Context=c2pa_context_new();
    if (!Context)
    {
        fclose(FileContext.F);
        return;
    }

    C2paReader* Reader=c2pa_reader_from_context(Context);
    if (Reader)
    {
        C2paStream* Stream=c2pa_create_stream(reinterpret_cast<StreamContext*>(&FileContext), C2PA_ReadCallback, C2PA_SeekCallback, NULL, NULL);
        if (Stream)
        {
            Reader=c2pa_reader_with_stream(Reader, "audio/wav", Stream);
            c2pa_release_stream(Stream);
        }
        else
        {
            c2pa_free(Reader);
            Reader = NULL;
        }
    }

    if (Reader)
    {
        char* ManifestJson=c2pa_reader_crjson(Reader);
        if (ManifestJson)
        {
            Global->C2PA->manifest=ManifestJson;
            c2pa_free(ManifestJson);
        }

        if (!Global->VerifyC2PA)
        {
            c2pa_free(Reader);
            c2pa_free(Context);
            fclose(FileContext.F);
            c2pa_unload();
            return;
        }

        char* DetailedJson=c2pa_reader_detailed_json(Reader);
        if (DetailedJson)
        {
            json_value_s* ParsedJson=json_parse(DetailedJson, strlen(DetailedJson));
            if (ParsedJson)
            {
                bool HasSuccess=false;
                bool HasSignatureStatus=false;
                bool HasSignatureValidated=false;

                if (ParsedJson->type==json_type_object)
                {
                    const json_object_s* RootJsonObject=json_value_as_object(const_cast<json_value_s*>(ParsedJson));
                    if (RootJsonObject)
                    {
                        for (const json_object_element_s* Element=RootJsonObject->start; Element; Element=Element->next)
                        {
                            string ElementName=string((Element->name && Element->name->string) ? Element->name->string : "");
                            if (ElementName.empty())
                                continue;

                            if (ElementName=="validation_state")
                            {
                                json_string_s* ElementValue=json_value_as_string(Element->value);
                                if (ElementValue && string(ElementValue->string)!="Invalid")
                                {
                                    HasSuccess=true;
                                    HasSignatureStatus=true;
                                    HasSignatureValidated=true;
                                }
                            }
                            else if (ElementName=="validation_results")
                            {
                                json_object_s* ValidationResults=json_value_as_object(Element->value);
                                if (!ValidationResults)
                                    continue;

                                for (const json_object_element_s* ValidationElement=ValidationResults->start; ValidationElement; ValidationElement=ValidationElement->next)
                                {
                                    string ValidationElementName=string((ValidationElement->name && ValidationElement->name->string) ? ValidationElement->name->string : "");
                                    if (ValidationElementName!="activeManifest")
                                        continue;

                                    json_object_s* ActiveManifest=json_value_as_object(ValidationElement->value);
                                    if (!ActiveManifest)
                                        continue;

                                    for (const json_object_element_s* StatusElement=ActiveManifest->start; StatusElement; StatusElement=StatusElement->next)
                                    {
                                        string StatusElementName=string((StatusElement->name && StatusElement->name->string) ? StatusElement->name->string : "");
                                        if (StatusElementName.empty() || StatusElementName!="failure")
                                            continue;
                                        
                                        json_array_s* StatusArray=json_value_as_array(StatusElement->value);
                                        if (!StatusArray)
                                            continue;

                                        for (const json_array_element_s* StatusItem=StatusArray->start; StatusItem; StatusItem=StatusItem->next)
                                        {
                                            string StatusItemElementCode;
                                            string StatusItemElementURL;
                                            string StatusItemElementExplanation;

                                            json_object_s* StatusItemObject=json_value_as_object(StatusItem->value);
                                            if (!StatusItemObject)
                                                continue;

                                            for (const json_object_element_s* StatusItemElement=StatusItemObject->start; StatusItemElement; StatusItemElement=StatusItemElement->next)
                                            {
                                                string StatusItemElementName=string((StatusItemElement->name && StatusItemElement->name->string) ? StatusItemElement->name->string : "");
                                                if (StatusItemElementName=="code")
                                                {
                                                    json_string_s* StatusCode=json_value_as_string(StatusItemElement->value);
                                                    if (StatusCode && StatusCode->string)
                                                        StatusItemElementCode=string(StatusCode->string);
                                                }
                                                else if (StatusItemElementName=="url")
                                                {
                                                    json_string_s* StatusURL=json_value_as_string(StatusItemElement->value);
                                                    if (StatusURL && StatusURL->string)
                                                        StatusItemElementURL=string(StatusURL->string);
                                                }
                                                else if (StatusItemElementName=="explanation")
                                                {
                                                    json_string_s* StatusExplanation=json_value_as_string(StatusItemElement->value);
                                                    if (StatusExplanation && StatusExplanation->string)
                                                        StatusItemElementExplanation=string(StatusExplanation->string);
                                                }
                                            }

                                            Handler->Errors << Global->File_Name.To_UTF8() << ": " << "C2PA validation failure, code: " << StatusItemElementCode << " url: " << StatusItemElementURL << " explanation: " << StatusItemElementExplanation << endl;
                                            Handler->PerFile_Error << "C2PA validation failure, code: " << StatusItemElementCode << " url: " << StatusItemElementURL << " explanation: " << StatusItemElementExplanation << endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (HasSuccess)
                    Global->C2PA->valid=true;

                if (HasSignatureStatus)
                    Global->C2PA->signatureValid=HasSignatureValidated;

                free(ParsedJson);
            }

            c2pa_free(DetailedJson);
        }

        c2pa_free(Reader);
    }

    c2pa_free(Context);
    fclose(FileContext.F);
}