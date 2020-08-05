#include <cstdlib>
#include <fstream>
#include <iostream>

#include "programstack/argument.h"
#include "programstack/prgstack.h"
#include "utility/utility.h"

int main(int argc, char** argv){
    prgstk::Argument arg;
    arg.Load(argc,argv);
    prgstk::ProgramStack memory(arg.input_src());
    memory.Load();
    memory.PrintAllInstruction();
}