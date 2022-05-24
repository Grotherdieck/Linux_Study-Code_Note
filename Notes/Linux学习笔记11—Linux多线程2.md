# 一、线程池

## 1 线程池的概念

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518195102.png" style="zoom:80%;" />

&emsp;&emsp;线程池也是类似的概念。

&emsp;&emsp;当任务到来时，系统一般会创建一个线程去完成对应的任务，虽然创建线程的成本并不高，但是如果没有池化技术不就相当于在需要内存时才向os要吗，所以类似的，我们也应该先创建一堆线程放到池子里等着。

&emsp;&emsp;提前创建好的一批线程，用来随时处理任务，这个东西就是线程池。

## 2 线程池简易实现

&emsp;&emsp;外部有多个线程往任务队列里放任务、线程池内部有多个线程竞争的往任务队列中拿任务，所以任务队列是临界资源。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518195750.png" style="zoom:80%;" />

&emsp;&emsp;代码如下：

```cpp
// thread_pool.hpp
#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "task.hpp"

namespace ns_threadpool
{
    const int g_num = 5;
    template <class T>
    class ThreadPool
    {
    private:
        int num_;// 线程池中的线程数量
        std::queue<T> task_queue_;
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
    public:
        // 封装一下上锁
        void Lock()
        {
            pthread_mutex_lock(&mtx_);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&mtx_);
        }

        bool isEmpty()
        {
            return task_queue_.empty();
        }
        void WakeUp()
        {
            pthread_cond_signal(&cond_);
        }
    private:
        void poptask(T* out)
        {
            *out = task_queue_.front();
            task_queue_.pop();
        }
    public:
        ThreadPool(int num = g_num) : num_(num)
        {
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&cond_, nullptr);
        }
        // 在类中，要让线程执行类内成员方法是不可行的
        // 因为pthread_create的语法是void* function(void*)
        // 而非静态类成员方法有this指针
        // 所以我们需要调用静态方法
        static void* Rountine(void* args)
        {
            ThreadPool<T>* tp = (ThreadPool<T>*)args;
            pthread_detach(pthread_self());
            while (true)
            {
                // 从任务队列中拿任务 为了能拿任务队列里的东西，需要传递this指针
                tp->Lock();
                while (tp->isEmpty()) 
                {
                    // 任务队列为空 线程池的线程应该做什么呢？应该让线程挂起 
                    // 利用条件变量
                    tp->wait();
                }
                T t;
                tp->poptask(&t);

                tp->Unlock();
                // 到锁外处理任务
                t();
                // sleep(1);
            }
        }
        void InitThreadPool()
        {
            pthread_t tid;
            for (int i = 0; i < num_; ++i)
            {
                pthread_create(&tid, nullptr, Rountine, (void*)this/*?*/);
            }
        }
        
        // 向任务队列放任务
        void pushtask(const T& in)
        {
            Lock();

            task_queue_.push(in);

            Unlock();
            WakeUp();
        }

        void wait()
        {
            pthread_cond_wait(&cond_,&mtx_);
        }

        ~ThreadPool() 
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }
    };
}

// main.cc
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
        // 主线程push任务 真实情况中 任务一般是从网络来的
        Task t(rand() % 20 + 1, rand() % 10 + 1, "+-*/%"[rand() % 5]);
        tp->pushtask(t);
        // sleep(1);
    }

    return 0;
}
```

# 二、单例模式

## 1 什么是单例模式

&emsp;&emsp;IT行业火了很多年, 涌入的人很多. 俗话说林子大了啥鸟都有. 大佬和菜鸡们(比如我)两极分化的越来越严重。为了让菜鸡们不太拖大 佬的后腿, 于是大佬们针对一些经典的常见的场景, 给定了一些对应的解决方案, 这个就是设计模式，可以理解为编程经验的提炼总结。

&emsp;&emsp;单例模式就是一种“经典的”设计模式。

## 2 单例模式的特点

&emsp;&emsp;某些类只能具有一个对象，就称为单例。

&emsp;&emsp;例如服务器开发场景，经常需要让服务器加载很多数据到内存中，此时往往需要用一个单例来管理这些数据，总而言之，我们只希望这个类创建的对象只有一个(比如动态库的存在，只需要存在一份)。

&emsp;&emsp;C/C++中，定义一个对象需要两步：开辟空间、给空间写入初始值，定义对象的本质就是把对象的数据载入内存。

&emsp;&emsp;所以单例模式实际上就是让一个类只在内存创建一个对象，即这个对象只在内存中加载一次。

&emsp;&emsp;那么什么时候创建对象呢？饿汉模式/懒汉模式。

&emsp;&emsp;一般而言，我们的对象被设计为单例的常见情景是：

