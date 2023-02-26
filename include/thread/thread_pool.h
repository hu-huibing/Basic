#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <future>
#include <functional>
#include <memory>

namespace Basic 
{

    class CThreadPool
    {
        CThreadPool(CThreadPool&)=delete;
        CThreadPool& operator=(CThreadPool&)=delete;
    public:
        CThreadPool(size_t thread_size);
        ~CThreadPool();

        void async_invoker(std::function<void()> task);
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    template <typename F, typename ...Args>
    void async_invoker(CThreadPool& thread_pool, F&& f, Args&&... args)
    {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        thread_pool.async_invoker(task);
    }
    
    template <typename F, typename ...Args>
    auto async_invoker_result(CThreadPool& thread_pool, F&& f, Args&&... args)
        ->std::future<typename std::result_of<F(Args...)>::type>
    {
        using result_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<result_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<result_type> future = task->get_future();
        {
            thread_pool.async_invoker([task](){(*task)();});
        }
        return future;
    }
   
}

#endif
