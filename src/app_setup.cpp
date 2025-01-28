#include "app_setup.hpp"

AppSetup::AppSetup(std::string conf_path)
{
    configuration_file_path = conf_path;
    commands_queue = new std::queue<std::string>;
}

AppSetup::~AppSetup()
{
    delete commands_queue;
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
    std::stringstream(conf_file.get()) >> conf_json;
    if (conf_json.is_null() || conf_json.empty() || conf_json.size() == 0)
    {
        std::cout << "Invalid configuration file . . ." << std::endl;
        return ERROR;
    }
    configuration = conf_json;

    // Read elements from configuration and setup private variables
    // TODO

    // Setup notification_server
    notification_server = SetupNotificationServer();
    if (!notification_server)
    {
        std::cout << "Couldn't setup Notifications server . . ." << std::endl;
        return ERROR;
    }
}

NotificationServer* AppSetup::SetupNotificationServer()
{
    return nullptr;
}

OutputCodes AppSetup::SetConfFilePath(std::string conf_path)
{
    return OK;
}

std::string AppSetup::GetConfFilePath()
{
    return configuration_file_path;
}

std::queue<std::string>* AppSetup::GetCmdQueue()
{
    return commands_queue;
}
