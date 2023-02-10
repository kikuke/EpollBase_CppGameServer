#ifndef LOGGER
#define LOGGER

#include <sys/types.h>

#define DEFAULT_LOG_DIR_SIZE 256
#define DEFAULT_LOG_BUFFER_SIZE 2048

enum LOGLEVEL
{
    DEBUG,
    WARNING,
    ERROR,
    INFO
};

class Logger
{
private:
    const char* LogPrefix[4] = {
        "DEBUG: ",
        "WARNING: ",
        "ERROR: ",
        "INFO: "
    };

    static bool isSet;

    static LOGLEVEL runLevel;
    static char saveRootDir[DEFAULT_LOG_DIR_SIZE];
    static size_t bufSize;

    char savePath[DEFAULT_LOG_DIR_SIZE*2];
    char saveFileName[DEFAULT_LOG_DIR_SIZE/2];
    size_t savePathLen;

    char* buffer;

    int SetNowTime();
    int SaveLog();

public:
    static int LoggerSetting(LOGLEVEL runLevel, const char* saveRootDir, size_t bufSize);
    Logger(const char* logFolderName);
    ~Logger()
    {
        delete[] buffer;
    }

    void Log(LOGLEVEL logLevel, const char* format, ...);
};

#endif