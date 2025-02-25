#include "notification_server.hpp"

NotificationServer::NotificationServer()
{
    worker = std::thread(&NotificationServer::do_work, this);
    minimum_notification_level = DEBUG;
}

NotificationServer::~NotificationServer()
{
    stop = true;
}

// Check cached Messages from the rest of the project, if there are any and with the right level, add them to logs
// Ho bisogno di ri-dichiarare la variabile statica
std::queue<Notification> NotificationServer::cachedMsg;
void NotificationServer::checkCache()
{
    if (cachedMsg.size())
    {
        while (cachedMsg.size())
        {
            if (!cachedMsg.front().level < minimum_notification_level)
                notification_queue.push(std::make_shared<Notification>(cachedMsg.front()));
            cachedMsg.pop();
        }
    }
}

///////////////////////////////////////////////////////////////
// send() is to send notifications to LOGs and other sources and also to write to console
// send() should be the only wrappers to interact with notification_queue 

// TODO: remove unecessary OutputCodes
OutputCodes NotificationServer::send(Notification message)
{
    if (message.level < minimum_notification_level) return OK;
    std::unique_lock lock(worker_mutex);
    checkCache();
    notification_queue.push(std::make_shared<Notification>(message));
    lock.unlock();
    return OK;
}

// TODO: remove unecessary OutputCodes
OutputCodes NotificationServer::send(std::queue<Notification>* messages_queue)
{   
    std::unique_lock lock(worker_mutex);
    checkCache();
    while (messages_queue->size())
    {
        if (!messages_queue->front().level < minimum_notification_level)       
            notification_queue.push(std::make_shared<Notification>(messages_queue->front()));
        messages_queue->pop();
    }
    lock.unlock();
    return OK;
}

void NotificationServer::send(std::string msg)
{
    std::string formattedMsg = GetFormattedTime("") + ": " + msg;
    std::cout << formattedMsg << std::endl;
    cachedMsg.push({DEBUG, formattedMsg});
}

///////////////////////////////////////////////////////////////

// This is used to start the log thread
void NotificationServer::pushLogs()
{
    work_todo.notify_one();
}

OutputCodes NotificationServer::do_work()
{
    while (true)
    {
        // questo lock è rilasciato nell'esecuzione di '.wait()', che attende a riprenderne il controllo
        // fino a quando non riceve una condizione vera tra le definite dalla lambda.
        std::unique_lock lock(worker_mutex);
        // se una condizione nel contestoo corrente è 'true', continua
        work_todo.wait(lock, [this] { return !notification_queue.empty() || stop; });
        if(stop) break;

        std::queue<std::shared_ptr<Notification>> snapshot_queue = std::move(notification_queue);
        lock.unlock();
        
        // se almeno una delle due stringe non è vuota, esegui, altimenti skippa e aspetta
        if (!log_file_path.empty() || !mqtt_address.empty())
        {
            try
            {
                while (!snapshot_queue.empty())
                {
                    if (!log_file_path.empty())
                        write_to_log(*snapshot_queue.front());
                    
                    if (!mqtt_address.empty())
                        write_to_mqtt(*snapshot_queue.front());
                    
                    snapshot_queue.pop();
                }  
            }
            catch(const std::exception& e)
            {
                std::cerr << "Errore nell'esecuzione del thread worker: " << e.what() << '\n';
                return WARNING;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(STANDARD_REFRESH_NOTIFICATION_SECONDS));
    }
    return OK;
}

OutputCodes NotificationServer::write_to_log(Notification message)
{
    // write to log file
    log_file_stream.open(log_file_path, std::ios_base::app);
    if (!log_file_stream.is_open())
    {
        send("Can't open log file stream at " + log_file_path + "!");
        return WARNING;
    }
    
    log_file_stream << GetFormattedTime("%T_%F") << message.text;
    log_file_stream.close();
    return OK;
}

OutputCodes NotificationServer::write_to_mqtt(Notification message)
{
    // write to mqtt channel
    return OK;
}

void NotificationServer::setMinimumNotificationLevel(NotificationLevels level)
{
    minimum_notification_level = level;
}

void NotificationServer::setLogFilePath(std::string log_file)
{
    log_file_path = log_file;
}

void NotificationServer::setMqttAddress(std::string mqtt)
{
    mqtt_address = mqtt;
}