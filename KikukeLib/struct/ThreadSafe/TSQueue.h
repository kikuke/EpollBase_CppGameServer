#ifndef TSQUEUE
#define TSQUEUE

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class TSQueue//Todo: 더 좋은방법 찾아보기. 좀더 공부해보기
{
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;

public:
    void push(T item)
    {
        // Acquire lock
        //이 객체가 소멸되면 락이 해제되는 유니크 락.
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(item);

        // Notify one thread that
        // is waiting 자고있는것 중 아무나 하나 깨움
        m_cond.notify_one();
    }

    T pop()
    {
        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // wait until queue is not empty
        // false를 반환하면 락 풀고 스레드 휴식상태에 들어감. 깨워서 true가 되면 락걸고 작업처리
        m_cond.wait(lock,[this](){ return !m_queue.empty(); });

        T item = m_queue.front();
        m_queue.pop();

        return item;
    }
};

#endif