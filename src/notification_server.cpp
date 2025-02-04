#include "notification_server.hpp"

NotificationServer::NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level)
    : log_file_path { _log_file_path }, mqtt_address { _mqtt_address }, minimum_notification_level { _notification_level }
{
    
}

NotificationServer::~NotificationServer()
{
}

void NotificationServer::start()
{
    // makes worker start
}

OutputCodes NotificationServer::send(Notification message)
{
    // enqueues the notifications
    // writer, unique lock
    
    if (message.level < minimum_notification_level)
        return OK;

    std::unique_lock lock(worker_mutex);
    notification_queue.push(message);
    lock.unlock();

    return OK;
}

OutputCodes NotificationServer::sendQueue(std::queue<Notification>* messages_queue)
{   
    // writer, unique lock
    std::unique_lock lock(worker_mutex);
    while (messages_queue->size())
    {
        notification_queue.push(messages_queue->front());
        messages_queue->pop();
    }
    lock.unlock();
    return OK;
}

OutputCodes NotificationServer::do_work()
{
    // reader, shared lock
    std::shared_lock lock(worker_mutex);
    
    try
    {
        if (started)
        {
            // make condition variable that notifies if there are any messages waiting to resolution
            std::unique_lock<std::mutex> worker_mutex;
            
            while (notification_queue.size())
            {
                if (!log_file_path.empty())
                    write_to_log(notification_queue.front());
                
                if (!mqtt_address.empty())
                    write_to_mqtt(notification_queue.front());
                
                notification_queue.pop();
            }
        }   
        lock.unlock();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        lock.unlock();
        return WARNING;
    }
    return OK;
}

OutputCodes write_to_log(Notification message)
{
    // write to log file
    return OK;
}

OutputCodes write_to_mqtt(Notification message)
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