#ifndef PACKET_COLLECTOR
#define PACKET_COLLECTOR

#include <sys/types.h>

#include "RingBuffer.h"

ssize_t WriteRingBuffer(int fd, const void* src_buf, size_t size);

#endif