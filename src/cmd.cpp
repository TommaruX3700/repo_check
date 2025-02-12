#include "cmd.hpp"
#include <array>

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

CmdOutput CMD::Run(std::string cmd)
{
    // Static cmd for line execution without return
    int output = std::system((cmd).c_str());
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");  // "r" mode to read output
    if (!pipe) {
        throw std::runtime_error("popen failed!");
    }

    // Read the output line by line
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    // Close the pipe
    fclose(pipe);
    CmdOutput out;
    out.first = static_cast<OutputCodes>(output);
    out.second = result;
    return out;
}

int CMD::GetExecOrder()
{
    return priority;
}

