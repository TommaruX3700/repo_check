#include "app_setup.hpp"

AppSetup::AppSetup(std::string conf_path)
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    configuration_file_path = conf_path;
    commands_queue = new std::queue<CMD*>;
}

AppSetup::~AppSetup()
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    while (commands_queue->size())
        commands_queue->pop();
    delete commands_queue;
    delete notification_server;
}

OutputCodes AppSetup::StartSetup()
{
    /*
        TODO:
        - start notification server as soon as possible and drop there all previus 
            cached std::cout, according to NotificationLevel
        - use set methods to update later notification server attributes
    */

    // Set application startup time    
    start_time = GetFormattedTime("%T_%F");

    std::ifstream conf_file(configuration_file_path);
    if (!conf_file.is_open())
    {
        
        CslMsg("Unable to open configuration file . . .");
        return ERROR;
    }

    nlohmann::json conf_json;
    std::stringstream buffer;
    buffer << conf_file.rdbuf();
    conf_json = nlohmann::json::parse(buffer.str());
    conf_file.close();

    if (conf_json.is_null() || conf_json.empty() || conf_json.size() == 0)
    {
        CslMsg("Invalid configuration file . . .");
        return ERROR;
    }
    configuration = conf_json;

    // Read elements from configuration and setup private variables
    if (GetAndApplyConfiguration() == ERROR || GetAndApplyConfiguration() == CRITICAL_ERROR)
    {
        CslMsg("Generic error on configuration.json, check file format. . .");
        return ERROR;
    }  

    // Setup notification_server
    notification_server = SetupNotificationServer();
    if (!notification_server)
    {
        CslMsg("Couldn't setup Notifications server . . .");
        return ERROR;
    }

    return OK;
}

OutputCodes AppSetup::GetAndApplyConfiguration()
{
    // Check, get and apply information from configuration json and return if its valid or not
    log_file_path = (configuration["log_file_path"] == "") ? STANDARD_LOG_FOLDER : configuration["log_file_path"];
    if (CreateLogFile() == WARNING)
        CslMsg("Continuing without log file!");

    mqtt_server_address = (configuration["mqtt_server_address"] == "") ? STANDARD_MQTT_ADDRESS : configuration["mqtt_server_address"];

    refresh_time = (configuration["refresh_time"] == "") ? STANDARD_REFRESH_TIME : configuration["refresh_time"];
    if (CheckAndCorrectRefreshTime() == WARNING)
        CslMsg("Invalid refresh_time found in configuration.json, using " + std::string(STANDARD_REFRESH_TIME) + " instead!");
    
    local_folder_path = (configuration["local_folder_path"] == "") ? "" : configuration["local_folder_path"];
    if(local_folder_path == "" || CheckLocalDir() != OK)
    {
        CslMsg("Can't continue without a valid local_folder_path!");
        return ERROR;
    }

    remote_repository = (configuration["remote_repository"] == "") ? "" : configuration["remote_repository"];
    if(remote_repository == "" || CheckRemoteRepo() != OK)
    {
        CslMsg("Can't continue without a valid remote_repository!");
        return ERROR;
    }

    if (PopulateCmdQueues() != OK)
    {
        CslMsg("Can't load user defined commands!");
        return ERROR;
    }

    if (CheckNotificationLevel() == WARNING)
    {
        CslMsg("Can't load user defined notification_level! Using 'info' instead");
        notification_level = INFO;
    }
    
    return OK;
}

OutputCodes AppSetup::CreateLogFile()
{
    std::filesystem::create_directories(log_file_path);

    log_file_path.append(start_time);
    log_file_path.append(STANDARD_LOG_FILETYPE);

    CslMsg("Writing logs to " + log_file_path);

    std::ofstream log_file(log_file_path);
    if (!log_file.is_open())
    {
        CslMsg("Couldn't create log file!");
        log_file.close();
        return WARNING;
    }
        
    log_file.close();
    return OK;
}

