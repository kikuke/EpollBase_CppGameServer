#ifndef SPSOCKET_H
#define SPSOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

//Make TCP Server Socket. return socket fd.
int SetTCPServSock(uint32_t s_addr, uint16_t s_port, int listen_queue);


#ifdef __cplusplus
}
#endif

#endif