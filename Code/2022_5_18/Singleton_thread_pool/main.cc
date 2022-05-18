#include "thread_pool.hpp"

using namespace ns_threadpool;
using namespace Ns_task;
int main()
{
    // ThreadPool<Task>* tp = new ThreadPool<Task>(10);
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    std::cout << "当前正在运行我的其他代码:" << std::endl;
    sleep(5);
    // tp->InitThreadPool();
    srand((unsigned int)time(nullptr));
    while (true)
    {
        // 主线程push任务 真实情况中 任务一般是从网络来的
        Task t(rand() % 20 + 1, rand() % 10 + 1, "+-*/%"[rand() % 5]);
        // tp->pushtask(t);
        // 获取单例即可
        ThreadPool<Task>::GetInstance()->pushtask(t);
        sleep(1);
    }

    return 0;
}