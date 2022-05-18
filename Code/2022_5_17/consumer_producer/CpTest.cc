// hpp文件在开源库中常见 不同于普通的头文件.h 它可以做到模板类的声明和定义放在一起 
#include "BlockQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "task.hpp"
using namespace ns_blockqueue;
using namespace Ns_task;
pthread_mutex_t Pprintmtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t Cprintmtx = PTHREAD_MUTEX_INITIALIZER; 

void* producter(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;
    string ops = "+-*/%";
    while (true)
    {
        // 生产者生产的慢一点
        // sleep(2);
        /*int data = rand() % 20 + 1;
        bq->push(data);
        pthread_mutex_lock(&Pprintmtx);
        cout << "生产者生产了一个数据:" << data << endl;
        pthread_mutex_unlock(&Pprintmtx);*/

        // 构造一个任务
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = ops[rand() % 5];
        Task t(x, y, op);
        bq->push(t);
        cout << "生产者生产了一个任务" << endl;
        sleep(1);
    }
}


void* consumer(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;
    while (true)
    {
        // sleep(2);
        // int data;
        // bq->pop(&data);
        // pthread_mutex_lock(&Cprintmtx);
        // cout << "消费者消费了一个数据:" << data << endl;
        // pthread_mutex_unlock(&Cprintmtx);
        Task t;
        bq->pop(&t);// 取出了任务
        int res = t();
        cout << pthread_self() << "线程已经解决问题:" << res << endl;
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c[1], p[1];
    for (int i = 0; i < 1; ++i)
    {
        pthread_create(&p[i], nullptr, producter, (void*)bq);
        pthread_create(&c[i], nullptr, consumer, (void*)bq);
    }
    for (int i = 0; i < 1; ++i)
    {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }
    delete bq;
}