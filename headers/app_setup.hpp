#include "public.hpp"
#include "notification_server.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <chrono>
#include <filesystem>
#include <regex>


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
    std::string configuration_file_path; //ok
    nlohmann::json configuration; //ok
    std::string start_time; //ok

    std::string log_file_path, mqtt_server_address, refresh_time, local_folder_path, remote_repository; //ok
    std::chrono::seconds seconds_refresh_time; //ok
    
    std::queue<std::string>* pre_commands_queue; //TODO
    std::queue<std::string>* commands_queue; //TODO

    std::map<int, std::string> notification_levels; //TODO
    NotificationServer* notification_server; //TODO

    NotificationServer* SetupNotificationServer(); //ok
    OutputCodes GetAndApplyConfiguration(); //ok
    OutputCodes CreateLogFile(); //ok
    OutputCodes CheckAndCorrectRefreshTime(); //ok
    OutputCodes CheckLocalDir(); //ok
    OutputCodes CheckRemoteRepo(); //ok
    OutputCodes PopulateCmdQueues(); //TODO

public:
    AppSetup(std::string conf_path); //ok
    ~AppSetup(); //TODO

    OutputCodes StartSetup(); //TODO

    // Set&Get methods
    std::queue<std::string>* GetCmdQueue(); //ok
    std::string GetLocalFolderPath(); //ok
    std::string GetRemoteRepository(); //ok
};

