#include <iostream>
#include "headers/app_setup.hpp"

/*
TODO: 
    - Add exception handling
    - Make proper class to store all std::cout output and write them to a file to provide debug infos on program functionalities
*/

int main(int argc, char* argv[]) 
{
    std::cout << "Repo_checker started! State: " << OK << std::endl;

    std::string str_path;

    if (argc != 2)
    {
        std::cout << "No configuration file provided, loading local execution path . . ." << std::endl;
        str_path = STANDARD_CONFIG_PATH;
    }
    else
    {
        auto path = argv[1];
        str_path = path;
        if (str_path.empty()) 
            std::cout << "No configuration file provided, loading execution path . . ." << std::endl;
        else
            std::cout << "Configuration file provided at " << str_path << std::endl;
    }
    
    /*
        Start application setup:
            > check configuration.json file format and retrive infos
    */
    AppSetup* setup;
    setup = new AppSetup(str_path);

    if (setup->StartSetup() != OK)
    {
        std::cout << "Unable to setup repository_watcher, check configuration.json file at " << str_path << std::endl;
        return ERROR;
    }  
    else
        std::cout << "Setup done!" << std::endl;
    
    /*
        - Start watcher loop
            > Start notification_server which will listen to notifications (maybe bind them to std behaviours).
            > Start timer the same main.cpp thread, then wait till timeout and repeat loop
            > handle closing keys
            > do git ops as cmds
    */

    NotificationServer* notification_server = setup->GetNotificationServer();
    std::queue<CMD*>* cmd_queue = setup->GetCmdQueue();

    // main loop
    while (true)
    {
        std::queue<CMD*> loop_cmd_queue = *cmd_queue;

        // pre-cmds
        while (loop_cmd_queue.front()->GetExecOrder() < 0)
        {
            CMD* pre_cmd = loop_cmd_queue.front();
            loop_cmd_queue.pop();
            pre_cmd->Run();
            delete pre_cmd;
        }
        
        // git ops

        // post-cmds
        while (loop_cmd_queue.front())
        {
            CMD* cmd = loop_cmd_queue.front();
            loop_cmd_queue.pop();
            cmd->Run();
            delete cmd;
        }
    }

    std::cout << "Shutting down service . . ." << std::endl;

    delete setup;
    delete notification_server;
    delete cmd_queue;
    return OK;
}