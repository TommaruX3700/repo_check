#include "public.hpp"

#include <iostream>

// Factory class that will produce notifications and send them to appropriate channels

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

