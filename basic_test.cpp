#include "thread/thread_pool.h"
#include <iostream>
#include <chrono>

void thread_pool_test()
{
    std::cout << "thread pool test" << std::endl;
    Basic::CThreadPool pool(4);
    auto async_call = [](){
        std::cout << "sync call" << std::endl;
    };
    auto async_call_params = [](int a, int b){
        std::cout << "sync call with param" << std::endl;
    };
    auto async_call_return = [](int a, int b){
        std::cout << "sync call with return" << std::endl;
        return a + b;
    };
    pool.async_invoker(async_call);
    Basic::async_invoker(pool, async_call_params, 1, 2);
    auto ret = Basic::async_invoker_result(pool, async_call_return, 1, 2);
    std::cout << "do something" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "sync call with return, ret: " << ret.get() << std::endl;
}

int main(void)
{
    thread_pool_test();
    return 0;
}