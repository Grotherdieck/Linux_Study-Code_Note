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
        int num_;// �̳߳��е��߳�����
        std::queue<T> task_queue_;
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
    public:
        // ��װһ������
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
        // �����У�Ҫ���߳�ִ�����ڳ�Ա�����ǲ����е�
        // ��Ϊpthread_create���﷨��void* function(void*)
        // ���Ǿ�̬���Ա������thisָ��
        // ����������Ҫ���þ�̬����
        static void* Rountine(void* args)
        {
            ThreadPool<T>* tp = (ThreadPool<T>*)args;
            pthread_detach(pthread_self());
            while (true)
            {
                // ����������������� Ϊ���������������Ķ�������Ҫ����thisָ��
                tp->Lock();
                while (tp->isEmpty()) 
                {
                    // �������Ϊ�� �̳߳ص��߳�Ӧ����ʲô�أ�Ӧ�����̹߳��� 
                    // ������������
                    tp->wait();
                }
                T t;
                tp->poptask(&t);

                tp->Unlock();
                // �����⴦������
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
        
        // ��������з�����
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
