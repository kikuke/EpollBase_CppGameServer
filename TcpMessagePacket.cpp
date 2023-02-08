#include "TcpMessagePacket.h"

int TcpMessagePacket::MessageEcho(MessageEchoData meData)
{
    switch (meData.target)
    {
    case MESSAGE_ECHO_DATA_TARGET_SELF:
        //혼잣말에 관한 전송 처리
        break;
    case MESSAGE_ECHO_DATA_TARGET_ALL:
        //전체말에 관한 전송 처리
        break;
    default:
        //error
        return 0;
        break;
    }

    return 1;
}