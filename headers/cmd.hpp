#include "public.hpp"

#include <iostream>
#include <cstdint>

class CMD
{
private:
    int priority;
    std::string CMD_text;
    bool has_output;
    std::string CMD_output;
    std::uintptr_t id;

public:
    CMD(int priority, std::string CMD_text, bool has_output);
    ~CMD();

    OutputCodes Run();
    int GetExecOrder();
    OutputCodes GetOutput();
};


