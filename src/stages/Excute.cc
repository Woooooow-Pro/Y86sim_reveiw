#include "Excute.h"

#include <cstdint>

#include "../utility/utility.h"
#include "Memory.h"
#include "Decode.h"
#include "register.h"

namespace stages{

bool     Excute::_cnd = false;
uint8_t Excute::_e_stat;
uint8_t Excute::_icode;
uint8_t Excute::_ifun;
uint64_t Excute::_dst_E;
uint64_t Excute::_dst_M;
uint64_t Excute::_val_A;
uint64_t Excute::_val_E;

uint64_t  Excute::SetAluA(uint8_t E_icode){
    if (utility::ValueIsInArray(E_icode, {IRRMOVQ, IOPQ}))
        return PipelineRegister::Get(EXCUTE, VAL_A);
    if (utility::ValueIsInArray(E_icode, {IIRMOVQ, IRMMOVQ, IMRMOVQ}))
        return PipelineRegister::Get(EXCUTE, VAL_C);
    if (utility::ValueIsInArray(E_icode, {ICALL, IPUSHQ}))
        return -8ull;
    if (utility::ValueIsInArray(E_icode, {IRET, IPOPQ}))
        return 8ull;
    return 0ull;  // Other instructions don’t need ALU
}

uint64_t  Excute::SetAluB(uint8_t E_icode){
    if(utility::ValueIsInArray(E_icode, {IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ}))
        return PipelineRegister::Get(EXCUTE, VAL_B);
    return 0ull;
}

bool Excute::NeedSetCC (uint8_t E_icode){
    if (E_icode != IOPQ) return false;

    std::vector<uint8_t> array{SADR, SINS, SHLT};
    auto m_stat = Memory::GetStat();
    if ( utility::ValueIsInArray(m_stat, array)) return false;

    auto W_stat = PipelineRegister::Get(WRITE, STAT);
    if (utility::ValueIsInArray(static_cast<uint8_t>(W_stat), array)) return false;
    // if (E_icode == IOPQ &&
    //     !(utility::ValueIsInArray(m_stat, {SADR, SINS, SHLT})) &&
    //     !(utility::ValueIsInArray(W_stat, {SADR, SINS, SHLT})))
    //     return true;
    return true;
}

uint64_t Excute::SetDstE (uint8_t E_icode){
    if (E_icode == IRRMOVQ && !_cnd)
        return RNONE;
    return PipelineRegister::Get(EXCUTE, DST_E);
}

uint64_t Add(int64_t num1, int64_t num2, bool& of){
    int64_t result = num1 + num2;
    of = (num1 > 0 && num2 > 0 && result < 0)
            || (num1 < 0 && num2 < 0 && result >= 0);
    return result;
}

uint64_t Excute::SetValE (
    int64_t aluA, int64_t aluB, uint8_t E_icode, uint8_t E_ifun){
        int64_t val_e;
        bool of;
        if (E_icode == IOPQ){
            switch( E_ifun ){
            case 1: val_e = Add(-aluA, aluB, of); break;
            case 2: val_e = aluB & aluA; of = false; break;
            case 3: val_e = aluB ^ aluA; of = false; break;
            default: val_e = Add(aluA, aluB, of);
            }
        }
        else{
            val_e = Add(aluA, aluB, of);
        }
        if(NeedSetCC(E_icode)){
            ConditionCode::Set(ZF, !val_e);
            ConditionCode::Set(SF, val_e < 0);
            ConditionCode::Set(OF, of);
        }
        return static_cast<uint64_t>(val_e);
}

bool Excute::SetCnd (uint8_t E_ifun){
    auto zf     = ConditionCode::Get(ZF);
    auto sf     = ConditionCode::Get(SF);
    auto of     = ConditionCode::Get(OF);
    bool result = true;
    switch (E_ifun) {
    case C_LE: result = zf || (sf && !of) || (!sf && of); break;
    case C_L:  result = (sf && !of) || (!sf && of); break;
    case C_E:  result = zf; break;
    case C_NE: result = !zf; break;
    case C_GE: result = (sf && of) || (!sf && !of); break;
    case C_G:  result = !zf && ((sf && of) || (!sf && !of)); break;
    default: break;
    }
    return result;
}

bool Excute::NeedBubble(){
    auto E_icode = PipelineRegister::Get(EXCUTE, I_CODE);
    auto E_dstM = PipelineRegister::Get(EXCUTE, DST_M);
    auto d_srcA = Decode::GetSrcA();
    auto d_srcB = Decode::GetSrcB();
    return (E_icode == IJXX && !_cnd) ||
        utility::ValueIsInArray(E_icode, {IMRMOVQ, IPOPQ}) &&
        utility::ValueIsInArray(E_dstM, {d_srcA, d_srcB});
}

bool Excute::NeedStall(){
    return false; 
}

bool Excute::Update(){
    auto E_icode = _icode = PipelineRegister::Get(EXCUTE, I_CODE);
    auto E_ifun = _ifun = PipelineRegister::Get(EXCUTE, I_FUN);
    _val_A = PipelineRegister::Get(EXCUTE, VAL_A);
    _dst_M = PipelineRegister::Get(EXCUTE, DST_M);
    _e_stat = PipelineRegister::Get(EXCUTE, STAT);
    auto aluA = SetAluA(E_icode);
    auto aluB = SetAluB(E_icode);
    _val_E = SetValE(static_cast<int64_t>(aluA), static_cast<int64_t>(aluB), E_icode, E_ifun);
    _cnd = SetCnd(E_ifun);
    _dst_E = SetDstE(E_icode);
    return true;
}

}  // namespace stages