#include "notification_server.hpp"

NotificationServer::NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level)
    : log_file_path { _log_file_path }, mqtt_address { _mqtt_address }, notification_level { _notification_level }
{
}

NotificationServer::~NotificationServer()
{
}

OutputCodes NotificationServer::send_notification(NotificationLevels level, std::string text)
{
    
    // getting notification level makes it usable also in run-time error debugging contexts
    // if a notification doesnt meet the level configured in the class, it wont be published.

    if (level != notification_level)
        return OK;

    // trasmit notification to all channels
    
    return OK;
}