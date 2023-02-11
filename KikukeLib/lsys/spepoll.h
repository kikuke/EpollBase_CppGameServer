#ifndef SPEPOLL_H
#define SPEPOLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

//Set FileDiscripter to Nonblocking mode.
void SetNonblockingFd(int fd);

//Make epoll_event to epoll_sz size and return epoll file discripter.
int InitEpoll(struct epoll_event** ep_events, size_t epoll_sz);

//Insert serv_sock to epoll file discripter and mod nonblocking mode
void SetETServSock(int epfd, int serv_sock);

//Insert clnt_sock to epoll file discripter and mod nonblocking, it worked edgetrigger.
void AddETClntSock(int epfd, int clnt_sock);

//Read edgetrigger fd and do etReadFunc (read data / sz) times. 
ssize_t ReadET(int fd, void* buf, size_t sz, ssize_t (*etReadFunc)(int, const void*, size_t));

#ifdef __cplusplus
}
#endif

#endif