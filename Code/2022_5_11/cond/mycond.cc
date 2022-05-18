#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

pthread_mutex_t mtx;
pthread_cond_t cdt;

const int N = 3;
// ctrl thread����work�߳� ������������
void *ctrl(void *args)
{
    string name = (char *)args;
    while (true)
    {
        // �����·����� �����ڸ����������µȴ���һ���߳�
        // ��ô���ѵ�����һ����?
        cout << "master say : \"begin work\"" << endl;
        // pthread_cond_signal(&cdt);
        pthread_cond_broadcast(&cdt);
        sleep(3);
    }
}

void *work(void *args)
{
    int number = *(int *)args;
    delete (int *)args;
    while (true)
    {
        // �ȴ�ctrl�̵߳ĺ���
        // �������ǵĻ���������ֻΪ��������� ��ʱ��ûʹ��
        pthread_cond_wait(&cdt, &mtx);
        cout << "worker:" << number << "is working..." << endl;
    }
}

int main()
{
    pthread_t worker[N]; // 5�������߳�
    pthread_t master;
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cdt, nullptr);
    pthread_create(&master, nullptr, ctrl, (void *)"master");
    for (int i = 0; i < N; ++i)
    {
        int *number = new int(i);
        pthread_create(&worker[i], nullptr, work, (void *)number);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_join(worker[i], nullptr);
    }
    pthread_join(master, nullptr);

    // �ͷŻ�����
    pthread_cond_destroy(&cdt);
    pthread_mutex_destroy(&mtx);
    return 0;
}