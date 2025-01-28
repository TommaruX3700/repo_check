#include <iostream>
#include "headers/app_setup.hpp"

int main(int argc, char* argv[]) 
{
    std::cout << "Repo_checker started! State: " << OK << std::endl;

    std::string str_path;

    if (argc != 1)
        std::cout << "No configuration file provided, loading execution path . . ." << std::endl;
    else
    {
        auto path = argv[1];
        str_path = path;
        if (str_path.empty()) 
            std::cout << "No configuration file provided, loading execution path . . ." << std::endl;
        else
            std::cout << "Configuration file provided at " << str_path << std::endl;
    }
    
    AppSetup* setup;
    if (setup->StartSetup(".") != OK)
        std::cout << "Unable to setup repository_watcher, check configuration.json file at " << str_path << std::endl;
    else
        std::cout << "Setup done!";
    
    /*
        - Start watcher loop
            > Start notification_server which will produce notification objects, execute them and release them in the end.
            > Start timer on async thread, and each configuration_time timeout, trigger the rest
            > Each time it triggers, start async thread to do repository check and cmds
    */

    std::cout << "Shutting down service . . ." << std::endl;
    return 0;
}