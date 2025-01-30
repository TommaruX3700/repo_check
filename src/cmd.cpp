#include "cmd.hpp"

CMD::CMD(int priority_value, std::string _CMD_text, bool _has_output)
    : priority { priority_value }, CMD_text { _CMD_text }, has_output { _has_output }
{
    id = reinterpret_cast<std::uintptr_t>(this);
    std::cout << "Command " << id << " configured!" << std::endl;
}

CMD::~CMD()
{
    id = 0;
    priority = 0;
    CMD_text.clear();
}

OutputCodes CMD::Run()
{
    // TODO
    return OK;
}

int CMD::GetExecOrder()
{
    return priority;
}

OutputCodes CMD::GetOutput()
{
    // TODO
    return OK;
}