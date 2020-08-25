#ifndef SRC_STAGES_WRITE_H_
#define SRC_STAGES_WRITE_H_

#include <cstdint>

namespace stages{

class Write{
protected:
    static uint8_t _stat;

public:
    static uint8_t Update();

    static bool NeedStall();
    static bool NeedBubble();

    static uint8_t GetStat() { return _stat; }

};

}  // namespace stages

#endif