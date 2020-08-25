#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "programstack/argument.h"
#include "programstack/prgstack.h"
#include "stages/register.h"
#include "stages/Fetch.h"
#include "stages/Decode.h"
#include "stages/Excute.h"
#include "stages/Memory.h"
#include "stages/Write.h"
#include "stages/LoadRegister.h"
#include "utility/utility.h"

int main(int argc, char** argv){
    prgstk::Argument arg;
    if(!arg.Load(argc,argv)) exit(EXIT_FAILURE);
    prgstk::ProgramStack memory(arg.input_src());
    // std::cout << "prgstk::ProgramStack memory(arg.input_src())  DONE\n";
    memory.Load();
    // std::cout << "memory.Load(); DONE\n";
    memory.PrintAllInstruction();

    uint8_t stat = stages::SAOK;

    // force to end the infinite loop
    uint64_t cpu_clock                = 0ull;
    const uint64_t force_end = 1000ull;
    // Output the programmer-visible state
    std::fstream output;
    output.open("Register.txt", std::ofstream::out | std::ofstream::trunc);
    if (!output){
        std::cerr << "Can not output the instruction!\n";
        exit(EXIT_FAILURE);
    }
    output << std::hex;
    // std::cout << "DONE!\n";
    while(stat == stages::SAOK && cpu_clock <= force_end){
        stat = stages::Write::Update();
        stages::Memory::Update();
        stages::Excute::Update();
        stages::Decode::Update();
        stages::Fetch::Update(memory);

        stages::LoadRegister::LoadingAll();

        output << std::setfill('0') << std::setw(3);
        output << "-------------------------------------\n"
               << "|*                                 *|\n"
               << "|*               " << cpu_clock << "                *|\n"
               << "|*                                 *|\n"
               << "-------------------------------------\n";
        stages::Register::Print(output);
        output << "\n\n========== STAGE'S REGISTERS ==========\n";
        stages::PipelineRegister::Print(stages::FETCH, output);
        stages::PipelineRegister::Print(stages::DECODE, output);
        stages::PipelineRegister::Print(stages::EXCUTE, output);
        stages::PipelineRegister::Print(stages::MEMORY, output);
        stages::PipelineRegister::Print(stages::WRITE, output);
        output << "\n\n";
        stages::ConditionCode::Print(output);
        output << "\n\n\n";

        ++cpu_clock;
    }
    return 0;
}