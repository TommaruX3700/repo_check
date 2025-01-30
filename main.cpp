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
    
    AppSetup* setup;
    setup = new AppSetup(str_path);

    if (setup->StartSetup() != OK)
        std::cout << "Unable to setup repository_watcher, check configuration.json file at " << str_path << std::endl;
    else
        std::cout << "Setup done!" << std::endl;
    
    /*
        - Start watcher loop
            > Start notification_server which will produce notification objects, execute them and release them in the end.
            > Start timer on async thread, and each configuration_time timeout, trigger the rest
            > Each time it triggers, start async thread to do repository check and cmds
    */

    std::cout << "Shutting down service . . ." << std::endl;
    return 0;
}