- 语义上只需要一个；
- 该对象内部存在大量空间，保存了大量的数据，如果允许该对象存在多份，则内存中会有大量冗余的数据。

## 3 饿汉模式和懒汉模式

&emsp;&emsp;吃完饭, 立刻洗碗, 这种就是饿汉方式. 因为下一顿吃的时候可以立刻拿着碗就能吃饭. 

&emsp;&emsp;饿汉模式的核心就是创建就加载数据。

&emsp;&emsp;饿汉模式的伪代码：

```cpp
template <class T>
class Singleton
{
private:
    static T data_;
public:
    static T* GetInstance()
    {
        return &data_;
    }
};
// 类外的data_的初始化
```

&emsp;&emsp;吃完饭, 先把碗放下, 然后下一顿饭用到这个碗了再洗碗, 就是懒汉方式.

&emsp;&emsp;懒汉模式的核心思想是延时加载，到对象用的时候我才创建，这和父子进程的地址空间的写时拷贝类似。

&emsp;&emsp;懒汉模式的伪代码：

```cpp
template <class T>
class Singleton
{
private:
    static T* inst;
public:
    Singleton() : inst(nullptr) {}
    static T* GetInstance()
    {
        if (init == nullptr) inst = new T();
        return inst;
    }
};

```

## 4 懒汉方式的单例模式线程池

&emsp;&emsp;线程池这个模板类显然就是符合单例模式的，因此我们实现一个单例模式版的线程池，在类对象中装一个``ThreadPool<T>*``，然后它的初始化放在类外初始化，然后通过一个``GetInstance``获得单例即可。

