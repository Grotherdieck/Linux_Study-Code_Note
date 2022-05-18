#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <mutex>

namespace ns_ring_queue
{
    const int g_cap_default = 50;
    template <class T>
    class RingQueue
    {
    private:
        std::vector<T> ring_queue_;
        int cap_; // 固定长度
        // 生产者关心空位置资源
        sem_t blank_sem_;
        // 消费者关心数据资源
        sem_t data_sem_;
        // 生产和消费的位置
        int c_step_;
        int p_step_;
        pthread_mutex_t c_mtx_;
        pthread_mutex_t p_mtx_;
    public:
        RingQueue(int cap = g_cap_default) : cap_(cap), ring_queue_(cap), c_step_(0), p_step_(0)
        {
            sem_init(&blank_sem_, 0, cap);
            sem_init(&data_sem_, 0, 0);
            pthread_mutex_init(&c_mtx_, nullptr);
            pthread_mutex_init(&p_mtx_, nullptr);
        }
        ~RingQueue()
        {
            // 销毁信号量
            sem_destroy(&blank_sem_);
            sem_destroy(&data_sem_);
            pthread_mutex_destroy(&c_mtx_);
            pthread_mutex_destroy(&p_mtx_);
        }

    public:
        // 当前高优先级实现单生产者消费者模型
        // 生产接口 
        void push(const T &in)
        {
            // 把上锁放到申请信号量前面的话 只有一个线程能来申请信号量 其实和单生产者单消费者没啥区别
            // 申请blank_sem
            sem_wait(&blank_sem_); // P(空位置)
            pthread_mutex_lock(&p_mtx_);// 有资格竞争锁的前提是先拿信号量 这样就可以预先分配信号量 效率更高
            // 可以生产了 但是往啥位置生产呢？
            ring_queue_[p_step_] = in;
            // 数据增加了一个 V(数据)
            sem_post(&data_sem_);

            // 更新位置
            p_step_ = (p_step_ + 1) % cap_;
            pthread_mutex_unlock(&p_mtx_);
        }
        // 消费接口
        void pop(T *out)
        {
            // P(data)
            sem_wait(&data_sem_);
            pthread_mutex_lock(&c_mtx_);
            *out = ring_queue_[c_step_];
            sem_post(&blank_sem_);// V(空格子)
            // 更新位置
            c_step_ = (c_step_ + 1) % cap_;
            pthread_mutex_unlock(&c_mtx_);
        }
    };
}