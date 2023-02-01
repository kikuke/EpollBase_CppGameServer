#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "spsocket.h"
#include "spepoll.h"

int main(void)
{
    constexpr int SERV_ADDR = INADDR_ANY;
    //나중에 읽기 파일 만들어서 할당시키기
    constexpr int SERV_PORT = 1234;
    constexpr int EPOLL_SIZE = 5000;
    constexpr int BUF_SIZE = 2048;

    int serv_sock, clnt_sock;
    struct sockaddr_in clnt_adr;
    socklen_t adr_sz;
    int epfd, event_cnt;
    int i;
    char buf[BUF_SIZE];

    struct epoll_event* ep_events;
    
    serv_sock = SetTCPServSock(SERV_ADDR, SERV_PORT, SOMAXCONN);
    if(serv_sock == -1){
        exit(1);
    }

    epfd = InitEpoll(ep_events, EPOLL_SIZE);
    SetETServSock(epfd, serv_sock);

    do
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1){
            break;
        }

        for(i = 0; i < event_cnt; i++)
        {
            if(ep_events[i].data.fd == serv_sock){
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                AddETClntSock(epfd, clnt_sock);
            }
            else {
                if(ReadET(ep_events[i].data.fd, buf, BUF_SIZE, write) == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                }
                else
                {
                    /*
                    위에서는 write로 바로 처리하고있다. 스트림 형태이므로 나중에 메소드 바꿔서 TCP마다 메시지 시작과 끝 체크 후 처리 필요.
                    전체 메시지 발송 등등
                    */
                }
            }
        }
    } while (true);
    

    close(serv_sock);
    close(epfd);
    return 0;
}
