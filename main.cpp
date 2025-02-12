#include <iostream>
#include "headers/app_setup.hpp"
#include "operations.hpp"

/*
TODO: 
    - Add exception handling
*/

int main(int argc, char* argv[]) 
{
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
            > handle closing keyboard keys (CTRL + C) and destroy proper resources
    */
    
    // Get git queue to execute every interactions.
    std::queue<CMD*>* git_cmd_queue = setup->GetGitCmdQueue();

    Operations* ops = new Operations();

    // main loop
    while (true)
    {
        ops->exec(setup->GetPreCmdQueue(), NOT_GIT);
        ops->exec(setup->GetGitCmdQueue(), GIT);
        ops->exec(setup->GetPostCmdQueue(), NOT_GIT);
        // questo basta e avanza senza la complessità inutile di una classe "timout"
        std::this_thread::sleep_for(std::chrono::seconds(STD_REFRESH_TIME_SECONDS));
    }

    CslMsg("Shutting down service . . .");

    delete setup;
    delete notification_server;
    return OK;
}