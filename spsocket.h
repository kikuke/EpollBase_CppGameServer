#ifndef SPSOCKET_H
#define SPSOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

//Make zero memory sockaddr_in and init s_addr, sin_port
void SetSockaddr_In(struct sockaddr_in* addr_in, in_addr_t s_addr, in_port_t sin_port);

//Make TCP Server Socket. return socket fd.
//-1 is error value.
int SetTCPServSock(in_addr_t s_addr, in_port_t sin_port, int listen_queue, int isReuse);


#ifdef __cplusplus
}
#endif

#endif