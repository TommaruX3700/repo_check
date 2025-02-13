#ifndef PUBLIC_HPP
#define PUBLIC_HPP

// Public usable interfaces and defines

// Output codes for functions
enum OutputCodes { 
    OK,
    WARNING,
    ERROR,
    CRITICAL_ERROR
};

// Notification levels to determine how much infos will be given out to notification streams
enum NotificationLevels {
    MINIMAL,
    INFO,
    DEBUG
};

enum TypeOfCmd {
    NOT_GIT,
    GIT
};

enum TypeOfGitCmd {
    CD,
    REV_PARSE,
    INIT,
    REMOTE_ADD,
    PULL

};

#ifdef _WIN32
    #define OS_SEP "\\"
#endif

#ifdef linux
    #define OS_SEP "/"
#endif

#define STANDARD_CONFIG_PATH "." OS_SEP "configuration.json"
#define STANDARD_LOG_FOLDER "." OS_SEP "logs"
#define STANDARD_LOG_FILETYPE ".txt"
#define STANDARD_MQTT_ADDRESS ""
#define STANDARD_REFRESH_TIME "10m"
#define STANDARD_REFRESH_NOTIFICATION_SECONDS 5

#define STD_REFRESH_TIME_SECONDS 60
#define SECONDS_MULTIPLIER 1
#define MINUTES_MULTIPLIER 60
#define HOURS_MULTIPLIER 3600
#define DAYS_MULTIPLIER 86400

#include <utility>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <queue>

/*
    CmdOutput
        > defines the output code and verbose output of a cmd
*/
typedef std::pair<OutputCodes, std::string> CmdOutput;

/*
    Notification
        > defines a notification, with level and string
*/
struct Notification
{
    NotificationLevels level;
    std::string text;
};

// inline definition helps the function not to be defined on multiple ocasions 
inline std::string GetFormattedTime(const char* format)
{
    if (format[0] == ' ' || sizeof(format)/sizeof(char*) == 0) format = "%T_%F";    
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), format);
    return  oss.str();
}

#endif