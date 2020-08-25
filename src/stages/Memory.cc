#include "Memory.h"

#include <cstdint>

#include "../programstack/prgstack.h"
#include "../utility/utility.h"
#include "register.h"

namespace stages{

bool Memory::_dmem_error = false;
uint8_t Memory::_m_stat;
uint8_t Memory::_icode;
uint8_t Memory::_dst_E;
uint8_t Memory::_dst_M;
uint64_t Memory::_val_E;
uint64_t Memory::_val_M;

bool Memory::NeedRead ( const uint8_t icode){
    if (utility::ValueIsInArray(icode, {IMRMOVQ, IPOPQ, IRET}))
        return true;
    return false;
}

bool Memory::NeedWrite (const uint8_t icode){
    if (utility::ValueIsInArray(icode, {IRMMOVQ, IPUSHQ, ICALL}))
        return true;
    return false;
}

uint8_t Memory::MemoryStat (){
    if (_dmem_error)
        return SADR;
    return PipelineRegister::Get(MEMORY, STAT);
}

uint64_t Memory::MemoryAddress (const uint8_t icode){
    if (utility::ValueIsInArray(icode, {IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ}))
        return PipelineRegister::Get(MEMORY, VAL_E);
    if(utility::ValueIsInArray(icode, {IPOPQ, IRET}))
        return PipelineRegister::Get(MEMORY, VAL_A);
}

bool Memory::WriteMemory (uint64_t address, uint64_t value){
    return _dmem_error = !prgstk::ProgramStack::Write(address, value, 8);
}

uint64_t Memory::ReadMemory (uint64_t address){
    _dmem_error = address + 8 < address;
    return prgstk::ProgramStack::Read(address, 8);
}

bool Memory::NeedBubble(){
    auto W_stat = PipelineRegister::Get(WRITE, STAT);
    return utility::ValueIsInArray(_m_stat, {SADR, SINS, SHLT}) ||
        utility::ValueIsInArray(W_stat, {SADR, SINS, SHLT});
}

bool Memory::NeedStall(){
    return false;
}

bool Memory::Update(){
    _icode = PipelineRegister::Get(MEMORY, I_CODE);
    _dst_E = PipelineRegister::Get(MEMORY, DST_E);
    _dst_M = PipelineRegister::Get(MEMORY, DST_M);
    _val_E = PipelineRegister::Get(MEMORY, VAL_E);

    auto address = MemoryAddress(_icode);
    if(NeedRead(_icode)){
        _val_M = ReadMemory(address);
    }
    if(NeedWrite(_icode)){
        auto val_A = PipelineRegister::Get(MEMORY, VAL_A);
        WriteMemory(address, val_A);
    }
    _m_stat = MemoryStat();
    return true;
}

}   // stages