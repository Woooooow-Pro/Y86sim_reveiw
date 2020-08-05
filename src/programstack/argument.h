#ifndef SRC_PROGRAMSTACK_ARGUMENT_
#define SRC_PROGRAMSTACK_ARGUMENT_

#include <string>

namespace prgstk{

// Collects the arguments that main() function gets, 
// and checks out the argument
class Argument{
protected:
    std::string _input_src;
    std::string _output_dir;

    static void PrintErrorMessage(const int err_code);
    static void HelpfulMessage();

    static bool SourceIsValid(const std::string& src);
    static bool SourceExists(const std::string& src);

public:
    bool Load(int argc, char** args);
    std::string input_src() const { return _input_src; }
    std::string output_dir() const { return _output_dir; }
};

}  // namespace prgstk

#endif  // SRC_PROGRAMSTACK_ARGUMENT_