#include "SocketManager.h"
#include "TcpPacketHandler.h"

void TcpPacketHandler::ExecuteOP(unsigned int mainOp, unsigned int subOp)
{
    for(int i=0; i<handler_size; i++){
        if((*(handlers[i])).getMainOp() == mainOp){
            (*(handlers[i])).execute(subOp, readBuf);
            return;
        }
    }
    
    //에러처리. 여기 밑으로는 에러임.

    /*
    각 응답 클래스의 run인자에는 RingBuffer를 이용해 빼내는 함수도 있음?
    각 응답코드별 응답 구조체 또는 클래스를 만들어서 그대로 빼내기.
    응답 클래스에는 응답 구조체도 들어있음 응답 구조체를 이용해 한번에 빼내기.
    응답 구조체는 get()같은 상위 클래스의 메서드를 이용해 빼낼 수 있음.
    상위 클래스에 run 같은 느낌으로 실행되는 것들도 만들기
    */
}

int TcpPacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    TCPTestPacketHeader header;

    int useSz = info->recvBuffer.getUseSize();

    if(sizeof(TCPTestPacketHeader) > useSz)
        return 0;

    info->recvBuffer.peek(&header, sizeof(header));
    if(header.packetLen > useSz)
        return 0;

    if(!TCPHeaderCheck(&header)){
        //에러 처리 코드들.
        return 0;
    }
    
    info->recvBuffer >> header;
    readBuf.flush();
    readBuf << info->recvBuffer;

    ExecuteOP(header.mainOp, header.subOp);

    readBuf >> info->recvBuffer;

    return 1;
}

int TcpPacketHandler::TCPHeaderCheck(TCPTestPacketHeader* header)
{
    //인증, 인가, 플래그 체크 등등의 작업하기
    if(!(header->startCode == TCP_PACKET_START_CODE))
        return 0;

    return 1;
}