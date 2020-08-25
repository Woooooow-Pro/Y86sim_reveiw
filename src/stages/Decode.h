#ifndef SRC_STAGES_DECODE_H_
#define SRC_STAGES_DECODE_H_

#include <cstdint>

namespace stages{

class Decode{
protected:
    static uint8_t  _d_stat, _icode, _ifun;
    static uint64_t _val_C, _val_A, _val_B, _dst_E, _dst_M, _src_A, _src_B;
    static uint64_t SetValA();
    static uint64_t SetValB();
    static uint64_t SetDstE();
    static uint64_t SetDstM();
    static uint64_t SetSrcA();
    static uint64_t SetSrcB();

public:
    static uint8_t GetStat() { return _d_stat; }
    static uint8_t GetICode() { return _icode; }
    static uint8_t GetIFun() { return _ifun; }
    static uint64_t GetValC() { return _val_C; }
    static uint64_t GetValB() { return _val_B; }
    static uint64_t GetValA() { return _val_A; }
    static uint64_t GetDstE() { return _dst_E; }
    static uint64_t GetDstM() { return _dst_M; }
    static uint64_t GetSrcA() { return _src_A; }
    static uint64_t GetSrcB() { return _src_B; }

    static bool NeedBubble();
    static bool NeedStall();
    static bool Update();

};

}  // namespace stages

#endif  // SRC_STAGES_DECODE_H_