#include "public.hpp"

#include <iostream>

// Class that will asynchrounsluy produce notifications and send them to appropriate channels

/*
    TODO:
        - Creare un tread "worker" nel costruttore
        - creare un distruttore adeguato
        - fare una queue per i messaggi
        - fare metodo ESPOSTO al main send_notification per aggiungere alla queue il messaggio
        - mutex su queue
        - condition_variable per triggerare il thread e leggere la queue
        - thread accede alla queue per inviare i messaggi

*/

class NotificationServer
{
private:
    std::string log_file_path, mqtt_address;
    NotificationLevels notification_level;
    
public:
    NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level);
    ~NotificationServer();

    OutputCodes send_notification(NotificationLevels level, std::string text);
};

