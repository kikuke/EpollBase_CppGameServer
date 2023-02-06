#ifndef PACKET_HANDLER
#define PACKET_HANDLER

#include <sys/types.h>

//Enable TCP/UDP

//CheckPacket Value
#define PACKET_HANDLER_WORKING 0
#define PACKET_HANDLER_RECEIVED 1
#define PACKET_HANDLER_BROKEN -1

//return PacketLength and CheckPacketValue.
//packetLength include header, trailer
size_t GetPacketLength(unsigned char startCode, size_t sLenBitIdx, size_t eLenBitIdx, const void* buf, size_t bufUseSize);

//return CheckPacket Value
//packetLength include header, trailer
int CheckPacketLength(size_t packetLen, unsigned char endCode, const void* buf, size_t bufUseSize);

//패킷 처리하는 함수 고민해보기

#endif