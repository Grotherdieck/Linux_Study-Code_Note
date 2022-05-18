#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;

pthread_cond_t sig1;
pthread_cond_t sig2;
pthread_mutex_t mtx;


void* threada(void* args)
{
    string ans  = (char*)args;
    while (true)
    {
        pthread_cond_wait(&sig1, &mtx);
        //pthread_cond_signal(&sig2);
        //pthread_cond_wait(&sig1, &mtx);
        cout << ans << endl;
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&sig2);
        sleep(1);
    }
}

void* threadb(void* args)
{
    string ans  = (char*)args;
    while (true)
    {
        pthread_cond_signal(&sig1);
        //pthread_mutex_lock(&mtx);
        pthread_cond_wait(&sig2, &mtx);
        cout << ans << endl;
        pthread_mutex_unlock(&mtx);
        //pthread_cond_signal(&sig1);
        sleep(1);
    }
}
int main()
{
    pthread_t a, b;
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&sig1, nullptr);
    pthread_cond_init(&sig2, nullptr);
    pthread_create(&a, nullptr, threada, (void*)"我是线程A");
    pthread_create(&b, nullptr, threadb, (void*)"我是线程B");
    pthread_join(a, nullptr);
    pthread_join(b, nullptr);
}
