#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

int SetServSock(uint32_t s_addr, uint16_t s_port, int listen_queue)
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

int InitEpoll(struct epoll_event* ep_events, size_t epoll_sz)
{
    int epfd;

    epfd = epoll_create(epoll_sz);
    ep_events = (epoll_event*)malloc(sizeof(struct epoll_event) * epoll_sz);

    return epfd;
}

void SetNonblockingFd(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

void SetETServSock(int epfd, int serv_sock)
{
    struct epoll_event event;

    SetNonblockingFd(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
}

void AddETClntSock(int epfd, int clnt_sock)
{
    struct epoll_event event;
    
    SetNonblockingFd(clnt_sock);
    event.events = EPOLLIN|EPOLLET;
    event.data.fd = clnt_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
}

ssize_t ReadET(int fd, void* buf, size_t sz, ssize_t (*etReadFunc)(int, const void*, size_t))
{
    ssize_t str_len = 0;
    ssize_t temp_len;

    while(true)
    {
        temp_len = read(fd, buf,  sz);
        if(temp_len == 0){
            return 0;
        }
        else if(temp_len < 0)
        {
            if(errno == EAGAIN)
                return str_len;
        }
        else
        {
            str_len += etReadFunc(fd, buf, temp_len);
        }
    }
}

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
    
    serv_sock = SetServSock(SERV_ADDR, SERV_PORT, SOMAXCONN);
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
