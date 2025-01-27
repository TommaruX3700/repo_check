#include "public.hpp"
#include "notifications.hpp"
#include "json.hpp"

#include <iostream>
#include <string>

// Setup class for whole project startup

/*
TODO:
    > Check configuration file
    > Setup public usable variables
    > Setup "notification" object factory

NB:
    - Protect other methods
*/

class ServerSetup
{
private:
    std::string configuration_file_path;
    NotificationFab* SetupNotificationFab();
    OutputCodes SetConfFilePath(std::string conf_path);
    

public:
    ServerSetup();
    ~ServerSetup();

    OutputCodes StartSetup(std::string conf_path);

    // TODO: set and get methods
    std::string GetConfFilePath(std::string conf_path);
    

};

