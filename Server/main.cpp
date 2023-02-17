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
#include "PacketIO.h"
#include "TcpService.h"
#include "PacketHandler.h"
#include "TcpPacketHandler.h"
#include "Logger.h"
#include "impl/TcpMessagePacket.h"

//Todo: 나중에 읽기 파일 만들어서 할당시키기 자리도 옮기기
constexpr unsigned int SERV_ADDR = INADDR_ANY;
constexpr unsigned int SERV_PORT = 1234;
constexpr int EPOLL_SIZE = 5000;
constexpr int BUF_SIZE = 2048;
constexpr int READTHREAD_SIZE = 4;

void ReadThread(JobQueue* jobQueue, int epfd)//Todo: 위치 옮기기
{
    //Todo: 스레드id 로그에 찍기

    int sock;
    TcpService tcpService;

    char buf[BUF_SIZE];

    Logger log("MainLog");//Todo: ThreadSafe로 바꾸기
    log.Log(LOGLEVEL::DEBUG, "ReadThread Start");

    //소켓 범위일 경우
    while((sock = jobQueue->readQueue.pop()) > 2)
    {
        log.Log(LOGLEVEL::DEBUG, "Read ReadQueue: %d", sock);
        //Todo: r/w전용 큐 만들어서 넣어버리기. 다른스레드에서 읽게끔. 처리전용 스레드도 만들고 처리전용 큐 만들어서 거기서 처리하기
        if (ReadET(sock, buf, BUF_SIZE, WriteRingBuffer) == 0)
        {
            tcpService.CloseTcpSocket(sock, epfd);
            log.Log(LOGLEVEL::DEBUG, "CloseTcpSocket()");
        }
        else//Todo: 스레드 분리 처리하기
        {
            jobQueue->workQueue.push(sock);
            log.Log(LOGLEVEL::DEBUG, "Push WorkQueue: %d", sock);
        }
    }

    log.Log(LOGLEVEL::ERROR, "Read Thread Down!");
    return;
}

void WorkThread(JobQueue* jobQueue)//Todo: RingBuffer 스레드세이프로 바꾸기
{
    int sock;

    PacketHandler *tcpHandles[] = {new TcpMessagePacket(), new TcpMessagePacket()};
    TcpPacketHandler tcpPacketHandler(tcpHandles, sizeof(tcpHandles) / sizeof(*tcpHandles));

    Logger log("MainLog");
    log.Log(LOGLEVEL::DEBUG, "WorkThread Start");

    //소켓 범위일 경우
    while((sock = jobQueue->workQueue.pop()) > 2)
    {
        while (tcpPacketHandler.execute(sock)) // 메시지 처리함수. 빌때까지.
        {
            log.Log(LOGLEVEL::DEBUG, "TcpPacketHandler.excute()");
        }
    }

    log.Log(LOGLEVEL::ERROR, "Work Thread Down!");
    return;
}

int main(void)
{
    int serv_sock=-1;
    int epfd=-1, event_cnt=0;

    struct epoll_event *ep_events;
    JobQueue jobQueue;
    
    //Todo: 읽기 파일 만들어서 할당시키기 - 읽기수준, 저장 경로
    Logger::LoggerSetting(LOGLEVEL::DEBUG, "../TestLog", DEFAULT_LOG_BUFFER_SIZE);
    Logger log("MainLog");

    TcpService tcpService;

    std::thread* readThreads[READTHREAD_SIZE];
    std::thread* workThread;

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

    //읽기 스레드 생성
    for(int i=0; i<READTHREAD_SIZE; i++){
        readThreads[i] = new std::thread(ReadThread, &jobQueue, epfd);
    }

    //작업 스레드 생성
    workThread = new std::thread(WorkThread, &jobQueue);

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
            if (ep_events[i].data.fd == serv_sock)//Todo: 얘를 함수로 추가 분리하기 시멘틱 프로그래밍
            {
                tcpService.AcceptTcpSocket(ep_events[i].data.fd, epfd);
                log.Log(LOGLEVEL::DEBUG, "AcceptTcpSocket()");
            }
            else
            {
                jobQueue.readQueue.push(ep_events[i].data.fd);
                log.Log(LOGLEVEL::DEBUG, "Push ReadQueue: %d", ep_events[i].data.fd);
            }
        }
    } while (true);//Todo: dowhile사용 하지않기 반복문 탈출은 break나 와일조건 검사안에 true가 아닌 조건문넣어서 bool값을 넣고 내부에서 bool값 변경해도 됨.

    log.Log(LOGLEVEL::INFO, "Shutdown Server...");
    close(serv_sock);
    close(epfd);
    return 0;
}
