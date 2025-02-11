#include "notification_server.hpp"

NotificationServer::NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level)
{
    log_file_path = _log_file_path;
    mqtt_address = _mqtt_address;
    minimum_notification_level = _notification_level;
    worker = std::thread(&NotificationServer::do_work, this);
}

NotificationServer::~NotificationServer()
{
    stop = true;
}

OutputCodes NotificationServer::send(Notification message)
{
    if (message.level < minimum_notification_level) return OK;
    std::unique_lock lock(worker_mutex);
    notification_queue.push(std::make_shared<Notification>(message));
    lock.unlock();
    work_todo.notify_one();
    return OK;
}

OutputCodes NotificationServer::sendQueue(std::queue<Notification>* messages_queue)
{   
    std::unique_lock lock(worker_mutex);
    while (messages_queue->size())
    {
        if (!messages_queue->front().level < minimum_notification_level)       
            notification_queue.push(std::make_shared<Notification>(messages_queue->front()));
        messages_queue->pop();
    }
    lock.unlock();
    work_todo.notify_one();
    return OK;
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
        CslMsg("Can't open log file stream at " + log_file_path + "!");
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

void NotificationServer::setLogFilePath(std::string log_file)
{
    log_file_path = log_file;
}

void NotificationServer::setMqttAddress(std::string mqtt)
{
    mqtt_address = mqtt;
}