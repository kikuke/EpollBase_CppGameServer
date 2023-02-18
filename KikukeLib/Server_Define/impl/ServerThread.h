#ifndef SERVER_THREAD
#define SERVER_THREAD

#include "ServerInfo.h"

void WorkThread(JobQueue* jobQueue);

void ReadThread(JobQueue* jobQueue, const int epfd, const int buf_sz);

#endif