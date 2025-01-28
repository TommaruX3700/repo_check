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
    std::queue<std::string> commands_queue;
    
    NotificationServer* SetupNotificationServer();
    
    OutputCodes IsConfigurationOk();

    // Set&Get methods
    OutputCodes SetConfFilePath(std::string conf_path);

public:
    AppSetup();
    ~AppSetup();

    OutputCodes StartSetup(std::string conf_path);

    // Set&Get methods
    std::string GetConfFilePath();
    std::queue<std::string> GetCmdQueue();
    
};

