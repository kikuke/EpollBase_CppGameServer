#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#include "spsocket.h"
#include "spepoll.h"
#include "PacketIO.h"
#include "TcpService.h"
#include "PacketHandler.h"
#include "TcpPacketHandler.h"
#include "TcpMessagePacket.h"
#include "Logger.h"

int main(void)
{
    constexpr int SERV_ADDR = INADDR_ANY;
    // 나중에 읽기 파일 만들어서 할당시키기
    constexpr int SERV_PORT = 1234;
    constexpr int EPOLL_SIZE = 5000;
    constexpr int BUF_SIZE = 2048;

    int serv_sock=-1;
    int epfd=-1, event_cnt=0;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;

    TcpService tcpService;

    PacketHandler *tcpHandles[] = {new TcpMessagePacket(), new TcpMessagePacket()};
    TcpPacketHandler tcpPacketHandler(tcpHandles, sizeof(tcpHandles) / sizeof(*tcpHandles));

    int i=0;

    // 읽기 파일 만들어서 할당시키기 - 읽기수준, 저장 경로
    Logger::LoggerSetting(LOGLEVEL::DEBUG, "./TestLog", DEFAULT_LOG_BUFFER_SIZE);
    Logger log("MainLog");

    log.Log(LOGLEVEL::INFO, "Start Server...");

    serv_sock = SetTCPServSock(SERV_ADDR, SERV_PORT, SOMAXCONN, true);
    if (serv_sock == -1)
    {
        log.Log(LOGLEVEL::ERROR, "SetTCPServSock() - %s", strerror(errno));
        exit(1);
    }

    epfd = InitEpoll(&ep_events, EPOLL_SIZE);

    SetETServSock(epfd, serv_sock);
    log.Log(LOGLEVEL::DEBUG, "SetETServSock()");

    // 나중에 멀티스레딩도적용해보기 아직은 tcp 요청만 처리하지만 나중에 udp등의 요청을 처리하거나 다른 스레드나 프로세스로 연결해주기
    do
    {
        log.Log(LOGLEVEL::DEBUG, "epoll_wait()");

        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1)
        {
            log.Log(LOGLEVEL::ERROR, "epoll_wait() - %s", strerror(errno));
            break;
        }
    
        for (i = 0; i < event_cnt; i++)
        {
            if (ep_events[i].data.fd == serv_sock)
            {
                tcpService.AcceptTcpSocket(ep_events[i].data.fd, epfd);
                log.Log(LOGLEVEL::DEBUG, "AcceptTcpSocket()");
            }
            else
            {
                if (ReadET(ep_events[i].data.fd, buf, BUF_SIZE, WriteRingBuffer) == 0)
                {
                    tcpService.CloseTcpSocket(ep_events[i].data.fd, epfd);
                    log.Log(LOGLEVEL::DEBUG, "CloseTcpSocket()");
                }
            }

            while (tcpPacketHandler.execute(ep_events[i].data.fd)) // 메시지 처리함수. 빌때까지.
            {
                log.Log(LOGLEVEL::DEBUG, "TcpPacketHandler.excute()");
            }
        }
    } while (true);

    log.Log(LOGLEVEL::INFO, "Shutdown Server...");
    close(serv_sock);
    close(epfd);
    return 0;
}
