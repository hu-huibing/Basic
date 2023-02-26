#include "thread/thread_pool.h"
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>

namespace Basic
{
    #define m_stop m_impl->mm_stop
    #define m_mutex m_impl->mm_mutex
    #define m_condition m_impl->mm_condition
    #define m_workers m_impl->mm_workers
    #define m_tasks m_impl->mm_tasks

    struct CThreadPool::Impl
    {
        Impl(bool stop)
        :mm_stop(stop){};

        bool                                mm_stop;
        std::mutex                          mm_mutex;
        std::condition_variable             mm_condition;
        std::vector<std::thread>            mm_workers;
        std::queue<std::function<void()>>   mm_tasks;
    };

    CThreadPool::CThreadPool(size_t thread_size)
    {
        m_impl.reset(new Impl(false));
        for (size_t i = 0; i < thread_size; ++i)
        {
            m_workers.emplace_back([this](){
                for(;;)
                {
                    auto task = std::function<void()>();
                    {
                        std::unique_lock<std::mutex> lock(this->m_mutex);
                        this->m_condition.wait(lock, [this](){
                            return this->m_stop || !m_tasks.empty();
                        });
                        if (this->m_stop && this->m_tasks.empty())
                            return;
                        task = this->m_tasks.front();
                        this->m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    CThreadPool::~CThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (auto& work : m_workers)
        {
            work.join();
        }
    }

    void CThreadPool::async_invoker(std::function<void()> task)
    {
        if (task)
        {
            {
                std::unique_lock<std::mutex> lock(this->m_mutex);
                m_tasks.emplace(task); 
            }
            m_condition.notify_one();
        }
    }

}