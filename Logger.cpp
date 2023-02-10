#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#include "spfileio.h"
#include "Logger.h"

bool Logger::isSet;

LOGLEVEL Logger::runLevel;
char Logger::saveRootDir[DEFAULT_LOG_DIR_SIZE];
size_t Logger::bufSize;

int Logger::LoggerSetting(LOGLEVEL _runLevel, const char* _saveRootDir, size_t _bufSize)
{
    if(isSet)
        return 0;
    
    isSet = true; runLevel = _runLevel; bufSize = _bufSize;
    strcpy(saveRootDir, _saveRootDir);

    mkdir_all(saveRootDir);

    return isSet;
}

Logger::Logger(const char* logFolderName)
{
    if(!isSet){
        LoggerSetting(LOGLEVEL::DEBUG, "./Log", DEFAULT_LOG_BUFFER_SIZE);
    }
    
    sprintf(savePath, "%s/%s/", saveRootDir, logFolderName);
    savePathLen = strlen(savePath);

    buffer = new char[bufSize];
    memset(buffer, 0, sizeof(buffer));

    mkdir_all(savePath);
}

void Logger::Log(LOGLEVEL logLevel, const char* format, ...)
{
    va_list args;

    if(runLevel > logLevel)
        return;
    
    SetNowTime();
    strcpy(buffer + strlen(buffer), LogPrefix[logLevel]);

    va_start(args, format);
    sprintf(buffer + strlen(buffer), format, args);
    va_end(args);

    strcpy(buffer + strlen(buffer), "\n");
    printf("%s", buffer);

    SaveLog();

    memset(buffer, 0, sizeof(buffer));
}

int Logger::SaveLog()
{
    FILE* logFile;

    strcpy(savePath + savePathLen, saveFileName);
    if(runLevel == LOGLEVEL::DEBUG) {
        logFile = fopen(savePath, "a+");
    } else {
        logFile = fopen(savePath, "ab+");
    }

    if(logFile == NULL){
        perror("SaveLog()");

        return -1;
    }
    fprintf(logFile, "%s", buffer);

    fclose(logFile);
    return 0;
}

int Logger::SetNowTime()
{
    struct timeval now;
    struct tm* tm_now;

    if(gettimeofday(&now, NULL)!=0)
        return 0;
    tm_now = localtime(&now.tv_sec);

    sprintf(saveFileName, "%02d-%02d-%02d_%01d.txt", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour/6+1);
    sprintf(buffer, "[%02d-%02d-%02d %02d:%02d:%02d:%03ld]",
    tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday,
    tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, now.tv_usec);

    return 1;
}