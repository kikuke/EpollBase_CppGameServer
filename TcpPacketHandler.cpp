#include "SocketManager.h"
#include "TcpPacketHandler.h"

int TcpPacketHandler::ExecuteOP(int sock, unsigned int mainOp, unsigned int subOp)
{
    for(int i=0; i<handler_size; i++){
        if((*(handlers[i])).getMainOp() == mainOp)
            return (*(handlers[i])).execute(sock, subOp, readBuf);
    }

   return 0;
}

int TcpPacketHandler::CatchError(int sock, unsigned int mainOp, unsigned int errorCode)
{
    for(int i=0; i<handler_size; i++){
        if((*(handlers[i])).getMainOp() == mainOp)
            return (*(handlers[i])).catchError(sock, errorCode);
    }

    return 0;
}

int TcpPacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    TCPTestPacketHeader header;
    int ret;

    int useSz = info->recvBuffer.getUseSize();

    if(sizeof(TCPTestPacketHeader) > useSz)
        return 0;

    info->recvBuffer.peek(&header, sizeof(header));
    if(header.packetLen > useSz)
        return 0;

    ret = TCPHeaderCheck(&header);
    if(ret != 1){
        (*log).Log(LOGLEVEL::ERROR, "TCPHeaderChcek()");
        return 0;
    }
    
    info->recvBuffer >> header;
    readBuf.flush();
    readBuf << info->recvBuffer;

    ret = ExecuteOP(sock, header.mainOp, header.subOp);
    if(ret != 1){
        (*log).Log(LOGLEVEL::ERROR, "ExecuteOP()");
        //해당 소켓의 버퍼는 비게됨.
        //해당 오류에 대한 해당 기능의 에러 처리
        CatchError(sock, header.mainOp, ret);
        return 0;
    }

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