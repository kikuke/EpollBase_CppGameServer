#include <string.h>

#include "RingBuffer.h"

RingBuffer::RingBuffer()
{
    buf_sz = RINGBUFFER_DEFAULT_SIZE;
    buffer = new unsigned char[buf_sz];
    InitBufPointer();
}

RingBuffer::RingBuffer(int buf_sz)
{
    this->buf_sz = buf_sz;
    buffer = new unsigned char[buf_sz];
    InitBufPointer();
}

RingBuffer::~RingBuffer()
{
    delete[] buffer;
    buffer = nullptr;
}

size_t RingBuffer::peek(void* dest_buf, size_t size)
{
    size_t read_sz = getNotBrokenGetSize();

    if(size == 0) return 0;
    if(size > getUseSize())
        size = getUseSize();

    memcpy(dest_buf, buffer + front, size < read_sz ? size : read_sz);
    if(size > read_sz){
        memcpy((unsigned char*)dest_buf + read_sz, buffer, size - read_sz);
    }

    return size;
}

size_t RingBuffer::enqueue(const void* src_buf, size_t size)
{
    size_t write_sz;

    if(size == 0) return 0;
    if(size > getRemainSize()){
        DoubleBuffer(size);
    }
    write_sz = getNotBrokenPutSize();
    write_sz = size < write_sz ? size : write_sz;

    memcpy(buffer + rear, src_buf, write_sz);
    if(size > write_sz){
        memcpy(buffer, (unsigned char*)src_buf + write_sz, size - write_sz);
    }

    rear = getNextPointerMove(rear, size);

    return size;
}

size_t RingBuffer::dequeue(void* dest_buf, size_t size)
{
    if(size == 0) return 0;
    if(size > getUseSize())
        size = getUseSize();
    peek(dest_buf, size);

    front = getNextPointerMove(front, size);

    return size;
}

void RingBuffer::flush()
{
    InitBufPointer();
}

size_t RingBuffer::getNotBrokenGetSize()
{
    if(rear >= front)
        return rear - front;
    return buf_sz - front;
}

size_t RingBuffer::getNotBrokenPutSize()
{
    if(rear >= front)
        return buf_sz - rear;
    return front - rear - 1;
}

size_t RingBuffer::getBufferSize()
{
    return sizeof(unsigned char) * buf_sz;
}

size_t RingBuffer::getUseSize()
{
    if(rear >= front)
        return rear - front;
    return buf_sz - front + rear;
}

size_t RingBuffer::getRemainSize()
{
    return getBufferSize() - getUseSize() - 1;
}

void RingBuffer::InitBufPointer()
{
    front = 0;
    rear = 0;
}

size_t RingBuffer::getNextPointerMove(size_t idx, size_t size)
{
    return (idx + size) % buf_sz;
}

size_t RingBuffer::DoubleBuffer(size_t need_sz)
{
    size_t use_sz = getUseSize();
    size_t newBuf_sz = buf_sz * (need_sz/buf_sz + 2);
    unsigned char* newBuf;

    newBuf = new unsigned char[newBuf_sz];
    peek(newBuf, use_sz);

    delete[] buffer;
    buffer = newBuf;
    buf_sz = newBuf_sz;
    front = 0;
    rear = use_sz;

    return buf_sz;
}