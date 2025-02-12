#include "public.hpp"

#include <iostream>
#include <cstdint>

// CMD class
// defines a cmd element

class CMD
{
private:

    // priority definisce in che ordine il comando deve venire eseguito.
    int priority;

    std::string CMD_text;
    bool has_output;
    std::string CMD_output;
    std::uintptr_t id;

public:
    CMD(int priority, std::string CMD_text, bool has_output);
    ~CMD();

    static CmdOutput Run(std::string cmd);
    CmdOutput Run();
    int GetExecOrder();
};


