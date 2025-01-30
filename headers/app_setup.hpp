#include "public.hpp"
#include "notification_server.hpp"
#include "cmd.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <chrono>
#include <list>
#include <filesystem>

// Setup class for whole project startup

/*
TODO:
    > Check configuration file if is ok with its syntax
    > Setup public usable variables 
    > Setup "notification_server" object factory
    > implement "Cmd" class correctly and substiture all the cmds verbose commands

NB:
    - Protect other methods
    - Manage resources correctly
*/

class AppSetup
{
private:
    std::string configuration_file_path; //to_test
    nlohmann::json configuration; //to_test
    std::string start_time; //to_test

    std::string log_file_path, mqtt_server_address, refresh_time, local_folder_path, remote_repository; //to_test
    std::chrono::seconds seconds_refresh_time; //to_test
    
    std::queue<std::string>* pre_commands_queue; //TODO
    std::queue<std::string>* commands_queue; //TODO

    std::map<int, std::string> notification_levels; //TODO
    NotificationServer* notification_server; //TODO

    NotificationServer* SetupNotificationServer(); //to_test
    OutputCodes GetAndApplyConfiguration(); //to_test
    OutputCodes CreateLogFile(); //to_test
    OutputCodes CheckAndCorrectRefreshTime(); //to_test
    OutputCodes CheckLocalDir(); //to_test
    OutputCodes CheckRemoteRepo(); //to_test
    OutputCodes PopulateCmdQueues(); //TODO

public:
    AppSetup(std::string conf_path); //to_test
    ~AppSetup(); //TODO

    OutputCodes StartSetup(); //TODO

    // Set&Get methods
    std::queue<std::string>* GetCmdQueue(); 
    std::string GetLocalFolderPath(); 
    std::string GetRemoteRepository(); 
};

