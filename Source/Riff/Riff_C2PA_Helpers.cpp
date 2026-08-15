//---------------------------------------------------------------------------
#include "Riff/Riff_Base.h"
#include "Riff/Riff_C2PA_Helpers.h"

//---------------------------------------------------------------------------
#include "ThirdParty/json/json.h"
#include "c2pa.h"
#include <cstdlib>
#include <cstdio>

#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include "ZenLib/FileName.h"
#include "Common/Common_About.h"

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

//---------------------------------------------------------------------------
static intptr_t C2PA_WriteCallback(StreamContext* Context, const uint8_t* Data, intptr_t Len)
{
    if (!Context || !Data || Len < 0)
        return -1;

    C2PA_FileStreamContext* FileContext=reinterpret_cast<C2PA_FileStreamContext*>(Context);
    if (!FileContext->F)
        return -1;

    size_t WriteSize=fwrite(Data, 1, (size_t)Len, FileContext->F);
    if (WriteSize<(size_t)Len)
        return -1;

    return (intptr_t)WriteSize;
}

//---------------------------------------------------------------------------
static intptr_t C2PA_FlushCallback(StreamContext* Context)
{
    if (!Context)
        return -1;

    C2PA_FileStreamContext* FileContext=reinterpret_cast<C2PA_FileStreamContext*>(Context);
    if (!FileContext->F || fflush(FileContext->F)!=0)
        return -1;

    return 0;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
static string Json_EscapeString(const string& Value)
{
    string Result;
    Result.reserve(Value.size()+2);
    for (size_t Pos=0; Pos<Value.size(); Pos++)
    {
        char C=Value[Pos];
        switch (C)
        {
            case '"' : Result+="\\\""; break;
            case '\\': Result+="\\\\"; break;
            case '\n': Result+="\\n"; break;
            case '\r': Result+="\\r"; break;
            case '\t': Result+="\\t"; break;
            default:
                if ((unsigned char)C<0x20)
                {
                    char Buffer[8];
                    snprintf(Buffer, sizeof(Buffer), "\\u%04x", (unsigned char)C);
                    Result+=Buffer;
                }
                else
                    Result+=C;
        }
    }
    return Result;
}

//---------------------------------------------------------------------------
static string C2PA_DefaultClaimGeneratorInfo()
{
    string Version=NameVersion_Text();
    size_t Pos=Version.find_last_of(' ');
    string VersionNumber=Pos==string::npos?Version:Version.substr(Pos+1);
    return "[{\"name\":\"BWFMetaEdit\",\"version\":\""+Json_EscapeString(VersionNumber)+"\"}]";
}

//---------------------------------------------------------------------------
static string C2PA_BaseFileName(const string& FileName)
{
    return FileName::Name_Get(Ztring().From_UTF8(FileName)).To_UTF8();
}

//---------------------------------------------------------------------------
static void C2PA_ParseStatusArray(json_array_s* StatusArray, vector<string>& Messages)
{
    if (!StatusArray)
        return;

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

        ostringstream Message;
        Message << "code: " << StatusItemElementCode << " url: " << StatusItemElementURL << " explanation: " << StatusItemElementExplanation;
        Messages.push_back(Message.str());
    }
}

//---------------------------------------------------------------------------
void C2PA_Validate(Riff_Handler* Handler, Riff_Base::global* Global)
{
    if (!Handler || !Global || !Global->C2PA)
        return;

    //if (!c2pa_load())
    //    return;

    string FileName=Global->File_Name.To_UTF8();
    if (FileName.empty())
    {
        //c2pa_unload();
        return;
    }

    C2PA_FileStreamContext FileContext;
    FileContext.F=fopen(FileName.c_str(), "rb");
    if (!FileContext.F)
    {
        //c2pa_unload();
        return;
    }

    C2paContext* Context=c2pa_context_new();
    if (!Context)
    {
        fclose(FileContext.F);
        //c2pa_unload();
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
            //c2pa_unload();
            return;
        }

        char* DetailedJson=c2pa_reader_detailed_json(Reader);
        if (DetailedJson)
        {
            json_value_s* ParsedJson=json_parse(DetailedJson, strlen(DetailedJson));
            if (ParsedJson)
            {
                Global->C2PA->validationErrors.clear();
                Global->C2PA->validationWarnings.clear();

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
                                        if (StatusElementName=="failure")
                                            C2PA_ParseStatusArray(json_value_as_array(StatusElement->value), Global->C2PA->validationErrors);
                                        else if (StatusElementName=="informational")
                                            C2PA_ParseStatusArray(json_value_as_array(StatusElement->value), Global->C2PA->validationWarnings);
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

                for (size_t Pos=0; Pos<Global->C2PA->validationErrors.size(); Pos++)
                {
                    Handler->Errors << Global->File_Name.To_UTF8() << ": C2PA validation failure, " << Global->C2PA->validationErrors[Pos] << endl;
                    Handler->PerFile_Error << "C2PA validation failure, " << Global->C2PA->validationErrors[Pos] << endl;
                }
                for (size_t Pos=0; Pos<Global->C2PA->validationWarnings.size(); Pos++)
                {
                    Handler->Warnings << Global->File_Name.To_UTF8() << ": C2PA validation warning, " << Global->C2PA->validationWarnings[Pos] << endl;
                    Handler->PerFile_Warning << "C2PA validation warning, " << Global->C2PA->validationWarnings[Pos] << endl;
                }

                free(ParsedJson);
            }

            c2pa_free(DetailedJson);
        }

        c2pa_free(Reader);
    }

    c2pa_free(Context);
    fclose(FileContext.F);
    //c2pa_unload();
}

