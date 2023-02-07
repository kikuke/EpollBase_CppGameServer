#include "sptype.h"
#include "spbitio.h"
#include "PacketParser.h"

int CheckStartCode(unsigned int startCode, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize)
{
    if(bufUseSize*BYTE_BIT < eBitIdx)
        return PACKET_HANDLER_WORKING;

    if(startCode != BitsToUInt(sBitIdx, eBitIdx, buf, bufUseSize))
        return PACKET_HANDLER_BROKEN;

    return PACKET_HANDLER_RECEIVED;
}

size_t GetPacketLength(size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize)
{
    if(bufUseSize*BYTE_BIT < eBitIdx)
        return PACKET_HANDLER_WORKING;
    
    return BitsToUInt(sBitIdx, eBitIdx, buf, bufUseSize);
}

int CheckEndCode(unsigned int endCode, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize)
{
    return CheckStartCode(endCode, sBitIdx, eBitIdx, buf, bufUseSize);
}