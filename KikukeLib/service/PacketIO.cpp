#include <unistd.h>

#include "SocketManager.h"
#include "PacketIO.h"

ssize_t WriteRingBuffer(int fd, const void* src_buf, size_t size)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(fd);

    return (*((*info).recvBuffer)).enqueue(src_buf, size);
}

ssize_t ReadETRingBuffer(int fd, void* buf, size_t sz)
{
    ssize_t str_len = 0;
    ssize_t temp_len;

    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(fd);

    while(1)
    {
        temp_len = read(fd, buf,  sz);
        if(temp_len == 0){
            return 0;
        }
        else if(temp_len < 0)
        {
            if(errno == EAGAIN)
                return str_len;
        }
        else
        {
            str_len += (*((*info).recvBuffer)).enqueue(buf, temp_len);
        }
    }
}