#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <mutex>// C++11语言支持的多线程的锁

using namespace std;
// 抢票逻辑 10000票 同时抢
// tickets 就是临界资源:tickets--是原子的吗(也就是说对tickets--是安全的吗)
// 为了让多个线程进行切换，线程什么时候会切换呢？(1.时间片到了;2.检测的时间点:从内核态返回用户态的时候)
// int tickets = 1000;

class Ticket
{
public:
    // 构造函数 初始化一个锁
    Ticket() : tickets(10000)
    {
        // pthread_mutex_init(&mxt, nullptr);
    }

    // 析构函数 销毁这个锁
    ~Ticket()
    {
        //pthread_mutex_destroy(&mxt);
    }

    // 抢票 抢票过程加锁解锁
    bool GetTicket()
    {
        // 加锁
        //pthread_mutex_lock(&mxt);
        mymtx.lock();
        // mymtx.lock();
        bool res = true;
        if (tickets > 0)
        {
            usleep(1000);
            cout << "我是[" << pthread_self() << "] 我要抢的票是:" << tickets << endl;
            printf("");
            tickets--;
        }
        else 
        {
            printf("已经没有票可以抢了\n");
            res = false;
        }
        // 解锁
        // pthread_mutex_unlock(&mxt);
        mymtx.unlock();
        return res;
    }
private:
    int tickets;
    //pthread_mutex_t mxt;
    mutex mymtx;
};

void *ThreadRun(void *args)
{
    Ticket* t = (Ticket*)args;
    int cnt = 0;
    while (true)
    {
        usleep(10000);
        if (t->GetTicket()) 
        {
            ++cnt;
        }
        else break;
    }
    cout << "我是" << pthread_self() << "线程," << "我抢了" << cnt << "张票" << endl;
    return nullptr;
}

int main()
{
    Ticket *t = new Ticket();
    pthread_t tid[5];
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(tid + i, nullptr, ThreadRun, (void *)t);
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tid[i], nullptr);
    }
    delete t;
    return 0;
}