#include "thread_pool.hpp"

using namespace ns_threadpool;
using namespace Ns_task;
int main()
{
    ThreadPool<Task>* tp = new ThreadPool<Task>(10);
    tp->InitThreadPool();
    srand((unsigned int)time(nullptr));
    while (true)
    {
        // ���߳�push���� ��ʵ����� ����һ���Ǵ���������
        Task t(rand() % 20 + 1, rand() % 10 + 1, "+-*/%"[rand() % 5]);
        tp->pushtask(t);
        // sleep(1);
    }

    return 0;
}