#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <mutex>
using namespace std;

mutex mtx;

void* thread1(void* args)
{
    int x0 = 1;
    int xi = 11 * x0 % 16;
    mtx.lock();
    printf("公式1:");
    printf("%d ", x0);
    printf("%d ", xi);
    while (xi != x0)
    {
        xi = 11 * xi % 16;
        printf("%d ", xi);
    }
    cout << endl;
    mtx.unlock();
}

void* thread2(void* args)
{
    int x0 = 2;
    int xi = 11 * x0 % 16;
    mtx.lock();
    printf("公式2:");
    printf("%d ", x0);
    printf("%d ", xi);
    while (xi != x0)
    {
        xi = 11 * xi % 16;
        printf("%d ", xi);
    }
    cout << endl;
    mtx.unlock();
}

void* thread3(void* args)
{
    int x0 = 1;
    int xi = 2 * x0 % 13;
    mtx.lock();
    printf("公式3:");
    printf("%d ", x0);
    printf("%d ", xi);
    while (xi != x0)
    {
        xi = 2 * xi % 13;
        printf("%d ", xi);
    }
    cout << endl;
    mtx.unlock();
}

void* thread4(void* args)
{
    int x0 = 1;
    int xi = 3 * x0 % 13;
    mtx.lock();
    printf("公式4:");
    printf("%d ", x0);
    printf("%d ", xi);
    while (xi != x0)
    {
        xi = 3 * xi % 13;
        printf("%d ", xi);
    }
    cout << endl;
    mtx.unlock();
}

typedef void*(*pf)(void*);


int main()
{
    pthread_t tid[4];
    pf Pf[4] = {thread1, thread2, thread3, thread4};
    for (int i = 0; i < 4; ++i)
    {
        pthread_create(&tid[i], nullptr, Pf[i], nullptr);
    }
    for (int i = 0; i < 4; ++i) pthread_join(tid[i], nullptr);
    return 0;
}