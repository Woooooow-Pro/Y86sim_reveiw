#ifndef SRC_STAGES_EXCUTE_H_
#define SRC_STAGES_EXCUTE_H_

#include <cstdint>

namespace stages{

enum ConditionMap : int {
    C_AW = 0x0,
    C_LE = 0x1,
    C_L  = 0x2,
    C_E  = 0x3,
    C_NE = 0x4,
    C_GE = 0x5,
    C_G  = 0x6
};

class Excute{
protected:
    static bool     _cnd;
    static uint8_t  _e_stat, _icode, _ifun;
    static uint64_t _dst_E, _dst_M, _val_A, _val_E;

    static uint64_t  SetAluA(uint8_t E_icode);
    static uint64_t  SetAluB(uint8_t E_icode);
    static uint64_t  SetValE(int64_t aluA, int64_t aluB, uint8_t E_icode, uint8_t E_ifun);
    static uint64_t SetDstE(uint8_t E_icode);
    static bool     SetCnd(uint8_t E_ifun);

public:
    friend class LoadRegister;
    static uint8_t  GetStat()  { return _e_stat; }
    static uint8_t  GetICode() { return _icode; }
    static uint8_t  GetIFun()  { return _ifun; }
    static uint64_t GetDstE()  { return _dst_E; }
    static uint64_t GetDstM()  { return _dst_M; }
    static uint64_t GetValA()  { return _val_A; }
    static uint64_t GetValE()  { return _val_E; }
    static bool     GetCnd()   { return _cnd; }
    static bool     NeedSetCC(uint8_t E_icode);

    static bool NeedBubble();
    static bool NeedStall();

    static bool Update();

};

}  // namespace stages

#endif  // SRC_STAGES_EXCUTE_H_