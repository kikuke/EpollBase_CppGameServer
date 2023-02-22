#ifndef PACKET_GENERATOR
#define PACKET_GENERATOR

#include <sys/types.h>

//return packet length
template <typename T1, typename T2>
size_t MakePacket(void* buf, T1 header, T2 data, unsigned char endCode);

#endif