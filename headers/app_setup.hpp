#include "public.hpp"
#include "notification_server.hpp"
#include "json.hpp"

#include <iostream>
#include <string>
#include <queue>

// Setup class for whole project startup

/*
TODO:
    > Check configuration file if is ok with its syntax
    > Setup public usable variables 
    > Setup "notification_server" object factory

NB:
    - Protect other methods
    - Manage resources correctly
*/

class AppSetup
{
private:
    std::string configuration_file_path;
    std::queue<std::string>* commands_queue;
    
    OutputCodes IsConfigurationOk();

    NotificationServer* SetupNotificationServer();
   
    // Set&Get methods
    OutputCodes SetConfFilePath(std::string conf_path);

public:
    AppSetup(std::string conf_path);
    ~AppSetup();

    OutputCodes StartSetup();

    // Set&Get methods
    std::string GetConfFilePath();
    std::queue<std::string> GetCmdQueue();
    
};

