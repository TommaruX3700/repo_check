#include "app_setup.hpp"

AppSetup::AppSetup(std::string conf_path, NotificationServer* notification_srv)
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    configuration_file_path = conf_path;
    notification_server = notification_srv;
    pre_commands_queue = new std::queue<CMD*>;
    git_commands_queue = new std::queue<CMD*>;
    post_commands_queue = new std::queue<CMD*>;
}

AppSetup::~AppSetup()
{
    configuration.clear();
    log_file_path.clear();
    mqtt_server_address.clear();
    refresh_time.clear();
    while (pre_commands_queue->size())
        pre_commands_queue->pop();
    delete pre_commands_queue;
    while (git_commands_queue->size())
        git_commands_queue->pop();
    delete git_commands_queue;
    while (post_commands_queue->size())
        post_commands_queue->pop();
    delete post_commands_queue;
    delete notification_server;
}

OutputCodes AppSetup::StartSetup()
{
    /*
        TODO:
        - use set methods to update later notification server attributes
    */

    // Set application startup time    
    start_time = GetFormattedTime("%T_%F");

    std::ifstream conf_file(configuration_file_path);
    if (!conf_file.is_open())
    {
        
        NotificationServer::send("Unable to open configuration file . . .");
        return ERROR;
    }

    nlohmann::json conf_json;
    std::stringstream buffer;
    buffer << conf_file.rdbuf();
    conf_json = nlohmann::json::parse(buffer.str());
    conf_file.close();

    if (conf_json.is_null() || conf_json.empty() || conf_json.size() == 0)
    {
        NotificationServer::send("Invalid configuration file . . .");
        return ERROR;
    }
    configuration = conf_json;

    // Read elements from configuration and setup private variables
    if (GetAndApplyConfiguration() == ERROR || GetAndApplyConfiguration() == CRITICAL_ERROR)
    {
        NotificationServer::send("Generic error on configuration.json, check file format. . .");
        return ERROR;
    }  

    // Setup notification_server
    if (!notification_server)
    {
        NotificationServer::send("Couldn't setup Notifications server . . .");
        return ERROR;
    }
    notification_server->setLogFilePath(this->log_file_path);
    notification_server->setMqttAddress(this->mqtt_server_address);
    notification_server->setMinimumNotificationLevel(this->notification_level);

    return OK;
}

OutputCodes AppSetup::GetAndApplyConfiguration()
{
    // Check, get and apply information from configuration json and return if its valid or not
    log_file_path = (configuration["log_file_path"] == "") ? STANDARD_LOG_FOLDER : configuration["log_file_path"];
    if (CreateLogFile() == WARNING)
        NotificationServer::send("Continuing without log file!");

    mqtt_server_address = (configuration["mqtt_server_address"] == "") ? STANDARD_MQTT_ADDRESS : configuration["mqtt_server_address"];

    refresh_time = (configuration["refresh_time"] == "") ? STANDARD_REFRESH_TIME : configuration["refresh_time"];
    if (CheckAndCorrectRefreshTime() == WARNING)
        NotificationServer::send("Invalid refresh_time found in configuration.json, using " + std::string(STANDARD_REFRESH_TIME) + " instead!");
    
    local_folder_path = (configuration["local_folder_path"] == "") ? "" : configuration["local_folder_path"];
    if(local_folder_path == "" || CheckLocalDir() != OK)
    {
        NotificationServer::send("Can't continue without a valid local_folder_path!");
        return ERROR;
    }

    remote_repository = (configuration["remote_repository"] == "") ? "" : configuration["remote_repository"];
    if(remote_repository == "" || CheckRemoteRepo() != OK)
    {
        NotificationServer::send("Can't continue without a valid remote_repository!");
        return ERROR;
    }

    if (PopulateCmdQueues() != OK)
    {
        NotificationServer::send("Can't load user defined commands!");
        return ERROR;
    }

    if (CheckNotificationLevel() == WARNING)
    {
        NotificationServer::send("Can't load user defined notification_level! Using 'info' instead");
        notification_level = INFO;
    }
    
    return OK;
}

