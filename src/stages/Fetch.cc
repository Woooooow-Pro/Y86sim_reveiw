#include "Fetch.h"

#include <cstdint>

#include "../programstack/prgstack.h"
#include "../utility/utility.h"
#include "Decode.h"
#include "register.h"

namespace stages{

bool     Fetch::_mem_error = false; 
bool     Fetch::_instr_valid = true;
uint8_t  Fetch::_f_stat;
uint8_t  Fetch::_icode;
uint8_t  Fetch::_ifun;
uint8_t  Fetch::_r_A;
uint8_t  Fetch::_r_B;
uint64_t Fetch::_val_C;
uint64_t Fetch::_val_P;
uint64_t Fetch:: _pc;
uint64_t Fetch::_predPC;
std::vector<uint8_t> Fetch::_instruction;

uint64_t Fetch::SelectPC (){
    auto M_icode = PipelineRegister::Get(MEMORY, I_CODE);
    auto M_Cnd   = PipelineRegister::Get(MEMORY, CND);
    auto M_valA  = PipelineRegister::Get(MEMORY, VAL_A);
    auto W_icode = PipelineRegister::Get(WRITE,I_CODE);
    auto W_valM  = PipelineRegister::Get(WRITE, VAL_M);

    if (M_icode == IJXX && !M_Cnd) return M_valA;
    if (W_icode == IRET)           return W_valM;

    return PipelineRegister::Get(FETCH, PRED_PC);
}

uint8_t Fetch::SetICode (){
    if (_mem_error) return INOP;
    return _instruction[0] >> 4;
}

uint8_t Fetch::SetIFun (){
    if (_mem_error) return 0xF;
    return _instruction[0] & 0xF;
}

bool Fetch::InstructionValid (){
    if(utility::ValueIsInArray(_icode, {INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
          IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ}))
          return true;
    return false;
}

uint8_t Fetch::SetStat (){
    if (_mem_error)      return SADR;
    if (!_instr_valid)    return SINS;
    if (_icode == IHALT) return SHLT;
    return SAOK;
}

bool Fetch::NeedRegids (){
    if (utility::ValueIsInArray(_icode, {
        IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ}))
        return true;
    return false;
}

bool Fetch::NeedValC (){
    if (utility::ValueIsInArray(_icode, {
        IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL}))
        return true;
    return false; 
}

uint8_t Fetch::SetRA (){
    return _instruction[1] >> 4;
}

uint8_t Fetch::SetRB (){
    return _instruction[1] &0xF;
}

uint64_t Fetch::PredPC (){
    if(utility::ValueIsInArray(_icode, {IJXX, ICALL}))
        return _val_C;
    return _val_P;
}

bool Fetch::NeedBubble (){
    return false;
}

bool Fetch::NeedStall (){
    auto E_icode = PipelineRegister::Get(EXCUTE, I_CODE);
    auto E_dstM = PipelineRegister::Get(EXCUTE, DST_M);
    auto D_icode = PipelineRegister::Get(DECODE, I_CODE);
    auto M_icode = PipelineRegister::Get(MEMORY, I_CODE);
    auto d_srcA = Decode::GetSrcA();
    auto d_srcB = Decode::GetSrcB();

    return utility::ValueIsInArray(E_icode, {IMRMOVQ, IPOPQ}) && 
        utility::ValueIsInArray(E_dstM, {d_srcA, d_srcB}) ||
        utility::ValueIsInArray(static_cast<uint64_t>(IRET), {D_icode, E_icode, M_icode});
}

bool Fetch::Update(const prgstk::ProgramStack& input){
    uint64_t pc = 0;
    _pc = SelectPC();
    _instruction = input.GetInstruction(_pc, _mem_error);
    _icode = SetICode();
    _ifun = SetIFun();
    ++pc;
    _r_A = _r_B = RNONE;
    bool Needregids = NeedRegids();
    bool NeedvalC = NeedValC();
    if(Needregids){
        _r_A = SetRA();
        _r_B = SetRB();
        ++pc;
    }
    _val_C = 0ull;
    if(NeedvalC){
        for (int i = 0; i < 8; ++i){
            _val_C |= _instruction[pc+i] << (i << 3);
        }
        pc += 8;
    }
    _val_P = _pc + pc;
    _predPC = PredPC();
    _instr_valid = InstructionValid();
    _f_stat = SetStat();

}

}  // namespace stages