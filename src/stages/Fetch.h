#ifndef SRC_STAGES_FETCH_H_
#define SRC_STAGES_FETCH_H_

#include <cstdint>
#include <vector>

#include "../programstack/prgstack.h"

namespace stages{

class Fetch{
protected:
    static bool _mem_error, _instr_valid;
    static uint8_t  _f_stat, _icode, _ifun, _r_A, _r_B;
    static uint64_t _val_C,  _val_P, _pc,   _predPC;
    static std::vector<uint8_t> _instruction;

    static uint64_t SelectPC();
    static uint8_t  SetICode();
    static uint8_t  SetIFun();
    static uint8_t  SetRA();
    static uint8_t  SetRB();
    // static uint64_t SetValC();
    static uint64_t SetValP();
    static bool     NeedValC();
    static bool     NeedRegids();
    static bool     InstructionValid();
    static uint8_t  SetStat();
    static uint64_t PredPC();

public:
    static uint8_t  GetStat()   { return _f_stat; }
    static uint8_t  GetICode()  { return _icode; }
    static uint8_t  GetIFun()   { return _ifun; }
    static uint8_t  GetRA()     { return _r_A; }
    static uint8_t  GetRB()     { return _r_B; }
    static uint64_t GetValC()   { return _val_C; }
    static uint64_t GetValP()   { return _val_P; }
    static uint64_t GetPC()     { return _pc; }
    static uint64_t GetPredPC() { return _predPC; }

    static bool NeedBubble();
    static bool NeedStall();
    static bool Update(const prgstk::ProgramStack& input);
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_