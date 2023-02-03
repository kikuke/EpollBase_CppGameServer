#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "spsocket.h"
#include "spepoll.h"
#include "PacketCollector.h"
#include "TcpService.h"

int main(void)
{
    constexpr int SERV_ADDR = INADDR_ANY;
    //나중에 읽기 파일 만들어서 할당시키기
    constexpr int SERV_PORT = 1234;
    constexpr int EPOLL_SIZE = 5000;
    constexpr int BUF_SIZE = 2048;

    int serv_sock;
    int epfd, event_cnt;
    char buf[BUF_SIZE];

    struct epoll_event* ep_events;
    
    int i;
    
    serv_sock = SetTCPServSock(SERV_ADDR, SERV_PORT, SOMAXCONN);
    if(serv_sock == -1){
        exit(1);
    }

    epfd = InitEpoll(ep_events, EPOLL_SIZE);
    SetETServSock(epfd, serv_sock);

    //나중에 멀티스레딩도적용해보기
    do
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1){
            break;
        }

        for(i = 0; i < event_cnt; i++)
        {
            if(ep_events[i].data.fd == serv_sock){
                AcceptTcpSocket(ep_events[i].data.fd, epfd);
            }
            else {
                if(ReadET(ep_events[i].data.fd, buf, BUF_SIZE, WriteRingBuffer) == 0)
                {
                    CloseTcpSocket(ep_events[i].data.fd, epfd);
                }
            }

            //해당 패킷의 완전 수신여부 체크해서 처리하기.
        }
    } while (true);
    

    close(serv_sock);
    close(epfd);
    return 0;
}
