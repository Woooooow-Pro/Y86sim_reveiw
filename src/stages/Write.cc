#include "Write.h"

#include <cstdint>
#include <string>

#include "register.h"
#include "../utility/utility.h"

namespace stages{

uint8_t Write::_stat;

bool Write::NeedStall (){
    auto W_stat = PipelineRegister::Get(WRITE, STAT);
    if (utility::ValueIsInArray(W_stat, {SADR, SINS, SHLT}))
        return true;
    return false;
}

bool Write::NeedBubble(){
    return false;
}

uint8_t Write::Update(){
    auto w_stat = PipelineRegister::Get(WRITE, STAT);
    auto dst_M = PipelineRegister::Get(WRITE, DST_M);
    auto dst_E = PipelineRegister::Get(WRITE, DST_E);
    auto val_M = PipelineRegister::Get(WRITE, VAL_M);
    auto val_E = PipelineRegister::Get(WRITE, VAL_E);
    Register::Set(dst_M, val_M);
    Register::Set(dst_E, val_E);
    if (w_stat == SBUB) w_stat = SAOK;
    return _stat = w_stat;
}

}  // namespace stages