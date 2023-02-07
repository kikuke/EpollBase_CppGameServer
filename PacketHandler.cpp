#include "PacketDefine.h"
#include "SocketManager.h"
#include "IPacketHandler.h"
#include "PacketHandler.h"

int PacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    if(TCPHeaderCheck(info)!=1){
        //에러 처리 코드들.
        //나중에 상수들 매크로로 바꾸기
        //헤더를 밖으로 꺼내서 쓸지 생각중
        return 0;
    }
    
    readBuf.flush();
    readBuf << info->recvBuffer;

    //헤더도 같이 넘겨줄 것인가 고민
    //case같은 느낌으로 헤더의 OP따라 핸들링 하기

    readBuf >> info->recvBuffer;

    IPacketHandler temp;
    temp.execute(readBuf);//내부에선 이런식으로 알아서 빼고 쓰다남은건 다시 돌려줌.
    
    /*
    각 응답 클래스의 run인자에는 RingBuffer를 이용해 빼내는 함수도 있음?
    각 응답코드별 응답 구조체 또는 클래스를 만들어서 그대로 빼내기.
    응답 클래스에는 응답 구조체도 들어있음 응답 구조체를 이용해 한번에 빼내기.
    응답 구조체는 get()같은 상위 클래스의 메서드를 이용해 빼낼 수 있음.
    상위 클래스에 run 같은 느낌으로 실행되는 것들도 만들기
    */

    return 1;
}

int PacketHandler::TCPHeaderCheck(TCPSOCKETINFO* info)
{
    TCPTestPacketHeader header;

    int useSz = info->recvBuffer.getUseSize();

    if(sizeof(TCPTestPacketHeader) > useSz)
        return 0;

    info->recvBuffer.peek(&header, sizeof(header));
    if(header.packetLen > useSz)
        return 0;

    return 1;
}