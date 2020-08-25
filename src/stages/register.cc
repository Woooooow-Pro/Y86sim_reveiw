#include "register.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "../utility/utility.h"

namespace stages{

std::vector<uint64_t> Register::_data (_Total);
std::vector<bool>     ConditionCode::_data (_Total);
std::vector<std::vector<uint64_t> > PipelineRegister::_data (
    _Total_Stages, std::vector<uint64_t>(_Total));

static const std::vector<std::string> registername{
    "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI",
    "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "RNONE"};

static const std::vector<std::string> stage_name{
    "FETCH", "DECODE", "EXCUTE", "MEMORY", "WRITE" 
};

static const std::vector<std::string> pipeline_register_name{
    "PRED_PC", "STAT",  "I_CODE", "I_FUN", "R_A",   "R_B",
    "VAL_C",   "VAL_P", "VAL_A",  "VAL_B", "VAL_E", "VAL_M",
    "SRC_A",   "SRC_B", "DST_E",  "DST_M", "CND"
};

static const std::vector<std::string> conditioncode_name{"OF", "SF", "ZF"};

static const std::vector<std::vector<uint64_t> > stages_register{
    {PRED_PC},
    {STAT, I_CODE, I_FUN, R_A, R_B, VAL_C, VAL_P},
    {STAT, I_CODE, I_FUN, VAL_C, VAL_A, VAL_B, DST_E, DST_M, SRC_A, SRC_B},
    {STAT, I_CODE, CND, VAL_E, VAL_A, DST_E, DST_M},
    {STAT, I_CODE, VAL_E, VAL_M, DST_E, DST_M}
};

bool Register::Set(int reg_num, uint64_t new_value){
    if (reg_num >= _Total || reg_num < 0) return false;
    _data[reg_num] = new_value;
    return true;
}

void Register::Clear(){
    for (int i = 0; i < _Total; ++i) _data[i] = 0ull;
}

void Register::Print(std::fstream& output){
    output << "\n";
    output << std::string("=", 6) << " REGISTER FILE " << std::string("=", 6) << "\n";
    for (int i = 0; i < 16; ++i){
        if (i == RNONE) continue;
        output << "%" << registername[i] << ":";
        utility::SetOutputHexWidth(16);
        output << Get(i) << "\n";
    }
}

bool PipelineRegister::Set(
    int stage_number, int register_number, uint64_t value){
    if (stage_number < 0 || stage_number >= _Total_Stages || 
    register_number < 0 || register_number >= _Total) return false;
    _data[stage_number][register_number] = value;
    return true;
}

bool PipelineRegister::Clear(int stage_number){
    if ( stage_number < 0 || stage_number >= _Total_Stages) return false;

    auto reg = stages_register[stage_number];
    for (auto reg_num: reg)
        _data[stage_number][reg_num] = 0ull;
    _data[stage_number][I_CODE] = INOP;
    _data[stage_number][DST_E] = _data[stage_number][DST_M] = 
    _data[stage_number][SRC_A] = _data[stage_number][SRC_B] = 
    _data[stage_number][R_A] = _data[stage_number][R_B] = RNONE;
    return true;
}

bool PipelineRegister::Print(int stage_number, std::fstream& output){
    if ( stage_number < 0 || stage_number >= _Total_Stages) return false;

    output << "====== " << stage_name[stage_number] << " REGISTER ======\n";
    auto reg = stages_register[stage_number];
    for (auto reg_num: reg){
        output << pipeline_register_name[reg_num] << "\t= 0x";
        utility::SetOutputHexWidth(16);
        output << Get(stage_number, reg_num) << "\n";
    }
    return true;
}

bool ConditionCode::Set(int CC_num, bool value){
    if (CC_num < 0 || CC_num >= _Total) return false;
    _data[CC_num] = value;
    return true;
}

void ConditionCode::Clear(){
    for(int i = 0; i < _Total; ++i)
        _data[i] = false;
}

void ConditionCode::Print(std::fstream& output){
    output << std::string("=", 6) << " Condition Code " << std::string("=", 6) << "\n";
    for (int i = 0; i < _Total; ++i){
        output << conditioncode_name[i] << " = ";
        output << _data[i] << "\n";
    } 
}


}  // namespace stages