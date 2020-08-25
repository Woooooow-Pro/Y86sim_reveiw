#ifndef SRC_STAGES_REGISTER_H_
#define SRC_STAGES_REGISTER_H_

#include <cstdint>
#include <fstream>
#include <vector>

namespace stages{

enum InstructionMap : uint8_t {
    IHALT   = 0x0,
    INOP    = 0x1,
    IRRMOVQ = 0x2,
    IIRMOVQ = 0x3,
    IRMMOVQ = 0x4,
    IMRMOVQ = 0x5,
    IOPQ    = 0x6,
    IJXX    = 0x7,
    ICALL   = 0x8,
    IRET    = 0x9,
    IPUSHQ  = 0xA,
    IPOPQ   = 0xB
};

enum RegisterMap : int {
    RAX   = 0x0,
    RCX   = 0x1,
    RDX   = 0x2,
    RBX   = 0x3,
    RSP   = 0x4,
    RBP   = 0x5,
    RSI   = 0x6,
    RDI   = 0x7,
    R8    = 0x8,
    R9    = 0x9,
    R10   = 0xA,
    R11   = 0xB,
    R12   = 0xC,
    R13   = 0xD,
    R14   = 0xE,
    RNONE = 0xF
};

enum PipelineRegisterMap : int {
    PRED_PC = 0x0,
    STAT    = 0x1,
    I_CODE  = 0x2,
    I_FUN   = 0x3,
    R_A     = 0x4,
    R_B     = 0x5,
    VAL_C   = 0x6,
    VAL_P   = 0x7,
    VAL_A   = 0x8,
    VAL_B   = 0x9,
    VAL_E   = 0xA,
    VAL_M   = 0xB,
    SRC_A   = 0xC,
    SRC_B   = 0xD,
    DST_E   = 0xE,
    DST_M   = 0xF,
    CND     = 0x10
};

enum ConditionCodeMap : int { 
    OF = 0x0, 
    SF = 0x1, 
    ZF = 0x2
};

enum StatusMap : uint8_t {
    SBUB = 0x0,
    SAOK = 0x1,
    SADR = 0x2,
    SINS = 0x3,
    SHLT = 0x4
};

enum StageMap : int {
    FETCH  = 0x0,
    DECODE = 0x1,
    EXCUTE = 0x2,
    MEMORY = 0x3,
    WRITE  = 0x4
};

// something very abstruct
// struct Change{
//     int _register_code;
//     uint64_t _old_value, _new_value;
//     Change(int reg_code = 0, uint64_t old_value_ = 0, uint64_t new_value_ = 0)
//         : _register_code(reg_code),
//           _old_value(old_value_),
//           _new_value(new_value_) {}
// };

// abstruct of all the Register in Register File
class Register{
protected:
    static const size_t            _Total = 16;
    static std::vector<uint64_t>   _data;
public:
    static int64_t Get(int reg_number) { return _data[reg_number]; }
    static bool Set(int reg_number, uint64_t value);
    static void Clear();
    static void Print(std::fstream& output);
};

class PipelineRegister{
protected:
    static const size_t _Total = 17;
    static const size_t _Total_Stages = 5;
    static std::vector<std::vector<uint64_t> > _data;
public:
    static uint64_t Get(int stg_num, int reg_num) { return _data[stg_num][reg_num]; }
    static bool Set(int stage_number, int register_number, uint64_t value);
    static bool Clear(int stg_num);
    static bool Print(int stg_num, std::fstream& output);
};

class ConditionCode{
protected:
    static const size_t _Total = 3;
    static std::vector<bool> _data;
public:
    static bool Get(int CC_num) { return _data[CC_num]; }
    static bool Set(int CC_num, bool value);
    static void Clear();
    static void Print(std::fstream& output);
};

}  // namespace stages

#endif  // SRC_STAGES_REGISTER_H_