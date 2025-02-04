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
    started = true;
}

OutputCodes NotificationServer::send(Notification message)
{
    // enqueues the notifications

    // getting notification level makes it usable also in run-time error debugging contexts
    // if a notification doesnt meet the level configured in the class, it wont be published.

    if (message.level < minimum_notification_level)
        return OK;

    std::unique_lock<std::mutex> lock(worker_mutex);
    lock.lock();
    notification_queue.push(message);
    work_todo.notify_all();
    lock.unlock();

    return OK;
}

OutputCodes NotificationServer::do_work()
{
    // lock queue
    // send notifications
    // unlock queue

    std::unique_lock lock(worker_mutex);
    work_todo.wait(lock);
    
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
        }
        
        worker_mutex.unlock();
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