
// Collects cmds in a queue (cmd_pool) and runs them
// need to be sequentially executed in main() thread because these operations are sensitive.

// Questa classe deve raccogliere i comandi, eseguirli, raccogliere log, spedirlo e restituire codici errore
// NON serve che operations controlli CMD::ExecOrder, in quanto i comandi gli sono già forniti ordinati (vedi AppSetup::PopulateCmdQueues());

#include "cmd.hpp"
#include "public.hpp"

class Operations
{
private:
    /* data */
public:
    Operations(/* args */);
    ~Operations();

    OutputCodes exec(std::queue<CMD*>* cmd_queue, TypeOfCmd isGit);
    
};


