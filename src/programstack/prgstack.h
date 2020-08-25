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
    static std::vector<uint8_t> _memory_data;
    static std::map<uint64_t, std::vector<uint8_t> > _instructions;
    static uint64_t _max_address;
    // Capacity of the program stack is 8 KB.
    static const size_t Capacity = 1 << 13;

    static void PrintErrorMessageL(const int err_code);
    static void PrintErrorMessageM(const int err_code);

public:
    ProgramStack(const std::string scr) : _scr(scr) {}

    bool Load();
    static uint64_t Read(uint64_t address, size_t size = 8);
    static bool Write(const uint64_t& address, uint64_t value, size_t size = 8);
    std::vector<uint8_t> GetInstruction (uint64_t address, bool& error) const;

    static void PrintAllInstruction();
};

}  // namespace regstk

#endif  // SRC_PROGRAMSTACK_PRGSTACK_H_