#include "LoadRegister.h"

#include <cstdint>

#include "Fetch.h"
#include "Decode.h"
#include "Excute.h"
#include "Memory.h"
#include "Write.h"
#include "register.h"

namespace stages{

void LoadRegister::LoadFetch (){
    PipelineRegister::Set(FETCH, VAL_P, Fetch::GetPC());
    if (Fetch::NeedBubble())
        PipelineRegister::Clear(FETCH);
    else if (!Fetch::NeedStall()){
        PipelineRegister::Set(FETCH, PRED_PC, Fetch::GetPredPC());
    }
}

void LoadRegister::LoadDecode (){
    if (Decode::NeedBubble())
        PipelineRegister::Clear(DECODE);
    else if (!Decode::NeedStall()){
        PipelineRegister::Set(DECODE, STAT, Fetch::GetStat());
        PipelineRegister::Set(DECODE, I_CODE, Fetch::GetICode());
        PipelineRegister::Set(DECODE, I_FUN, Fetch::GetIFun());
        PipelineRegister::Set(DECODE, R_A, Fetch::GetRA());
        PipelineRegister::Set(DECODE, R_B, Fetch::GetRB());
        PipelineRegister::Set(DECODE, VAL_C, Fetch::GetValC());
        PipelineRegister::Set(DECODE, VAL_P, Fetch::GetValP());
    }
}

void LoadRegister::LoadExcute (){
    if(Excute::NeedBubble())
        PipelineRegister::Clear(EXCUTE);
    else if(!Excute::NeedStall()){
        PipelineRegister::Set(EXCUTE, STAT, Decode::GetStat());
        PipelineRegister::Set(EXCUTE, I_CODE, Decode::GetICode());
        PipelineRegister::Set(EXCUTE, I_FUN, Decode::GetIFun());
        PipelineRegister::Set(EXCUTE, VAL_C, Decode::GetValC());
        PipelineRegister::Set(EXCUTE, VAL_A, Decode::GetValA());
        PipelineRegister::Set(EXCUTE, VAL_B, Decode::GetValB());
        PipelineRegister::Set(EXCUTE, DST_E, Decode::GetDstE());
        PipelineRegister::Set(EXCUTE, DST_M, Decode::GetDstM());
        PipelineRegister::Set(EXCUTE, SRC_A, Decode::GetSrcA());
        PipelineRegister::Set(EXCUTE, SRC_B, Decode::GetSrcB());
    }
}

void LoadRegister::LoadMemory (){
    if (Memory::NeedBubble()){
        PipelineRegister::Clear(MEMORY);
        // if (Excute::NeedSetCC(PipelineRegister::Get(EXCUTE, I_CODE))) 
        //     ConditionCode::Clear();
    }
    else if (!Memory::NeedStall()){
        PipelineRegister::Set(MEMORY, STAT, Excute::GetStat());
        PipelineRegister::Set(MEMORY, I_CODE, Excute::GetICode());
        PipelineRegister::Set(MEMORY, CND, Excute::GetCnd());
        PipelineRegister::Set(MEMORY, VAL_E, Excute::GetValE());
        PipelineRegister::Set(MEMORY, VAL_A, Excute::GetValA());
        PipelineRegister::Set(MEMORY, DST_E, Excute::GetDstE());
        PipelineRegister::Set(MEMORY, DST_M, Excute::GetDstM());
    }
}

void LoadRegister::LoadWrite (){
    if (Write::NeedBubble())
        PipelineRegister::Clear(WRITE);
    else if (!Write::NeedStall()){
        PipelineRegister::Set(WRITE, STAT, Memory::GetStat());
        PipelineRegister::Set(WRITE, I_CODE, Memory::GetICode());
        PipelineRegister::Set(WRITE, VAL_E, Memory::GetValE());
        PipelineRegister::Set(WRITE, VAL_M, Memory::GetValM());
        PipelineRegister::Set(WRITE, DST_M, Memory::GetDstM());
        PipelineRegister::Set(WRITE, DST_E, Memory::GetDstE());
    }
}

void LoadRegister::LoadingAll (){
    LoadFetch();
    LoadDecode();
    LoadExcute();
    LoadMemory();
    LoadWrite();
}

}  // namespace stages