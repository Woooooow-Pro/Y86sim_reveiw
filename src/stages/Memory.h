#ifndef SRC_STAGES_MEMORY_H_
#define SRC_STAGES_MEMORY_H_

#include <cstdint>

namespace stages{

class Memory{
protected:
    static bool _dmem_error;
    static uint8_t _m_stat, _icode, _dst_E, _dst_M;
    static uint64_t _val_E, _val_M;

    static bool NeedRead(const uint8_t icode);
    static bool NeedWrite(const uint8_t icode);
    static uint8_t  MemoryStat();
    static uint64_t MemoryAddress(const uint8_t icode);
    static bool     WriteMemory  (uint64_t address, uint64_t value);
    static uint64_t ReadMemory   (uint64_t address);

public:
    static uint8_t  GetStat()  { return _m_stat; }
    static uint8_t  GetICode() { return _icode; }
    static uint8_t  GetDstE()  { return _dst_E; }
    static uint8_t  GetDstM()  { return _dst_M; }
    static uint64_t GetValE()  { return _val_E; }
    static uint64_t GetValM()  { return _val_M; }

    static bool NeedBubble();
    static bool NeedStall();

    static bool Update();

};

}  // stages

#endif