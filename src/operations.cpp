#include "operations.hpp"

Operations::Operations(/* args */)
{
}

Operations::~Operations()
{
}

OutputCodes Operations::exec(std::queue<CMD*>* cmd_queue, TypeOfCmd isGit)
{
    CmdOutput out;
    CMD* StoreInit;
    CMD* StoreRemoteAdd;

    if (isGit)
    {
        /*
            > CD
            > REV_PARSE
                SE FALSE -> GIT INIT + REMOTE_ADD
            > GIT PULL
                SE ERROR -> SEGNALA
        */
        while (cmd_queue->size())
        {
            if (cmd_queue->front()->GetExecOrder() == INIT)
            {
                StoreInit = cmd_queue->front();   
            }
            else if (cmd_queue->front()->GetExecOrder() == REMOTE_ADD)
            {
                StoreRemoteAdd = cmd_queue->front();
            }
            else if (cmd_queue->front()->GetExecOrder() == REV_PARSE)
            {
                out = cmd_queue->front()->Run();
                if (out.first != OK)
                {
                    if (StoreInit->Run().first != OK)
                    {
                        CslMsg("Can´t initiate proper git folder!");
                        return ERROR;
                    }
                    if (StoreRemoteAdd->Run().first != OK)
                    {
                        CslMsg("Can´t add remote!");
                        return ERROR;
                    }
                }
                CslMsg(out.second);
            }
            else if (cmd_queue->front()->GetExecOrder() == PULL)
            {
                out = cmd_queue->front()->Run();
                if (out.first != OK)
                {
                    return ERROR;
                }
                CslMsg(out.second);
            }
            else
            {
                CslMsg(cmd_queue->front()->Run().second);
            }
            cmd_queue->pop();
        }
    }
    else
    {
        while (cmd_queue->size())
        {
            CslMsg(cmd_queue->front()->Run().second);
            cmd_queue->pop();
        }
    }
    return OK;
}