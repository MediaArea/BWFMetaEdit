// BWF MetaEdit Riff - RIFF stuff for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Riff/Riff_Chunks.h"
//---------------------------------------------------------------------------

//***************************************************************************
// WAVE
//***************************************************************************

//---------------------------------------------------------------------------
void Riff_WAVE::Read_Internal ()
{
    //Integrity
    if (Global->WAVE)
        throw exception_valid("2 WAVE chunks");
    
    //Filling
    Global->WAVE=new Riff_Base::global::chunk_WAVE;
    Global->WAVE->Size_Original=Chunk.Content.Size;

    SUBS_BEGIN();
        SUB_ELEMENT(WAVE_fmt_);
        SUB_ELEMENT(WAVE_bext);
        SUB_ELEMENT(WAVE_data);
        SUB_ELEMENT(WAVE_ds64);
        SUB_ELEMENT(WAVE_INFO);
        SUB_ELEMENT(WAVE__PMX);
        SUB_ELEMENT(WAVE_axml);
        SUB_ELEMENT(WAVE_iXML);
        SUB_ELEMENT(WAVE_MD5_);
        SUB_ELEMENT(WAVE_cue_);
        SUB_ELEMENT(WAVE_adtl);
        SUB_ELEMENT(WAVE_CSET);
    SUBS_END();

    //Integrity
    if (Chunk.File_In_Position+Chunk.Header.Size+Chunk.Content.Size>Global->In.Size_Get())
        throw exception_valid(!Global->TruncatedChunks.str().empty()?"truncated ("+Global->TruncatedChunks.str()+")":"truncated");

    for (size_t Pos=0; Pos<Subs.size(); Pos++)
        switch (Subs[Pos]->Header_Name_Get())
        {
            case Elements::WAVE_filr :
            case Elements::WAVE_FILR :
            case Elements::WAVE_fllr :
            case Elements::WAVE_FLLR :
            case Elements::WAVE_junk :
            case Elements::WAVE_JUNK :
            case Elements::WAVE_junq :
            case Elements::WAVE_JUNQ :
            case Elements::WAVE_pad_ :
            case Elements::WAVE_PAD_ :
                                        //Removing padding chunks;
                                        Subs.erase(Subs.begin()+Pos); //Removing the chunk
                                        Pos--;
                                        break;
            case Elements::WAVE      :
                                        throw exception_valid("imbricated WAVE chunk");
                                        break;
            default                  :  break;
        }
}

//---------------------------------------------------------------------------
size_t Riff_WAVE::Insert_Internal (int32u Chunk_Name_Insert)
{
    Riff_Base* NewChunk;
    switch (Chunk_Name_Insert)
    {
        case Elements::WAVE_bext :  NewChunk=new Riff_WAVE_bext(Global); break;
        case Elements::WAVE_ds64 :  NewChunk=new Riff_WAVE_ds64(Global); break;
        case Elements::WAVE_INFO :  NewChunk=new Riff_WAVE_INFO(Global); break;
        case Elements::WAVE__PMX :  NewChunk=new Riff_WAVE__PMX(Global); break;
        case Elements::WAVE_axml :  NewChunk=new Riff_WAVE_axml(Global); break;
        case Elements::WAVE_iXML :  NewChunk=new Riff_WAVE_iXML(Global); break;
        case Elements::WAVE_MD5_ :  NewChunk=new Riff_WAVE_MD5_(Global); break;
        case Elements::WAVE_cue_ :  NewChunk=new Riff_WAVE_cue_(Global); break;
        case Elements::WAVE_adtl :  NewChunk=new Riff_WAVE_adtl(Global); break;
        case Elements::WAVE_CSET :  NewChunk=new Riff_WAVE_CSET(Global); break;
        default                  :  return Subs.size();
    }

    size_t Subs_Pos;
    switch (Chunk_Name_Insert)
    {
        case Elements::WAVE_bext :  Subs_Pos=Global->NewChunksAtTheEnd?(size_t)-1:Subs_Pos_Get(Elements::WAVE_fmt_); break;
        case Elements::WAVE_ds64 :  Subs_Pos=0                                                                     ; break;
        case Elements::WAVE_INFO :  Subs_Pos=Global->NewChunksAtTheEnd?(size_t)-1:Subs_Pos_Get(Elements::WAVE_fmt_); break;
        case Elements::WAVE__PMX :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_axml :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_iXML :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_MD5_ :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_cue_ :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_adtl :  Subs_Pos=(size_t)-1                                                            ; break;
        case Elements::WAVE_CSET :  Subs_Pos=(size_t)-1                                                            ; break;
        default                  :  return Subs.size();
    }

    NewChunk->Modify();
    if (!NewChunk->IsRemovable())
    {
        if (Subs_Pos<Subs.size())
        {
            Subs.insert(Subs.begin()+Subs_Pos+1, NewChunk); //First place after fmt, always
            return Subs_Pos+1;
        }
        else
        {
            Subs.push_back(NewChunk); //At the end
            return Subs.size()-1;
        }
    }
    else
    {
        delete NewChunk; //NewChunk=NULL;
        return Subs.size();
    }
}
