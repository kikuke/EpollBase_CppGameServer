#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "spsocket.h"

int SetTCPServSock(uint32_t s_addr, uint16_t s_port, int listen_queue)
{
    int serv_sock;
    struct sockaddr_in serv_adr;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        return -1;
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(s_addr);
    serv_adr.sin_port=htons(s_port);

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        return -1;
    }

    if(listen(serv_sock, listen_queue) == -1){
        return -1;
    }

    return serv_sock;
}