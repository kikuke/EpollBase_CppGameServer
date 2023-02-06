#ifndef PACKET_IO
#define PACKET_IO

#include <sys/types.h>

#include "RingBuffer.h"

ssize_t WriteRingBuffer(int fd, const void* src_buf, size_t size);
//패킷의 길이반환, 만들기 등을 해주는 객체생성기도 만들기

#endif