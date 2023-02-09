#ifndef LOGGER
#define LOGGER

#include <stdio.h>
#include <sys/types.h>

#define DEFAULT_LOG_DIR_SIZE 512
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
    static bool isSet;

    static LOGLEVEL runLevel;
    static char saveRootDir[DEFAULT_LOG_DIR_SIZE];
    static size_t bufSize;

    char savePath[DEFAULT_LOG_DIR_SIZE*2];
    char* buffer;

    char* GetNowTime();

public:
    static int LoggerSetting(LOGLEVEL runLevel, const char* saveRootDir, size_t bufSize);
    Logger(char* logFileName);
    ~Logger()
    {
        delete[] savePath;
        delete[] buffer;
    }
};

#endif