```cpp
// thread_pool.hpp
#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "task.hpp"

namespace ns_threadpool
{
    const int g_num = 5;
    template <class T>
    class ThreadPool
    {
    private:
        int num_;// 线程池中的线程数量
        std::queue<T> task_queue_;
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
        static ThreadPool<T>* ins;//创建类内对象的指针

    public:
        static ThreadPool<T>* GetInstance()
        {
            if (ins == nullptr) {
                // 当前单例对象尚未被创建
                ins = new ThreadPool<T>();
                ins->InitThreadPool();
                std::cout << "首次加载对象" << std::endl;
            }
            return ins;
        }
        // 封装一下上锁
        void Lock()
        {
            pthread_mutex_lock(&mtx_);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&mtx_);
        }

        bool isEmpty()
        {
            return task_queue_.empty();
        }
        void WakeUp()
        {
            pthread_cond_signal(&cond_);
        }
    private:
        void poptask(T* out)
        {
            *out = task_queue_.front();
            task_queue_.pop();
        }
    private:
        // 构造函数弄成私有的 不能类外构造对象
        ThreadPool(int num = g_num) : num_(num)
        {
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&cond_, nullptr);
        }
        // 在类中，要让线程执行类内成员方法是不可行的
        // 因为pthread_create的语法是void* function(void*)
        // 而非静态类成员方法有this指针
        // 所以我们需要调用静态方法

        // 单例模式 删掉拷贝构造和赋值
        ThreadPool(const ThreadPool<T>& tp) = delete;
        ThreadPool<T>& operator=(const ThreadPool<T>& tp) = delete;
    public:
        static void* Rountine(void* args)
        {
            ThreadPool<T>* tp = (ThreadPool<T>*)args;
            pthread_detach(pthread_self());
            while (true)
            {
                // 从任务队列中拿任务 为了能拿任务队列里的东西，需要传递this指针
                tp->Lock();
                while (tp->isEmpty()) 
                {
                    // 任务队列为空 线程池的线程应该做什么呢？应该让线程挂起 
                    // 利用条件变量
                    tp->wait();
                }
                T t;
                tp->poptask(&t);

                tp->Unlock();
                // 到锁外处理任务
                t();
                // sleep(1);
            }
        }
        void InitThreadPool()
        {
            pthread_t tid;
            for (int i = 0; i < num_; ++i)
            {
                pthread_create(&tid, nullptr, Rountine, (void*)this/*?*/);
            }
        }
        
        // 向任务队列放任务
        void pushtask(const T& in)
        {
            Lock();

            task_queue_.push(in);

            Unlock();
            WakeUp();
        }

        void wait()
        {
            pthread_cond_wait(&cond_,&mtx_);
        }

        ~ThreadPool() 
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }
    };
    // 类外初始化静态成员
    template<class T>
    ThreadPool<T>* ThreadPool<T>::ins = nullptr;
}

// main.cc
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
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518213933.png" style="zoom:80%;" />

## 5 单例模式的线程安全问题

&emsp;&emsp;多线程情况下，如果多线程同时进入``GetInstance``函数，不就会创建好多个对象了吗？

&emsp;&emsp;所以增加一把锁就好了，把这个函数给锁住。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518214408.png" style="zoom:80%;" />

&emsp;&emsp;但是这样的话后续访问ins是串行的，不是并行的，但是实际上我们希望并行的访问ins，所以可以增加一个**双判定**，减少锁的征用，提高获得单例的效率。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518215029.png" style="zoom:80%;" />

# 三、STL、智能指针与线程安全

## 1 STL中的容器是否是线程安全

&emsp;&emsp;不是. 原因是, ``STL``的设计初衷是将性能挖掘到极致, 而一旦涉及到加锁保证线程安全, 会对性能造成巨大的影响. 而且对于不同的容器, 加锁方式的不同, 性能可能也不同(例如hash表的锁表和锁桶). 因此``STL``默认不是线程安全. 如果需要在多线程环境下使用, 往往需要调用者自行保证线程安全。

## 2 智能指针是否是线程安全

&emsp;&emsp;对于``unique_ptr``, 由于只是在当前代码块范围内生效, 因此不涉及线程安全问题. 

&emsp;&emsp;对于``shared_ptr``, 多个对象需要共用一个引用计数变量, 所以会存在线程安全问题. 但是标准库实现的时候考虑到了这 个问题, 基于原子操作(``CAS``)的方式保证``shared_ptr``能够高效, 原子的操作引用计数.

# 四、其他的相关的锁

- 悲观锁：在每次取数据时，总是担心数据会被其他线程修改，所以会在取数据前先加锁（读锁，写锁，行 锁等），当其他线程想要访问数据时，被阻塞挂起，我们现在写的锁都是悲观锁。
- 乐观锁：每次取数据时候，总是乐观的认为数据不会被其他线程修改，因此不上锁。但是在更新数据前， 会判断其他数据在更新前有没有对数据进行修改。主要采用两种方式：版本号机制和CAS操作。MySQL中有乐观锁的使用。
- 自旋锁：

# 五、读者写者问题

## 1 读者写者模型简介

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524191210.png" style="zoom:80%;" />

## 2 读者写者模型的基本操作

phread库读写锁接口：

初始化和销毁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524191557.png" style="zoom:80%;" />

以读者身份加锁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524191636.png" style="zoom:80%;" />

以写方式加锁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524191716.png" style="zoom:80%;" />

释放锁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524191905.png" style="zoom:80%;" />

&emsp;&emsp;读者写者在处理数据时，如果确定当前是读者，就用读方式加锁；如果确定当前是写者，就用写方式加锁。

## 3 如何理解读者写者模型

&emsp;&emsp;我们通过一段伪代码理解一下读写锁的原理：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524193859.png" style="zoom:80%;" />

## 4 优先级问题

&emsp;&emsp;从上面的示意伪代码中，并不能看出读者优先还是写者优先，是竞争锁的关系，但是实际设计代码时，我们经常采用读者优先或写者优先的策略：

- 读者优先策略：读者和写者同时到来时，让读者先进入访问。
- 写者优先策略：当读者和写者同时到来时，写者不一定能先跑，因为当前临界区中可能还有许多读者正在读，此时写者你进不去，所以它的策略是比当前写者晚来的所有读者都不能进入临界区访问，等到临界区中没有读者时，让写者先写入。

&emsp;&emsp;上限的接口通常默认读者优先策略。

&emsp;&emsp;但是我们读者写者的锁不是默认读者多写者少吗，如果读者一直来，写者不就是一直不能写入吗，那不就会出现写饥饿问题了吗。

&emsp;&emsp;但是读者写者模型不就是为了处理读者多的情况嘛，为了保证读者不读到不同的数据；而且这里的写饥饿问题也在宏观角度其实并不会用时多久，可能就几秒，其实问题不大。

# 五、挂起等待锁和自旋锁

## 1 概念

&emsp;	挂起是有成本的，挂起需要时间，唤醒也需要时间，如果等待锁时需要的时间很短，那么其实不值得挂起；如果等待锁预期需要的时间很长，那么确实应该挂起。

&emsp;&emsp;如果不挂起，而是一直去查询锁的状态(类似轮询)，这种状态被称为自旋；而普通的去挂起等待就是挂起等待锁。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524201313.png" style="zoom:80%;" />

&emsp;&emsp;自旋锁的本质：不断地通过循环检测锁的状态。

&emsp;&emsp;那么线程如何得知自己会在临界资源中呆多少时间呢？线程肯定不知道啊，是写程序的人知道。所以选自旋锁还是挂起等待，显然是程序员的决定，我们前面的抢票逻辑就很适合自旋锁。

## 2 自旋锁的接口

初始化与销毁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524201623.png" style="zoom:80%;" />

加锁与解锁：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524201706.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220524201731.png" style="zoom:80%;" />

&emsp;&emsp;使用时与挂起等待锁是一样的，不必我么自己手打while循环查询锁的状态。





