// hpp文件在开源库中常见 不同于普通的头文件.h 它可以做到模板类的声明和定义放在一起 
#include "BlockQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace ns_blockqueue;

pthread_mutex_t Pprintmtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t Cprintmtx = PTHREAD_MUTEX_INITIALIZER; 

void* producter(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        // 生产者生产的慢一点
        // sleep(2);
        int data = rand() % 20 + 1;
        bq->push(data);
        pthread_mutex_lock(&Pprintmtx);
        cout << "生产者生产了一个数据:" << data << endl;
        pthread_mutex_unlock(&Pprintmtx);
    }
}


void* consumer(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        sleep(2);
        int data;
        bq->pop(&data);
        pthread_mutex_lock(&Cprintmtx);
        cout << "消费者消费了一个数据:" << data << endl;
        pthread_mutex_unlock(&Cprintmtx);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c[10], p[10];
    for (int i = 0; i < 10; ++i)
    {
        pthread_create(&p[i], nullptr, producter, (void*)bq);
        pthread_create(&c[i], nullptr, consumer, (void*)bq);
    }
    for (int i = 0; i < 10; ++i)
    {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }
    delete bq;
}