#include "Decode.h"

#include <cstdint>

#include "../utility/utility.h"
#include "Excute.h"
#include "Memory.h"
#include "register.h"

namespace stages{

uint8_t  Decode::_d_stat; 
uint8_t Decode::_icode;
uint8_t Decode::_ifun;
uint64_t Decode::_val_C;
uint64_t Decode::_val_A;
uint64_t Decode::_val_B;
uint64_t Decode::_dst_E;
uint64_t Decode::_dst_M;
uint64_t Decode::_src_A;
uint64_t Decode::_src_B;

uint64_t Decode::SetValA (){
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    auto M_dstM = PipelineRegister::Get(MEMORY, DST_M);
    auto M_dstE = PipelineRegister::Get(MEMORY, DST_E);
    auto M_valE = PipelineRegister::Get(MEMORY, VAL_E);
    auto W_dstM = PipelineRegister::Get(WRITE, DST_M);
    auto W_dstE = PipelineRegister::Get(WRITE, DST_E);
    auto W_valM = PipelineRegister::Get(WRITE, VAL_M);
    auto W_valE = PipelineRegister::Get(WRITE, VAL_E);
    auto d_srcA = _src_A;
    
    if (utility::ValueIsInArray(D_icode , {ICALL, IJXX}))
        return PipelineRegister::Get(DECODE, VAL_P);
    if (d_srcA == Excute::GetDstE()) 
        return Excute::GetValE();
    if (d_srcA == M_dstM) return Memory::GetValM();
    if (d_srcA == M_dstE) return M_valE;
    if (d_srcA == W_dstM) return W_valM;
    if (d_srcA == W_dstE) return W_valE;

    return Register::Get(_src_A);
}

uint64_t Decode::SetValB(){
    auto d_srcB = _src_B;
    auto e_dstE = Excute::GetDstE();
    auto e_valE = Excute::GetValE();
    auto M_dstM = PipelineRegister::Get(MEMORY, DST_M);
    auto m_valM = Memory::GetValM();
    auto M_dstE = PipelineRegister::Get(MEMORY, DST_E);
    auto M_valE = PipelineRegister::Get(MEMORY, VAL_E);
    auto W_dstM = PipelineRegister::Get(WRITE, DST_M);
    auto W_valM = PipelineRegister::Get(WRITE, VAL_M);
    auto W_dstE = PipelineRegister::Get(WRITE, DST_E);
    auto W_valE = PipelineRegister::Get(WRITE, VAL_E);

    if (d_srcB == e_dstE) return e_valE;
    if (d_srcB == M_dstM) return m_valM;
    if (d_srcB == M_dstE) return M_valE;
    if (d_srcB == W_dstM) return W_valM;
    if (d_srcB == W_dstE) return W_valE;
    
    return Register::Get(_src_B);
}

uint64_t Decode::SetDstE(){
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    if (utility::ValueIsInArray(D_icode, {IRRMOVQ, IIRMOVQ, IOPQ}))
        return PipelineRegister::Get(DECODE, R_B);
    if (utility::ValueIsInArray(D_icode, {IPUSHQ, IPOPQ, ICALL, IRET}))
        return RSP;
    return RNONE;
}

uint64_t Decode::SetDstM(){
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    if (utility::ValueIsInArray(D_icode, {IMRMOVQ, IPOPQ}))
        return PipelineRegister::Get(DECODE, R_A);
    return RNONE;
}

uint64_t Decode::SetSrcA(){
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    if (utility::ValueIsInArray(D_icode, {IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ}))
        return PipelineRegister::Get(DECODE, R_A);
    if (utility::ValueIsInArray(D_icode, {IPOPQ, IRET}))
        return RSP;
    return RNONE;
}

uint64_t Decode::SetSrcB(){
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    if (utility::ValueIsInArray(D_icode, {IOPQ, IRMMOVQ, IMRMOVQ}))
        return PipelineRegister::Get(DECODE, R_B);
    if (utility::ValueIsInArray(D_icode, {IPUSHQ, IPOPQ, ICALL, IRET}))
        return RSP;
    return RNONE;
}

bool Decode::NeedBubble(){
    auto E_icode = PipelineRegister::Get(EXCUTE, I_CODE);
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    auto M_icode = PipelineRegister::Get(MEMORY, I_CODE);
    auto E_dstM =  PipelineRegister::Get(EXCUTE, DST_M);
    auto e_Cnd = Excute::GetCnd();
    auto d_srcA = Decode::GetSrcA();
    auto d_srcB = Decode::GetSrcB();
    return (E_icode == IJXX && !e_Cnd) ||
        !(utility::ValueIsInArray(E_icode, {IMRMOVQ, IPOPQ}) && 
        utility::ValueIsInArray(E_dstM, {d_srcA, d_srcB})) &&
        utility::ValueIsInArray(static_cast<uint64_t>(IRET), {D_icode, E_icode, M_icode});
}

bool Decode::NeedStall(){
    auto E_icode = PipelineRegister::Get(EXCUTE, I_CODE);
    auto E_dstM =  PipelineRegister::Get(EXCUTE, DST_M);
    auto d_srcA = Decode::GetSrcA();
    auto d_srcB = Decode::GetSrcB();
    return utility::ValueIsInArray(E_icode, {IMRMOVQ, IPOPQ}) && 
        utility::ValueIsInArray(E_dstM, {d_srcA, d_srcB});
}

bool Decode::Update(){
    _d_stat = PipelineRegister::Get(DECODE, STAT);
    _icode  = PipelineRegister::Get(DECODE, I_CODE);
    _ifun   = PipelineRegister::Get(DECODE, I_FUN);
    _val_C  = PipelineRegister::Get(DECODE, VAL_C);
    _dst_E  = SetDstE();
    _dst_M  = SetDstM();
    _src_A  = SetSrcA();
    _src_B  = SetSrcB();
    _val_A  = SetValA();
    _val_B  = SetValB();
    return true;
}

}  // namespace stages