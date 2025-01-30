#ifndef PUBLIC_HPP
#define PUBLIC_HPP

// Public usable interfaces and defines

enum OutputCodes { 
    OK,
    WARNING,
    ERROR,
    CRITICAL_ERROR
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

#define STD_REFRESH_TIME_SECONDS 600
#define SECONDS_MULTIPLIER 1
#define MINUTES_MULTIPLIER 60
#define HOURS_MULTIPLIER 3600
#define DAYS_MULTIPLIER 86400

#endif