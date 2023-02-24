#ifndef SERVER_THREAD
#define SERVER_THREAD

#include "ServerInfo.h"

void WorkThread(JobQueue* jobQueue, const int epfd);

void ReadThread(JobQueue* jobQueue, const int buf_sz);

#endif