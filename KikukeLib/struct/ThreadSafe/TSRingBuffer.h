#ifndef TSRINGBUFFER
#define TSRINGBUFFER

#include <mutex>

#include "RingBuffer.h"

class TSRingBuffer : public RingBuffer
{
private:
    std::recursive_mutex m_mutex;

public:

    virtual size_t getUseSize()
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        return RingBuffer::getUseSize();
    }

    virtual size_t peek(void* dest_buf, size_t size)
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        RingBuffer::peek(dest_buf, size);
    }

    virtual size_t enqueue(const void* src_buf, size_t size)
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        RingBuffer::enqueue(src_buf, size);
    }

    virtual size_t dequeue(void* dest_buf, size_t size)
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        RingBuffer::dequeue(dest_buf, size);
    }

    //flush and reuse. no destroy.
    virtual void flush() override
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        RingBuffer::flush();
    }

    virtual RingBuffer& operator <<(RingBuffer& data) override
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        return RingBuffer::operator<<(data);
    }

    virtual RingBuffer& operator >>(RingBuffer& data) override
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        return RingBuffer::operator>>(data);
    }
};

#endif