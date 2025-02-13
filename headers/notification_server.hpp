#ifndef NOTIFICATIONSERVER_HPP
#define NOTIFICATIONSERVER_HPP

#include "public.hpp"

#include <iostream>
#include <queue>

#include <thread>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>
#include <fstream>
// #include <functional>

// Class that will asynchrounsluy produce notifications and send them to appropriate channels

class NotificationServer
{
private:
    
    std::string log_file_path, mqtt_address;
    std::ofstream log_file_stream;
    // Minimum_notification_level defines what is the minim level required to notifications for beign published
    NotificationLevels minimum_notification_level;

    // Cached messages, ovvero messaggi che aspettano di venir trasmessi
    static std::queue<Notification> cachedMsg;

    // Here are stored all the notifications 
    std::queue<std::shared_ptr<Notification>>notification_queue;

    // manages access to queue
    std::mutex worker_mutex;
    
    // manages the worker
    bool stop = false;
    std::condition_variable work_todo;

    void checkCache();
    
    OutputCodes do_work();
    std::thread worker;

    OutputCodes write_to_log(Notification message);
    OutputCodes write_to_mqtt(Notification message);

public:
    NotificationServer();
    ~NotificationServer();

    OutputCodes send(Notification message);
    OutputCodes send(std::queue<Notification>* messages_queue);
    static void send(std::string msg);
    
    void setMinimumNotificationLevel(NotificationLevels level);
    void setLogFilePath(std::string log_file);
    void setMqttAddress(std::string mqtt);

    void pushLogs();
};

#endif