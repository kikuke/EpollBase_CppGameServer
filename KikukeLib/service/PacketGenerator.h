#ifndef PACKET_GENERATOR
#define PACKET_GENERATOR

#include <sys/types.h>
#include <string.h>

//return packet length
template <typename T1, typename T2>
size_t MakePacket(void* buf, T1* header, T2* data, size_t data_sz, unsigned char endCode)
{
    size_t packetLen = 0;

    memcpy((unsigned char*)buf + packetLen, header, sizeof(*header));
    packetLen += sizeof(*header);

    memcpy((unsigned char*)buf + packetLen, data, data_sz);
    packetLen += data_sz;

    memcpy((unsigned char*)buf + packetLen, &endCode, sizeof(endCode));
    packetLen += sizeof(endCode);

    return packetLen;
}

#endif