//---------------------------------------------------------------------------
void C2PA_Sign(Riff_Handler* Handler, Riff_Base::global* Global)
{
    if (!Handler || !Global || Handler->C2PA_SignManifestJson.empty())
        return;

    string FileName=Global->File_Name.To_UTF8();
    if (FileName.empty())
        return;

    if (Handler->C2PA_SignAlgorithm.empty() || Handler->C2PA_SignCertificate.empty() || Handler->C2PA_SignPrivateKey.empty())
    {
        Handler->Errors<<FileName<<": C2PA signing, missing algorithm, certificate or private key (see --C2PA-Sign-Algorithm=, --C2PA-Sign-Certificate= and --C2PA-Sign-Key=)"<<endl;
        Handler->PerFile_Error<<"C2PA signing, missing algorithm, certificate or private key (see --C2PA-Sign-Algorithm=, --C2PA-Sign-Certificate= and --C2PA-Sign-Key=)"<<endl;
        return;
    }

    //Parsing the manifest definition, only to check which optional top-level fields are already provided
    json_value_s* ManifestRoot=json_parse(Handler->C2PA_SignManifestJson.data(), Handler->C2PA_SignManifestJson.size());
    if (!ManifestRoot || ManifestRoot->type!=json_type_object)
    {
        Handler->Errors<<FileName<<": C2PA signing, the signing manifest is not a valid JSON object"<<endl;
        Handler->PerFile_Error<<"C2PA signing, the signing manifest is not a valid JSON object"<<endl;
        if (ManifestRoot)
            free(ManifestRoot);
        return;
    }
    json_object_s* ManifestObject=json_value_as_object(ManifestRoot);

    bool HasClaimGeneratorInfo=false, HasTitle=false, HasFormat=false;
    for (const json_object_element_s* Element=ManifestObject->start; Element; Element=Element->next)
    {
        string Name=(Element->name && Element->name->string)?Element->name->string:"";
        if (Name=="claim_generator_info")
            HasClaimGeneratorInfo=true;
        else if (Name=="title")
            HasTitle=true;
        else if (Name=="format")
            HasFormat=true;
    }
    bool ManifestWasEmpty=ManifestObject->length==0;
    free(ManifestRoot);

    //Defaulting fields not provided by the caller, inserted as text right after the manifest's opening brace
    string Defaults;
    if (!HasClaimGeneratorInfo)
        Defaults+="\"claim_generator_info\":"+C2PA_DefaultClaimGeneratorInfo()+",";
    if (!HasTitle)
        Defaults+="\"title\":\""+Json_EscapeString(C2PA_BaseFileName(FileName))+"\",";
    if (!HasFormat)
        Defaults+="\"format\":\"audio/x-wav\",";

    string ManifestJson=Handler->C2PA_SignManifestJson;
    if (!Defaults.empty())
    {
        if (ManifestWasEmpty)
            Defaults.resize(Defaults.size()-1); //Removing the trailing comma, nothing follows in the object
        ManifestJson.insert(ManifestJson.find('{')+1, Defaults);
    }

//    if (!c2pa_load())
 //   {
 //       Handler->Errors<<FileName<<": C2PA signing, unable to load the C2PA library"<<endl;
 //       Handler->PerFile_Error<<"C2PA signing, unable to load the C2PA library"<<endl;
 //       return;
 //   }

    //Signer
    C2paSignerInfo SignerInfo;
    SignerInfo.alg=Handler->C2PA_SignAlgorithm.c_str();
    SignerInfo.sign_cert=Handler->C2PA_SignCertificate.c_str();
    SignerInfo.private_key=Handler->C2PA_SignPrivateKey.c_str();
    SignerInfo.ta_url=Handler->C2PA_SignTA_URL.empty()?NULL:Handler->C2PA_SignTA_URL.c_str();

    C2paSigner* Signer=c2pa_signer_from_info(&SignerInfo);
    if (!Signer)
    {
        char* Error=c2pa_error();
        Handler->Errors<<FileName<<": C2PA signing, unable to create the signer: "<<(Error?Error:"")<<endl;
        Handler->PerFile_Error<<"C2PA signing, unable to create the signer: "<<(Error?Error:"")<<endl;
        if (Error)
            c2pa_free(Error);
        //c2pa_unload();
        return;
    }

    //Builder, configured with the manifest definition
    C2paContext* Context=c2pa_context_new();
    C2paBuilder* Builder=Context?c2pa_builder_from_context(Context):NULL;
    if (Builder)
        Builder=c2pa_builder_with_definition(Builder, ManifestJson.c_str());

    if (!Builder)
    {
        char* Error=c2pa_error();
        Handler->Errors<<FileName<<": C2PA signing, unable to configure the manifest builder: "<<(Error?Error:"")<<endl;
        Handler->PerFile_Error<<"C2PA signing, unable to configure the manifest builder: "<<(Error?Error:"")<<endl;
        if (Error)
            c2pa_free(Error);
        c2pa_free(Signer);
        if (Context)
            c2pa_free(Context);
        //c2pa_unload();
        return;
    }

    //Signing the just-written and closed file into a new temporary file
    string TempFileName=FileName+".c2pasign.tmp";
    C2PA_FileStreamContext SourceContext;
    SourceContext.F=fopen(FileName.c_str(), "rb");
    C2PA_FileStreamContext DestContext;
    DestContext.F=SourceContext.F?fopen(TempFileName.c_str(), "wb+"):NULL;

    if (!SourceContext.F || !DestContext.F)
    {
        Handler->Errors<<FileName<<": C2PA signing, unable to open the source or destination file"<<endl;
        Handler->PerFile_Error<<"C2PA signing, unable to open the source or destination file"<<endl;
        if (SourceContext.F)
            fclose(SourceContext.F);
        if (DestContext.F)
            fclose(DestContext.F);
        c2pa_free(Builder);
        c2pa_free(Signer);
        c2pa_free(Context);
        //c2pa_unload();
        return;
    }

    C2paStream* SourceStream=c2pa_create_stream(reinterpret_cast<StreamContext*>(&SourceContext), C2PA_ReadCallback, C2PA_SeekCallback, NULL, NULL);
    C2paStream* DestStream=c2pa_create_stream(reinterpret_cast<StreamContext*>(&DestContext), C2PA_ReadCallback, C2PA_SeekCallback, C2PA_WriteCallback, C2PA_FlushCallback);

    const unsigned char* ManifestBytes=NULL;
    int64_t Result=(SourceStream && DestStream)?c2pa_builder_sign(Builder, "audio/wav", SourceStream, DestStream, Signer, &ManifestBytes):-1;
    if (ManifestBytes)
        c2pa_free(ManifestBytes);

    if (SourceStream)
        c2pa_release_stream(SourceStream);
    if (DestStream)
        c2pa_release_stream(DestStream);
    fclose(SourceContext.F);
    fclose(DestContext.F);

    c2pa_free(Builder);
    c2pa_free(Signer);
    c2pa_free(Context);

    if (Result<0)
    {
        char* Error=c2pa_error();
        Handler->Errors<<FileName<<": C2PA signing failed: "<<(Error?Error:"")<<endl;
        Handler->PerFile_Error<<"C2PA signing failed: "<<(Error?Error:"")<<endl;
        if (Error)
            c2pa_free(Error);
        File::Delete(Ztring().From_UTF8(TempFileName));
        //c2pa_unload();
        return;
    }

    //c2pa_unload();

    //Replacing the original file with the signed one, mirroring Riff_Base::Write's temp-file rename
    if (!File::Delete(Global->File_Name) || !File::Move(Ztring().From_UTF8(TempFileName), Global->File_Name))
    {
        Handler->Errors<<FileName<<": C2PA signing, unable to replace the original file with the signed file"<<endl;
        Handler->PerFile_Error<<"C2PA signing, unable to replace the original file with the signed file"<<endl;
        return;
    }

    Handler->Information<<FileName<<": C2PA, signed"<<endl;
    Handler->PerFile_Information<<"C2PA, signed"<<endl;
}
