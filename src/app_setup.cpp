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
    conf_file.close();

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

    // Set application startup time
    auto now = std::chrono::system_clock::now();
    std::time_t now_to_time = std::chrono::system_clock::to_time_t(now);
    start_time = *std::localtime(&now_to_time);

    return OK;
}

OutputCodes AppSetup::GetConfiguration()
{
    if (CreateLogFile() == WARNING)
        std::cout << "Continuing without log file!";

    
    

    return OK;
}

OutputCodes AppSetup::CreateLogFile()
{
    // Check and get information from configuration file and return if its valid or not
    log_file_path = (configuration["log_file_path"] == "") ? STANDARD_LOG_FOLDER : configuration["log_file_path"];
    std::filesystem::create_directories(log_file_path);
    
    std::ostringstream time_string_stream;
    time_string_stream << std::put_time(&start_time, "%H:%M-%d_%m_%Y");

    log_file_path.append(time_string_stream.str());
    log_file_path.append(STANDARD_LOG_FILETYPE);

    std::ofstream log_file(log_file_path);
    if (!log_file.is_open())
    {
        std::cout << "Couldn't create log file!";
        log_file.close();
        return WARNING;
    }
        
    log_file.close();
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
