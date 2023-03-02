#ifndef SERVER_THREAD
#define SERVER_THREAD

#include "GameRoom/GameRoomManager.h"
#include "ServerInfo.h"

void WorkThread(JobQueue* jobQueue, const int epfd);

void ReadThread(JobQueue* jobQueue, const int buf_sz);

void BroadcastThread(JobQueue* jobQueue, GameRoomManager& gameRoomManager);

void GameRoomThread(GameRoomManager& gameRoomManager);

#endif