#include "app_setup.hpp"

AppSetup::AppSetup(std::string conf_path)
{
    configuration_file_path = conf_path;
    commands_queue = new std::queue<std::string>;
}

AppSetup::~AppSetup()
{
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
    return OK;
}

NotificationServer* AppSetup::SetupNotificationServer()
{
    NotificationServer* notification_server;
    notification_server = new NotificationServer;
    return notification_server;
}

OutputCodes AppSetup::SetConfFilePath(std::string conf_path)
{
    return OK;
}

std::queue<std::string>* AppSetup::GetCmdQueue()
{
    return commands_queue;
}