OutputCodes AppSetup::CreateLogFile()
{
    std::filesystem::create_directories(log_file_path);

    log_file_path.append(start_time);
    log_file_path.append(STANDARD_LOG_FILETYPE);

    NotificationServer::send("Writing logs to " + log_file_path);

    std::ofstream log_file(log_file_path);
    if (!log_file.is_open())
    {
        NotificationServer::send("Couldn't create log file!");
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
            NotificationServer::send("Character " + std::string(1, c) + " not admitted in refresh_time");
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
        NotificationServer::send("Couldn't verify if the local path exists!");
        return ERROR;
    }
    return OK;
}

OutputCodes AppSetup::CheckRemoteRepo()
{
    // TODO: rework cmds with objects
    if (CMD::Run("git -v").first != OK)
    {
        NotificationServer::send("Git not found!");
        return CRITICAL_ERROR;
    }
    
    if (CMD::Run(("git -C" + local_folder_path + " remote -v").c_str()).first != OK)
    {
        NotificationServer::send("No .git folder found! Trying to add remote. . .");
        NotificationServer::send("Try 1 . . .");
        if (CMD::Run(("git -C"+ local_folder_path + " remote add origin " + remote_repository).c_str()).first != OK)
        {
            NotificationServer::send("Couldn't add any remote from " + remote_repository);
            NotificationServer::send("Try 2 . . .");
            if (CMD::Run(("git -C" + local_folder_path + " init && git -C " + local_folder_path + " remote add origin " + remote_repository).c_str()).first != OK)
            {
                NotificationServer::send("Failed also to initialize repo, check remote_repository and local_folder_path!");
                return ERROR;
            }
        }
    }

    if(CMD::Run(("git -C " + local_folder_path + " ls-remote").c_str()).first != OK)
    {
        NotificationServer::send("Any remotes are present!");
        return ERROR;
    }
    return OK;
}

OutputCodes AppSetup::CheckNotificationLevel()
{
    char m_notification_level_char = configuration["notification_level"].get<std::string>()[0];
    if (m_notification_level_char == ' ')
    {
        NotificationServer::send("Couln't read notification_level character in configuration.json");
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
            NotificationServer::send("Invalid notification_level character in configuration.json.");
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
    std::sort(temp_CMD_vector.begin(), temp_CMD_vector.end(), [](CMD* a, CMD* b) {return a->GetExecOrder() < b->GetExecOrder();});

    for (CMD* vector_CMD : temp_CMD_vector)
    {
        if (vector_CMD->GetExecOrder() < 0)
            pre_commands_queue->push(vector_CMD);
        else
            post_commands_queue->push(vector_CMD);
    }
    
    // base git ops
    CMD* git_cmd_0 = new CMD(CD, "cd " + local_folder_path, true);
    git_commands_queue->push(git_cmd_0);
    CMD* git_cmd_1 = new CMD(REV_PARSE, "git rev-parse --is-inside-work-tree", true);
    git_commands_queue->push(git_cmd_1);
    CMD* git_cmd_2 = new CMD(INIT, "git init", true);
    git_commands_queue->push(git_cmd_2);
    CMD* git_cmd_3 = new CMD(REMOTE_ADD, "git remote add origin " + remote_repository, true);
    git_commands_queue->push(git_cmd_3);
    CMD* git_cmd_4 = new CMD(PULL, "git pull origin main", true);
    git_commands_queue->push(git_cmd_4);

    return OK;
}

std::queue<CMD*>* AppSetup::GetPreCmdQueue()
{
    return pre_commands_queue;
}

std::queue<CMD*>* AppSetup::GetGitCmdQueue()
{
    return git_commands_queue;
}

std::queue<CMD*>* AppSetup::GetPostCmdQueue()
{
    return post_commands_queue;
}

std::string AppSetup::GetLocalFolderPath()
{
    return local_folder_path;
}

std::string AppSetup::GetRemoteRepository()
{
    return remote_repository;
}

NotificationLevels AppSetup::GetBaseNotificationLevel()
{
    return notification_level;
}