#include <iostream>
#include "headers/app_setup.hpp"

/*
TODO: 
    - Add exception handling
    - Make proper class to store all std::cout output and write them to a file to provide debug infos on program functionalities
*/

int main(int argc, char* argv[]) 
{
    // Dichiaro prima Notification Server e inizio a fare cache dei messaggi
    NotificationServer* notification_server = new NotificationServer();

    CslMsg("Repo_checker started! State: " + OK);

    std::string str_path;

    if (argc != 2)
    {
        CslMsg("No configuration file provided, loading local execution path . . .");
        str_path = STANDARD_CONFIG_PATH;
    }
    else
    {
        auto path = argv[1];
        str_path = path;
        if (str_path.empty()) 
            CslMsg("No configuration file provided, loading execution path . . .");
        else
            CslMsg("Configuration file provided at " + str_path);
    }
    

    // Start application setup: check configuration.json file format and retrive infos
    AppSetup* setup;
    setup = new AppSetup(str_path, notification_server);

    if (setup->StartSetup() != OK)
    {
        CslMsg("Unable to setup repository_watcher, check configuration.json file at " + str_path);
        return ERROR;
    }  
    else
        CslMsg("Setup done!");
    
    /*
        - Start watcher loop
            ok > Start notification_server which will listen to notifications (maybe bind them to std behaviours).
            ok > Start timer the same main.cpp thread, then wait till timeout and repeat loop
            > handle closing keyboard keys
            > do git ops as cmds

        TODO:
            > implement std::cout "override" to handle both notification and cmd stream
    */
    
    std::queue<CMD*>* cmd_queue = setup->GetCmdQueue();
    // Get git queue to execute every interactions.
    // std::queue<CMD*>* git_cmd_queue = setup->GetGitCmdQueue();

    // main loop
    while (true)
    {
        std::queue<CMD*> loop_cmd_queue = *cmd_queue;

        // pre-cmds
        // operations.exec(setup.getPreCMDQueue);

        while (loop_cmd_queue.front()->GetExecOrder() < 0)
        {
            CMD* pre_cmd = loop_cmd_queue.front();
            loop_cmd_queue.pop();
            pre_cmd->Run();
            delete pre_cmd;
        }
        
        // git ops
        // operations.exec(gitCMDQueue);

        // post-cmds
        // operations.exec(setup.getCMDQueue);
        while (loop_cmd_queue.front())
        {
            CMD* cmd = loop_cmd_queue.front();
            loop_cmd_queue.pop();
            cmd->Run();
            delete cmd;
        }

        // questo basta e avanza senza la complessità inutile di una classe "timout"
        std::this_thread::sleep_for(std::chrono::seconds(STD_REFRESH_TIME_SECONDS));
    }

    CslMsg("Shutting down service . . .");

    delete setup;
    delete notification_server;
    delete cmd_queue;
    return OK;
}