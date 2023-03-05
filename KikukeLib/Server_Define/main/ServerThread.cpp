#include <unistd.h>

#include "ServerDefine.h"
#include "spepoll.h"
#include "PacketIO.h"
#include "TcpService.h"
#include "PacketHandler.h"
#include "TcpPacketHandler.h"
#include "Logger.h"
#include "impl/handler/TcpMessagePacket.h"
#include "impl/handler/TcpDisconnectPacket.h"
#include "PacketGenerator.h"
#include "SocketManager.h"
#include "ServerThread.h"

#define BROADCAST_BUFFER_SIZE 2048

size_t DisconnectPacketFactory(void* buf);

void WorkThread(JobQueue* jobQueue, const int epfd)//Todo: RingBuffer 스레드세이프로 바꾸기
{
    int sock;

    PacketHandler *tcpHandles[] = { new TcpDisconnectPacket(epfd), new TcpMessagePacket() };
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

void ReadThread(JobQueue* jobQueue, const int buf_sz)//Todo: 위치 옮기기
{
    //Todo: 스레드id 로그에 찍기
    int sock;
    TCPSOCKETINFO* info;

    unsigned char buf[buf_sz];

    Logger log("MainLog");//Todo: ThreadSafe로 바꾸기
    log.Log(LOGLEVEL::DEBUG, "ReadThread Start");

    //소켓 범위일 경우
    while((sock = jobQueue->readQueue.pop()) > 2)
    {
        log.Log(LOGLEVEL::DEBUG, "Read ReadQueue: %d", sock);
        info = SocketManager::getInstance().getTcpSocketInfo(sock);
        if(info == nullptr){//Todo: Disconnect된 클라이언트를 참고하지 않기.
            log.Log(LOGLEVEL::ERROR, "ReadThread - %d TcpSocketInfo is null", sock);
            return;
        }

        if (ReadETRingBuffer(sock, buf, buf_sz) == 0)
        {
            //Write Disconnect Packet in sender recv RingBuffer
            int len;
            close(sock);

            len = DisconnectPacketFactory(buf);
            WriteRingBuffer(sock, buf, len);

            log.Log(LOGLEVEL::DEBUG, "CloseTcpSocket()");
        }

        jobQueue->workQueue.push(sock);
        log.Log(LOGLEVEL::DEBUG, "Push WorkQueue: %d", sock);
    }

    log.Log(LOGLEVEL::ERROR, "Read Thread Down!");
    return;
}

void BroadcastThread(JobQueue* jobQueue, GameRoomManager* gameRoomManager)
{
    int room_id;
    TcpGameRoom* gameRoom;
    int clnt_num;
    int* clnt_socks;

    unsigned char buf[BROADCAST_BUFFER_SIZE];

    int sendLen;

    Logger log("MainLog");
    log.Log(LOGLEVEL::DEBUG, "Broadcast Thread Start");

    while((room_id = jobQueue->broadcastQueue.pop()) >= 0)
    {
        gameRoom = gameRoomManager->GetGameRoom(room_id);
        clnt_num = gameRoom->getClientNum();
        clnt_socks = gameRoom->getClientSocks();

        //Todo: 게임룸의 링 버퍼 가져와서 비지 않았을경우 그안의 모든 멤버들에게 패킷 보내주면 됨.
        //Todo: 게임룸의 링버퍼에 넣는 로직 추가해줘야함. 생성자에서 넣기
        gameRoom->getBroadCastBuffer()->dequeue(buf, BROADCAST_BUFFER_SIZE);
        for(int i=0; i<clnt_num; i++){
            sendLen = write(clnt_socks[i], buf, BROADCAST_BUFFER_SIZE);
            
            log.Log(LOGLEVEL::DEBUG, "Broadcast %d Len to %d Socket", sendLen, clnt_socks[i]);
        }
    }

    log.Log(LOGLEVEL::ERROR, "Broadcast Thread Down!");
    return;
}

void GameRoomThread(GameRoomManager* gameRoomManager)
{
    timeval serverTime;
    timeval beforeTime = {0};
    double frameTime = 1.0/SERVER_FRAME;
    double interval;

    Logger log("GameRoomThread");
    log.Log(LOGLEVEL::DEBUG, "GameRoom Thread Start - FrameTime: %lf", frameTime);
    
    while(true){
        gettimeofday(&serverTime, NULL);
        interval = getTimeDist(&beforeTime, &serverTime);
        if(interval > frameTime){
            log.Log(LOGLEVEL::INFO, "Frame FrameTime: %lf, interval: %lf", frameTime, interval);

            gameRoomManager->UpdateGameRooms(serverTime);

            beforeTime = serverTime;
        }
    }

    return;
}

size_t DisconnectPacketFactory(void* buf)
{
    constexpr TCPTestPacketHeader header = {TCP_PACKET_START_CODE, sizeof(DisconnectData) + sizeof(unsigned char),
        DISCONNECT, DISCONNECT_OK, 0x000, 0x000};
    constexpr DisconnectData data = {0x00};

    return MakePacket(buf, &header, &data, sizeof(data), TCP_PACKET_END_CODE);
}