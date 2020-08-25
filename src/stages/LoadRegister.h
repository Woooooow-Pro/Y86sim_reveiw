#ifndef SRC_STAGES_LOADREGISTER_H_
#define SRC_STAGES_LOADREGISTER_H_

#include <cstdint>

namespace stages{

class LoadRegister{
public:
    static void LoadingAll();

    static void LoadFetch();
    static void LoadDecode();
    static void LoadExcute();
    static void LoadMemory();
    static void LoadWrite();
};

}  // namespace stages

#endif  // SRC_STAGES_LOADREGISTER_H_