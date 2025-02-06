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

/*
    TODO:
        ok - Creare un tread "worker" nel costruttore
        ok - creare un distruttore adeguato
        ok - fare una queue per i messaggi
        ok - fare metodo ESPOSTO al main send_notification() per aggiungere alla queue il messaggio
        ok - mutex su queue
        ok - condition_variable per triggerare il thread e leggere la queue
        ok - thread accede alla queue per inviare i messaggi
*/

class NotificationServer
{
private:
    
    std::string log_file_path, mqtt_address;
    std::ofstream log_file_stream;
    
    // Minimum_notification_level defines what is the minim level required to notifications for beign published
    NotificationLevels minimum_notification_level;

    // Here are stored all the notifications 
    std::queue<std::shared_ptr<Notification>>notification_queue;

    // manages access to queue
    std::mutex worker_mutex;
    
    // manages the worker
    bool stop = false;
    std::condition_variable work_todo;
    
    OutputCodes do_work();
    std::thread worker;

    OutputCodes write_to_log(Notification message);
    OutputCodes write_to_mqtt(Notification message);

public:
    NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level);
    ~NotificationServer();

    OutputCodes send(Notification message);
    OutputCodes sendQueue(std::queue<Notification>* messages_queue);
    
    void setLogFilePath(std::string log_file);
    void setMqttAddress(std::string mqtt);
};

