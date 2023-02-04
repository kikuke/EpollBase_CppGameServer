#include "SocketManager.h"
#include "PacketCollector.h"

ssize_t WriteRingBuffer(int fd, const void* src_buf, size_t size)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(fd);

    return (*info).recvBuffer.enqueue(src_buf, size);
}