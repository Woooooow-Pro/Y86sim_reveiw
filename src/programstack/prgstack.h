#ifndef SRC_PROGRAMSTACK_PRGSTACK_H_
#define SRC_PROGRAMSTACK_PRGSTACK_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace prgstk{

class ProgramStack{
protected:
    std::string _scr;
    std::vector<uint8_t> _memory_data;
    std::map<uint64_t, std::vector<uint8_t> > _instructions;
    uint64_t _max_address;
    // Capacity of the program stack is 8 KB.
    static const size_t Capacity = 1 << 13;

    void PrintErrorMessageL(const int err_code);
    void PrintErrorMessageM(const int err_code);

public:
    ProgramStack(const std::string scr) : _scr(scr) {}

    bool Load();
    uint64_t Read(const uint64_t& address, size_t size = 8);
    bool Write(const uint64_t& address, uint64_t value, size_t size = 8);

    void PrintAllInstruction();
};

}  // namespace regstk

#endif  // SRC_PROGRAMSTACK_PRGSTACK_H_