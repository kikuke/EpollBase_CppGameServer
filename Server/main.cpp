#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <thread>

#include "ServerInfo.h"
#include "spsocket.h"
#include "spepoll.h"
#include "TcpService.h"
#include "main/ServerThread.h"
#include "Logger.h"

//Todo: 나중에 읽기 파일 만들어서 할당시키기 자리도 옮기기
constexpr unsigned int SERV_ADDR = INADDR_ANY;
constexpr unsigned int SERV_PORT = 1234;
constexpr int EPOLL_SIZE = 5000;
constexpr int BUF_SIZE = 2048;
constexpr int READTHREAD_SIZE = 4;

int main(void)
{
    int serv_sock=-1;
    int epfd=-1, event_cnt=0;

    struct epoll_event *ep_events;
    JobQueue jobQueue;
    
    //Todo: 읽기 파일 만들어서 할당시키기 - 읽기수준, 저장 경로
    //Comment: DEBUG는 디버깅용, 서버는 WARNING 이상으로
    //Logger::LoggerSetting(LOGLEVEL::DEBUG, "/home/kikuke/Documents/TestLog", DEFAULT_LOG_BUFFER_SIZE);
    Logger::LoggerSetting(LOGLEVEL::WARNING, "/home/kikuke/Documents/TestLog", DEFAULT_LOG_BUFFER_SIZE);
    Logger log("MainLog");

    GameRoomManager gameRoomManager(&jobQueue);

    TcpService* tcpService;

    std::thread* readThreads[READTHREAD_SIZE];
    std::thread* workThread;
    std::thread* gameRoomThread;
    std::thread* broadcastThread;

    int i=0;

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

    tcpService = new TcpService(epfd, serv_sock, &jobQueue);

    //읽기 스레드 생성
    for(int i=0; i<READTHREAD_SIZE; i++){
        readThreads[i] = new std::thread(ReadThread, &jobQueue, BUF_SIZE);
    }

    //작업 스레드 생성
    workThread = new std::thread(WorkThread, &jobQueue, &gameRoomManager, epfd);

    gameRoomThread = new std::thread(GameRoomThread, &gameRoomManager);
    broadcastThread = new std::thread(BroadcastThread, &jobQueue, &gameRoomManager);

    //Todo: 나중에 멀티스레딩도적용해보기 아직은 tcp 요청만 처리하지만 나중에 udp등의 요청을 처리하거나 다른 스레드나 프로세스로 연결해주기
    do
    {
        log.Log(LOGLEVEL::DEBUG, "epoll_wait()");

        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1)
        {
            log.Log(LOGLEVEL::ERROR, "epoll_wait() - %s", strerror(errno));
            exit(1);
        }
    
        for (i = 0; i < event_cnt; i++)
        {
            (*tcpService).Networking(ep_events[i].data.fd);
        }
    } while (true);//Todo: dowhile사용 하지않기 반복문 탈출은 break나 와일조건 검사안에 true가 아닌 조건문넣어서 bool값을 넣고 내부에서 bool값 변경해도 됨.

    log.Log(LOGLEVEL::INFO, "Shutdown Server...");

    delete tcpService;
    close(serv_sock);
    close(epfd);
    return 0;
}