#include "operations.hpp"

Operations::Operations(/* args */)
{
}

Operations::~Operations()
{
}

OutputCodes Operations::exec(std::queue<CMD*>* cmd_queue, TypeOfCmd isGit)
{
    // Se git ops
    /*
        > CD
        > REV_PARSE
            SE FALSE -> GIT INIT + REMOTE_ADD
        > GIT PULL
            SE ERROR -> SEGNALA
    */
    return OK;
}