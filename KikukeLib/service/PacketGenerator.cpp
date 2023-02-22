#include <string.h>

#include "PacketGenerator.h"

template <typename T1, typename T2>
size_t MakePacket(void* buf, T1 header, T2 data, unsigned char endCode)
{
    size_t packetLen = 0;

    memcpy(buf + packetLen, header, sizeof(header));
    packetLen += sizeof(header);

    memcpy(buf + packetLen, data, sizeof(data));
    packetLen += sizeof(data);

    memcpy(buf + packetLen, endCode, sizeof(endCode));
    packetLen += sizeof(endCode);

    return packetLen;
}