OutputCodes AppSetup::CheckAndCorrectRefreshTime()
{
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
            if (!temp_num_found.empty())
            {
                switch (c)
                {
                    case 's':
                        multiplier = SECONDS_MULTIPLIER;
                        break;
                    case 'm':
                        multiplier = MINUTES_MULTIPLIER;
                        break;
                    case 'h':
                        multiplier = HOURS_MULTIPLIER;
                        break;
                    case 'd':
                        multiplier = DAYS_MULTIPLIER;
                        break;
                    default:
                        break;
                }
                seconds_refresh_time += std::chrono::seconds(std::stoi(temp_num_found)*multiplier);
            }
        }
        else
        {
            CslMsg("Character " + std::string(1, c) + " not admitted in refresh_time");
            seconds_refresh_time += std::chrono::seconds(STD_REFRESH_TIME_SECONDS);
            return WARNING;
        }
    }
    return OK;
}

OutputCodes AppSetup::CheckLocalDir()
{
    // chheck if the local dir exists
    std::filesystem::directory_entry dir_to_check{local_folder_path};
    if (!dir_to_check.exists())
    {
        CslMsg("Couldn't verify if the local path exists!");
        return ERROR;
    }
    return OK;
}

OutputCodes AppSetup::CheckRemoteRepo()
{
    // TODO: rework cmds with objects
    if (std::system("git -v") != OK)
    {
        CslMsg("Git not found!");
        return CRITICAL_ERROR;
    }
    
    if (std::system(("git -C" + local_folder_path + " remote -v").c_str()) != OK)
    {
        CslMsg("No .git folder found! Trying to add remote. . .");
        CslMsg("Try 1 . . .");
        if (std::system(("git -C"+ local_folder_path + " remote add origin " + remote_repository).c_str()) != OK)
        {
            CslMsg("Couldn't add any remote from " + remote_repository);
            CslMsg("Try 2 . . .");
            if (std::system(("git -C" + local_folder_path + " init && git -C " + local_folder_path + " remote add origin " + remote_repository).c_str()) != OK)
            {
                CslMsg("Failed also to initialize repo, check remote_repository and local_folder_path!");
                return ERROR;
            }
        }
    }

    if(std::system(("git -C " + local_folder_path + " ls-remote").c_str()) != OK)
    {
        CslMsg("Any remotes are present!");
        return ERROR;
    }
    return OK;
}

OutputCodes AppSetup::CheckNotificationLevel()
{
    char m_notification_level_char = configuration["notification_level"].get<std::string>()[0];
    if (m_notification_level_char == ' ')
    {
        CslMsg("Couln't read notification_level character in configuration.json");
        return WARNING;
    }
    
    switch (m_notification_level_char)
    {
        case 'm':
            notification_level = MINIMAL;
            break;
        case 'i':
            notification_level = INFO;
            break;
        case 'd':
            notification_level = DEBUG;
            break;
        default:
            CslMsg("Invalid notification_level character in configuration.json.");
            return WARNING;
            break;
    }
    return OK;
}

OutputCodes AppSetup::PopulateCmdQueues()
{
    // Get all the CMDs, sort and order them in the pre_commands_queue and the commands_queue, return if successfull or not
    nlohmann::json json_commands = configuration["CMD"];
    CMD* newCMD;
    std::vector<CMD*> temp_CMD_vector; 

    // fill the vector in order
    for (nlohmann::json& j_CMD : json_commands)
    {
        newCMD = new CMD(j_CMD["exec_order"], j_CMD["command"], j_CMD["has_output"]);
        if (temp_CMD_vector.size() != 0)
        {
            std::vector<CMD*>::iterator iterator = temp_CMD_vector.begin();
            for (CMD* temp_CMD : temp_CMD_vector)
            {
                iterator++;
                if (temp_CMD->GetExecOrder() < newCMD->GetExecOrder())
                {
                    temp_CMD_vector.insert(iterator, newCMD);
                }
            }
        } else
            temp_CMD_vector.push_back(newCMD);
    }

    // convert the vector to queue
    for (CMD* vector_CMD : temp_CMD_vector)
        commands_queue->push(vector_CMD);
    
    return OK;
}

NotificationServer* AppSetup::SetupNotificationServer()
{
    // Setup correctly the Notification Server and return the object if it was successfull
    NotificationServer* notification_server;
    notification_server = new NotificationServer(log_file_path, mqtt_server_address, notification_level);
    return notification_server;
}

std::queue<CMD*>* AppSetup::GetCmdQueue()
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

NotificationServer* AppSetup::GetNotificationServer()
{
    return notification_server;
}

NotificationLevels AppSetup::GetBaseNotificationLevel()
{
    return notification_level;
}