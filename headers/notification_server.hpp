#include "public.hpp"

#include <iostream>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
// #include <functional>

// Class that will asynchrounsluy produce notifications and send them to appropriate channels

/*
    TODO:
        - Creare un tread "worker" nel costruttore
            - Se "start" non è a true, raccogli i log in queue ma non puoi inviarli
        - creare un distruttore adeguato
        ok - fare una queue per i messaggi
        - fare metodo ESPOSTO al main send_notification() per aggiungere alla queue il messaggio
        - mutex su queue
        - condition_variable per triggerare il thread e leggere la queue
        - thread accede alla queue per inviare i messaggi

*/

class NotificationServer
{
private:
    bool started = false;
    std::string log_file_path, mqtt_address;
    
    // Minimum_notification_level defines what is the minim level required to notifications
    // for beign published
    NotificationLevels minimum_notification_level;
    // Here are stored all the notifications 
    std::queue<Notification>notification_queue;

    std::mutex worker_mutex;
    std::condition_variable work_todo;
    
    std::thread worker;
    OutputCodes do_work();
    
public:
    NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level);
    ~NotificationServer();

    void start();
    OutputCodes send(Notification message);
};

