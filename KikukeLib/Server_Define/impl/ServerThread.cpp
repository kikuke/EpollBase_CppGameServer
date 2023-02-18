#include "spepoll.h"
#include "PacketIO.h"
#include "TcpService.h"
#include "PacketHandler.h"
#include "TcpPacketHandler.h"
#include "Logger.h"
#include "TcpMessagePacket.h"
#include "ServerThread.h"

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

void ReadThread(JobQueue* jobQueue, const int epfd, const int buf_sz)//Todo: 위치 옮기기
{
    //Todo: 스레드id 로그에 찍기

    int sock;
    TcpService tcpService;

    char buf[buf_sz];

    Logger log("MainLog");//Todo: ThreadSafe로 바꾸기
    log.Log(LOGLEVEL::DEBUG, "ReadThread Start");

    //소켓 범위일 경우
    while((sock = jobQueue->readQueue.pop()) > 2)
    {
        log.Log(LOGLEVEL::DEBUG, "Read ReadQueue: %d", sock);
        //Todo: r/w전용 큐 만들어서 넣어버리기. 다른스레드에서 읽게끔. 처리전용 스레드도 만들고 처리전용 큐 만들어서 거기서 처리하기
        if (ReadET(sock, buf, buf_sz, WriteRingBuffer) == 0)
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