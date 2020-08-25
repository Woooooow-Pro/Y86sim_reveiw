#include "argument.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

namespace prgstk{

void Argument::PrintErrorMessage(const int err_code){
    std::cerr << "Argument Error";
    switch (err_code){
    case 1: std::cerr << "1: No Argument provided.\n"; break;
    case 2: std::cerr << "2: Invalid format of the file.\n"; break;
    case 3: std::cerr << "3: Input file doesn't exist.\n"; break;
    default: std::cerr << "Oops! An unknow error occurs!\n"; break;
    }
}

void Argument::HelpfulMessage(){
    std::cout
        << "Something which maybe helpful:\n"
        << "  input_file: <file_name>.yo\n"
        << "      Currently only .yo files are accepted.\n"
        << "  output_dir\n"
        << "      Optional. Current directory by default.\n";
}

bool Argument::SourceIsValid (const std::string& scr){
    size_t length = scr.length();
    if ( length <= 3 ) return false;
    if ( scr.substr( length - 3, 3 ) != ".yo") return false;
    return true;
}

bool Argument::SourceExists (const std::string& scr){
    if ( std::filesystem::exists( scr.c_str() ) ) return true;
    return false;
}

bool Argument::Load(int argc, char** argv){
    if (argc < 2) {
        PrintErrorMessage(1);
        HelpfulMessage();
        return false;
    }

    const std::string scr = argv[1];
    if ( !SourceIsValid( scr ) ){
        PrintErrorMessage(2);
        return false;
    }
    if ( !SourceExists( scr ) ){
        PrintErrorMessage(3);
        return false;
    }
    _input_src = scr;
    std::cout << "Loading Success! File: \"" << _input_src << "\"\n";

    std::string output = "./";
    if ( argc > 2 ) output = argv[2];
    _output_dir = output;
    std::cout << "Output directory set to: \"" << _output_dir << "\"\n";
    return true;
}

}  // namespace prgstk