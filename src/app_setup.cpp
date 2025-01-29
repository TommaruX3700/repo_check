#include "app_setup.hpp"

AppSetup::AppSetup(std::string conf_path)
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    notification_levels.clear();
    configuration_file_path = conf_path;
    commands_queue = new std::queue<std::string>;
}

AppSetup::~AppSetup()
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    notification_levels.clear();
    while (commands_queue->size())
        commands_queue->pop();
    delete commands_queue;
    delete notification_server;
}

OutputCodes AppSetup::StartSetup()
{
    std::ifstream conf_file(configuration_file_path);
    if (!conf_file.is_open())
    {
        std::cout << "Unable to open configuration file . . ." << std::endl;
        return ERROR;
    }

    nlohmann::json conf_json;
    std::stringstream buffer;
    buffer << conf_file.rdbuf();
    conf_json = nlohmann::json::parse(buffer.str());
    if (conf_json.is_null() || conf_json.empty() || conf_json.size() == 0)
    {
        std::cout << "Invalid configuration file . . ." << std::endl;
        return ERROR;
    }
    configuration = conf_json;

    // Read elements from configuration and setup private variables
    if (GetConfiguration() == ERROR || GetConfiguration() == CRITICAL_ERROR)
    {
        std::cout << "Check configuration file format. . ." << std::endl;
        return ERROR;
    }

    // Setup notification_server
    notification_server = SetupNotificationServer();
    if (!notification_server)
    {
        std::cout << "Couldn't setup Notifications server . . ." << std::endl;
        return ERROR;
    }

    return OK;
}

OutputCodes AppSetup::GetConfiguration()
{
    // Check and get information from configuration file and return if its valid or not

    

    return OK;
}

OutputCodes AppSetup::PopulateCmdQueues()
{
    // Get all the cmds, sort and order them in the pre_commands_queue and the commands_queue, return if successfull or not
}

NotificationServer* AppSetup::SetupNotificationServer()
{
    // Setup correctlu the Notification Server and return the object if it was successfull
    NotificationServer* notification_server;
    notification_server = new NotificationServer;
    return notification_server;
}

std::queue<std::string>* AppSetup::GetCmdQueue()
{
    return commands_queue;
}

std::string AppSetup::GetLocalFolderPath()
{
    return local_folder_path;
}

std::string AppSetup::GetRemoteRepository()
{
    return remote_repository;
}
