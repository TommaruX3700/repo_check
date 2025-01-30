#include "notification_server.hpp"

NotificationServer::NotificationServer(std::string _log_file_path, std::string _mqtt_address, NotificationLevels _notification_level)
    : log_file_path { _log_file_path }, mqtt_address { _mqtt_address }, notification_level { _notification_level }
{
}

NotificationServer::~NotificationServer()
{
}
