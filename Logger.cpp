#include <string.h>

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
    return isSet;
}

Logger::Logger(char* logFileName)
{
    if(!isSet){
        LoggerSetting(LOGLEVEL::DEBUG, "./Log", DEFAULT_LOG_BUFFER_SIZE);
    }
    
    sprintf(savePath, "%s/%s", saveRootDir, logFileName);
    buffer = new char[bufSize];
}


char* Logger::GetNowTime()
{
    
}