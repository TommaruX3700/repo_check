#include "public.hpp"
#include "notification_server.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>
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
    nlohmann::json configuration;

    std::string log_file_path, mqtt_server_address, refresh_time, local_folder_path, remote_repository;
    std::map<int, std::string> notification_levels;
    
    std::queue<std::string>* pre_commands_queue;
    std::queue<std::string>* commands_queue;

    NotificationServer* notification_server;

    NotificationServer* SetupNotificationServer();
    OutputCodes GetConfiguration();
    OutputCodes PopulateCmdQueues();
   
    // Set&Get methods

public:
    AppSetup(std::string conf_path);
    ~AppSetup();

    OutputCodes StartSetup();

    // Set&Get methods
    std::queue<std::string>* GetCmdQueue();

    // these two usable for the rest of the program
    std::string GetLocalFolderPath();
    std::string GetRemoteRepository();
};

