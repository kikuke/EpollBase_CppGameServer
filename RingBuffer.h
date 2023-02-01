#ifndef RING_BUFFER
#define RING_BUFFER

#include <stddef.h>

#define RINGBUFFER_DEFAULT_SIZE 32768
class RingBuffer
{
private:
    //buffer pointer
    unsigned char* buffer;
    //buffer size
    size_t buf_sz;
    //buffer's writing idx
    size_t front;
    //buffer's reading idx
    size_t rear;

    void InitBufPointer();
    size_t DoubleBuffer(size_t need_sz);

    size_t getBufferSize();
    size_t getUseSize();
    size_t getRemainSize();

    size_t getNotBrokenGetSize();
    size_t getNotBrokenPutSize();

public:
    RingBuffer();
    RingBuffer(int buf_sz);
    ~RingBuffer();

    size_t peek(void* dest_buf, size_t size);
    size_t enqueue(void* src_buf, size_t size);
    size_t dequeue(void* dest_buf, size_t size);

    //flush and reuse. no destroy.
    void flush();
};

#endif