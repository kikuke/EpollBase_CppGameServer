#include "PacketHandler.h"

size_t GetPacketLength(unsigned char startCode, size_t sLenBitIdx, size_t eLenBitIdx, const void* buf, size_t bufUseSize)
{
    if(bufUseSize <= eLenBitIdx/sizeof(unsigned char))
        return PACKET_HANDLER_WORKING;
    
    size_t bitLen = eLenBitIdx - sLenBitIdx + 1;
    
    //비트단위 계산
}


int CheckPacketLength(size_t packetLen, unsigned char endCode, const void* buf, size_t bufUseSize)
{

}