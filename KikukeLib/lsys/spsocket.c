#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "spsocket.h"

void SetSockaddr_In(struct sockaddr_in* addr_in, in_addr_t s_addr, in_port_t sin_port)
{
    memset(addr_in, 0, sizeof(*addr_in));
    (*addr_in).sin_family=AF_INET;
    (*addr_in).sin_addr.s_addr=htonl(s_addr);
    (*addr_in).sin_port=htons(sin_port);
}

int SetTCPServSock(in_addr_t s_addr, in_port_t sin_port, int listen_queue, int isReuse)
{
    int serv_sock;
    struct sockaddr_in serv_adr;
    socklen_t optlen;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        return -1;
    }

    optlen = sizeof(isReuse);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&isReuse, optlen);

    SetSockaddr_In(&serv_adr, s_addr, sin_port);

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        return -1;
    }

    if(listen(serv_sock, listen_queue) == -1){
        return -1;
    }

    return serv_sock;
}