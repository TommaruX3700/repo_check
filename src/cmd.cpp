#include "cmd.hpp"

cmd::cmd(int priority_value, std::string _cmd_text, bool _has_output)
    : priority { priority_value }, cmd_text { _cmd_text }, has_output { _has_output }
{
    id = reinterpret_cast<std::uintptr_t>(this);
    std::cout << "Command " << id << " configured!" << std::endl;
}

cmd::~cmd()
{
    id = 0;
    priority = 0;
    cmd_text.clear();
}

OutputCodes cmd::Run()
{
    // TODO
    return OK;
}

OutputCodes cmd::GetOutput()
{
    // TODO
    return OK;
}