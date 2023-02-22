#ifndef PACKET_IO
#define PACKET_IO

#include <sys/types.h>

#include "RingBuffer.h"

ssize_t WriteRingBuffer(int fd, const void* src_buf, size_t size);
ssize_t ReadETRingBuffer(int fd, void* buf, size_t sz);

#endif