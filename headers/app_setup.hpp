#include "public.hpp"
#include "notification_server.hpp"
#include "cmd.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <chrono>
#include <vector>
#include <filesystem>

// Setup class for whole project startup

/*
TODO:
    > "CheckRemoteRepo()": refactor with "Cmd" class implementation to substitute all the cmds verbose commands with objects;
    > "StartSetup()": implement pre-log drop on notification_server at the ent of method.
*/

class AppSetup
{
private:
    std::string configuration_file_path; //to_test
    nlohmann::json configuration; //to_test
    std::string start_time; //to_test

    std::string log_file_path, mqtt_server_address, refresh_time, local_folder_path, remote_repository; //to_test
    std::chrono::seconds seconds_refresh_time; //to_test
    
    // Contains both pre-commands and normal ones: checked during runtime if there are any and executed untill .front().priority() >= 0
    std::queue<CMD*>* pre_commands_queue; //to_test
    std::queue<CMD*>* git_commands_queue; //to_test
    std::queue<CMD*>* post_commands_queue; //to_test

    NotificationLevels notification_level; //to_test
    NotificationServer* notification_server; //to_test

    OutputCodes GetAndApplyConfiguration(); //to_test
    OutputCodes CreateLogFile(); //to_test
    OutputCodes CheckAndCorrectRefreshTime(); //to_test
    OutputCodes CheckLocalDir(); //to_test
    OutputCodes CheckRemoteRepo(); //to_test
    OutputCodes CheckNotificationLevel(); //to_test
    OutputCodes PopulateCmdQueues(); //to_test

public:
    AppSetup(std::string conf_path, NotificationServer* notification_srv); //to_test
    ~AppSetup(); //to_test

    OutputCodes StartSetup(); //to_test

    // Set&Get methods
    std::queue<CMD*>* GetPreCmdQueue(); 
    std::queue<CMD*>* GetGitCmdQueue();
    std::queue<CMD*>* GetPostCmdQueue(); 

    std::string GetLocalFolderPath(); 
    std::string GetRemoteRepository();
    NotificationLevels GetBaseNotificationLevel();
};

