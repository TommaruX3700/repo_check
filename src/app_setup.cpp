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
    // Set application startup time    
    start_time = std::format("{:%T_%F}", std::chrono::system_clock::now());

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
    if (GetAndApplyConfiguration() == ERROR || GetAndApplyConfiguration() == CRITICAL_ERROR)
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

OutputCodes AppSetup::GetAndApplyConfiguration()
{
    // Check, get and apply information from configuration json and return if its valid or not
    log_file_path = (configuration["log_file_path"] == "") ? STANDARD_LOG_FOLDER : configuration["log_file_path"];
    if (CreateLogFile() == WARNING)
        std::cout << "Continuing without log file!";

    mqtt_server_address = (configuration["mqtt_server_address"] == "") ? STANDARD_MQTT_ADDRESS : configuration["mqtt_server_address"];

    refresh_time = (configuration["refresh_time"] == "") ? STANDARD_REFRESH_TIME : configuration["refresh_time"];
    if (CheckAndCorrectRefreshTime() == WARNING)
        std::cout << "Invalid refresh_time found in configuration.json, using " << STANDARD_REFRESH_TIME << "instead!" << std::endl;
    
    return OK;
}

OutputCodes AppSetup::CreateLogFile()
{
    std::filesystem::create_directories(log_file_path);

    log_file_path.append(start_time);
    log_file_path.append(STANDARD_LOG_FILETYPE);

    std::cout << "Writing logs to " << log_file_path << std::endl;

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

OutputCodes AppSetup::CheckAndCorrectRefreshTime()
{
    bool found_time = false;
    bool found_valid_sequence = false;

    std::string temp_num_found = "";
    char temp_time_found = ' ';
    int temp_seconds = 0;
    int multiplier = 1;

    seconds_refresh_time = std::chrono::seconds(0);

    for (char& c : refresh_time)
    {
        if (isdigit(c))
        {
            temp_num_found.append(1, c);
        } else if (isalpha(c))
        {
            temp_time_found = c;
            found_time = true;
            if (!temp_num_found.empty())
            {
                found_time = true;

                switch (c)
                {
                    case 's':
                        multiplier = 1;
                        break;
                    case 'm':
                        multiplier = 60;
                        break;
                    case 'h':
                        multiplier = 3600;
                        break;
                    case 'd':
                        multiplier = 86400;
                        break;
                    default:
                        break;
                }
                

                // std::stoi(temp_num_found);
            }
            
        }
        else
        {
            std::cout << "Character " << c << " not admitted in refresh_time";
            // export function to do conversion and sum STANDARD_REFRESH_TIME instead
            // seconds_refresh_time += 
            return WARNING;
        }
    }
    
    
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
