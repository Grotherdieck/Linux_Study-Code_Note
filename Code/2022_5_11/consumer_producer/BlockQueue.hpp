#pragma once
#include <iostream>
#include <queue>
using namespace std;

const int defaultcap = 5;

namespace ns_blockqueue
{
    template <class T>
    class BlockQueue
    {
    public:
        BlockQueue(int cap = defaultcap) :cap_(cap)
        {
            // 初始化锁和两个条件变量
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&is_full_, nullptr);
            pthread_cond_init(&is_empty_, nullptr);
        }
        ~BlockQueue() 
        {
            // 销毁锁和条件变量
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&is_full_);
            pthread_cond_destroy(&is_empty_);
        }
        // 队列是否满
        bool isFull()
        {
            return bq_.size() == cap_;
        }
        // 队列是否空
        bool isEmpty()
        {
            return bq_.size() == 0;
        }

        // const T& 输入型参数
        // T&输入输出型参数
        // T*输出型参数
        void push(const T& in)
        {
            // 先队列中放数据 生产函数
            // 还有bug?

            // 先把队列锁上
            LockQueue();
            // 如果满了就去等待
            if (isFull()) 
            {
                ProducterWait();
                // pthead_cond_wait返回时自动再次竞争锁 竞争到了才回来
                // 所以此时是有锁状态
            }
            bq_.push(in);

            // 生产者生产了 自然可以通知消费者可以消费了
            // 策略:如果队列中的元素大于2个 唤醒消费者
            //if (bq_.size() > 2) WakeupConsumer();
            UnlockQueue();
            WakeupConsumer();
        }

        void pop(T* out)
        {  
            LockQueue();
            // 从队列中拿数据 消费函数
            // 如果队列为空 消费者去等待
            if (isEmpty())
            {
                ConsumerWait();
            }
            // 拿走数据
            *out = bq_.front();
            bq_.pop();

            // 通知反正肯定不满了 生产者可以来生产了
            // WakeupProducer();

            // 策略 个数小于30时就去唤醒生产者。
            //if (bq_.size() <= 30) WakeupProducer();
            UnlockQueue();
            WakeupConsumer();
        }
    private:
        // 命名习惯 带_是成员
        queue<T> bq_;
        pthread_mutex_t mtx_;
        // 当满了的时候 就不要让生产者生产了 让消费者来消费
        // 当消费空了 就不要让生产者生产了 让生产者来生产
        int cap_;// 队列元素上限
        pthread_cond_t is_full_;// bq_是满的 消费者在该条件变量下等待
        pthread_cond_t is_empty_;// bq_是空的 生产者在该条件变量下等待
        void LockQueue()
        {
            pthread_mutex_lock(&mtx_);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&mtx_);
        }

        void ProducterWait()
        {
            // 带把锁的原因是防止自己去等的时候仍然上着锁不让访问临界资源
            // 1.调用这个函数时，会首先释放锁mtx_, 然后再挂起自己
            // 2.当它返回的时候，会首先自动竞争锁，获取到锁之后才能返回
            pthread_cond_wait(&is_empty_, &mtx_);
        }
        void ConsumerWait()
        {
            pthread_cond_wait(&is_full_, &mtx_);
        }
        void WakeupConsumer()
        {
            // 唤醒消费者 它在is_full_等
            pthread_cond_signal(&is_full_);
        }
        void WakeupProducer()
        {
            // 唤醒生产者 它在is_empty_等
            pthread_cond_signal(&is_empty_);
        }
    };
}