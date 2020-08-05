#include "prgstack.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../utility/utility.h"

namespace prgstk{

bool ProgramStack::Load(){
    std::ifstream input;
    input.open(_scr, std::ifstream::in);
    if (!input){
        PrintErrorMessageL(1);
        return false;
    }

    std::vector<std::string> raw_data;
    for (std::string line; !input.eof();) {
        std::getline(input, line);
        raw_data.push_back(line);
    }
    input.close();

    for (const auto& line: raw_data){
        if (line.empty() || line[0] == ' ') continue;
        size_t colon = line.find(':');
        std::vector<uint8_t> instruction;
        for ( size_t i = colon+2; line[i] != ' '; i += 2){
            std::string code_str = line.substr(i, 2);
            uint8_t code = static_cast<uint8_t>(std::stoi(code_str, nullptr, 16));
            instruction.push_back(code);
            _memory_data.push_back(code);
        }
        if (instruction.empty()) continue;
        std::string str = line.substr(2, colon);
        uint64_t address = static_cast<uint64_t>(std::stoi(str, nullptr, 16));
        _max_address = address;
        _instructions.insert(std::make_pair(address, instruction));
    }
    return true;
}

uint64_t ProgramStack::Read(const uint64_t& address, size_t size){
    if (size > 8 || address < size){
        PrintErrorMessageM(1);
        return UINT64_MAX;
    }
    uint64_t result;
    for (int i = 0; i < size; ++i){
        result += _memory_data[address+i] << (i << 3);
    }
    return result;
}

bool ProgramStack::Write(
    const uint64_t& address, uint64_t value, size_t size){
    if (size > 8 || address + size < address){
        PrintErrorMessageM(2);
        return false;
    }
    for (int i = 0; i < size; ++i, value >>= 8)
        _memory_data[address+i] = value & 0xFF;
    return true;
}

void ProgramStack::PrintErrorMessageL (const int err_code = 0){
    std::cerr << "Loading Error";
    switch (err_code){
    case 1: std::cerr << "1: Can not open the file \"" << _scr << "\""; break;
    default: std::cerr << ": Oops! An unknow error ocurs!";
    }
}

void ProgramStack::PrintErrorMessageM (const int err_code = 0){
    std::cerr << "Program Stack Error";
    switch (err_code){
    case 1: std::cerr << "1: Out of Reading range"; break;
    case 2: std::cerr << "2: Out of Writing range"; break;
    default: std::cerr << ": Oops! An unknow error ocurs!";
    }
}

void ProgramStack::PrintAllInstruction(){
    std::fstream output;
    output.open("ProgramStack.txt", std::ofstream::out | std::ofstream::trunc);
    if (!output){
        std::cerr << "Can not output the instruction!\n";
        return;
    }
    output << "======================= ProgramStack =======================";
    output << "\n\n";
    output << "---------------------------\n"
           << "|*                       *|\n"
           << "|*      Instructions     *|\n" 
           << "|*                       *|\n"
           << "---------------------------\n";
    uint64_t address = 0ull;
    for (auto instruction: _instructions){
        output << "0x";
        utility::SetOutputHexWidth(3,output);
        output << instruction.first << ": ";
        utility::SetOutputHexWidth(2,output);
        for (auto code: instruction.second){
            output << static_cast<uint16_t>(code);
            address ++;
        }
        output << "\n";
    }

    output << "\n\n";
    output << "---------------------------\n"
           << "|*                       *|\n"
           << "|*          stack        *|\n" 
           << "|*                       *|\n"
           << "---------------------------\n";
    ++ address;
    utility::SetOutputHexWidth(2,output);
    for (int cnt = 0; address < _max_address; ++address, ++cnt){
        output << static_cast<uint16_t>(_memory_data[address]) << " ";
        if (cnt && !(cnt%20))
            output << "\n";
    }
}

} // namespace prgstk