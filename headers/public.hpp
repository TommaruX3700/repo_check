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