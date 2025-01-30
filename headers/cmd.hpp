#include "public.hpp"

#include <iostream>
#include <cstdint>

class cmd
{
private:
    int priority;
    std::string cmd_text;
    bool has_output;
    std::string cmd_output;
    std::uintptr_t id;

public:
    cmd(int priority, std::string cmd_text, bool has_output);
    ~cmd();

    OutputCodes Run();
    OutputCodes GetOutput();
};


