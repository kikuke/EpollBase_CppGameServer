#ifndef PACKET_PARSER
#define PACKET_PARSER

#include <sys/types.h>

//Enable TCP/UDP

//CheckPacket Value
#define PACKET_HANDLER_WORKING 0
#define PACKET_HANDLER_RECEIVED 1
#define PACKET_HANDLER_BROKEN -1

int CheckStartCode(unsigned int startCode, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

//return PacketLength and CheckPacketValue.
//packetLength include header, trailer
size_t GetPacketLength(size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

//return CheckPacket Value
//packetLength include header, trailer
int CheckEndCode(unsigned int endCode, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

unsigned int BitsToUInt(size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

int CheckBitMask(unsigned int bitMask, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

size_t BitsToBools(size_t sBitIdx, int* boolVars, size_t boolSize, const void* srcbuf, size_t srcBufUseSize);

#endif