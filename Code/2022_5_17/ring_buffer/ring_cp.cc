#include "ring_queue.hpp"
#include "task.hpp"
using namespace ns_ring_queue;
using namespace Ns_task;
const int num = 5;
std::mutex mtx;  // 打印的锁
//std::mutex pmtx; // 生产者的锁 不能多个生产者同时生产
// std::mutex cmtx; // 消费者的锁 不能多个消费者同时消费

void *producter(void *args)
{
    auto *rq = (RingQueue<Task> *)args;
    const string ops = "+-*/%";
    while (true)
    {
        // rq->pop();
        //pmtx.lock();
        int x = rand() % 20 + 1;
        int y = rand() % 20 + 1;
        char op = ops[rand() % 5];
        Task t(x, y, op);
        mtx.lock();
        std::cout << "线程" << pthread_self() << "生产数据的是" << t.show() << std::endl;
        mtx.unlock();
        rq->push(t);
        // pmtx.unlock();
        sleep(2);
    }
}

void *consumer(void *args)
{
    auto *rq = (RingQueue<Task> *)args;
    while (true)
    {
        // rq->push();
        // cmtx.lock();
        Task t;
        rq->pop(&t);
        mtx.lock();
        // std::cout << "线程" << pthread_self() << "消费数据是" << << t() << std::endl;
        t.Run();
        mtx.unlock();
        // cmtx.unlock();
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t c[num], p[num];
    auto *rq = new RingQueue<Task>();
    for (int i = 0; i < num; ++i)
    {
        pthread_create(c + i, nullptr, consumer, (void *)rq);
        pthread_create(p + i, nullptr, producter, (void *)rq);
    }
    for (int i = 0; i < num; ++i)
    {
        pthread_join(c[i], nullptr);
        pthread_join(p[i], nullptr);
    }
    return 0;
}