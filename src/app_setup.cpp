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
    
}

OutputCodes AppSetup::IsConfigurationOk()
{

}

NotificationServer* AppSetup::SetupNotificationServer()
{

}

OutputCodes AppSetup::SetConfFilePath(std::string conf_path)
{

}



std::string AppSetup::GetConfFilePath()
{

}

std::queue<std::string> AppSetup::GetCmdQueue()
{

}
