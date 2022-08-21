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
        int num_; // 线程池中的线程数量
        std::queue<T> task_queue_;
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
        static ThreadPool<T> *ins; //创建类内对象的指针
        

    public:
        static ThreadPool<T> *GetInstance()
        {
            static pthread_mutex_t sigmtx_ = PTHREAD_MUTEX_INITIALIZER;
            if (ins == nullptr)
            {
                pthread_mutex_lock(&sigmtx_);
                if (ins == nullptr)
                {
                    // 当前单例对象尚未被创建
                    ins = new ThreadPool<T>();
                    ins->InitThreadPool();
                    std::cout << "首次加载对象" << std::endl;
                }
                pthread_mutex_unlock(&sigmtx_);
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
        void poptask(T *out)
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
        ThreadPool(const ThreadPool<T> &tp) = delete;
        ThreadPool<T> &operator=(const ThreadPool<T> &tp) = delete;

    public:
        static void *Rountine(void *args)
        {
            ThreadPool<T> *tp = (ThreadPool<T> *)args;
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
                pthread_create(&tid, nullptr, Rountine, (void *)this /*?*/);
            }
        }

        // 向任务队列放任务
        void pushtask(const T &in)
        {
            Lock();

            task_queue_.push(in);

            Unlock();
            WakeUp();
        }

        void wait()
        {
            pthread_cond_wait(&cond_, &mtx_);
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }
    };
    // 类外初始化静态成员
    template <class T>
    ThreadPool<T> *ThreadPool<T>::ins = nullptr;
}
