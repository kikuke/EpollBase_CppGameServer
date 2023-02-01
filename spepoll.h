#ifndef SPEPOLL_H
#define SPEPOLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void SetNonblockingFd(int fd);

int InitEpoll(struct epoll_event* ep_events, size_t epoll_sz);

void SetETServSock(int epfd, int serv_sock);

void AddETClntSock(int epfd, int clnt_sock);

ssize_t ReadET(int fd, void* buf, size_t sz, ssize_t (*etReadFunc)(int, const void*, size_t));

#ifdef __cplusplus
}
#endif

#endif