#include "cmd.hpp"

CMD::CMD(int priority_value, std::string _CMD_text, bool _has_output)
    : priority { priority_value }, CMD_text { _CMD_text }, has_output { _has_output }
{
    id = reinterpret_cast<std::uintptr_t>(this);
    CslMsg("Command " + std::to_string(id) + " configured!");
}

CMD::~CMD()
{
    id = 0;
    priority = 0;
    CMD_text.clear();
}

CmdOutput CMD::Run()
{
    // Execute command and return error_code and verbose output if present
    CmdOutput sample(OK, "aa");
    return sample;
}

int CMD::GetExecOrder()
{
    return priority;
}

