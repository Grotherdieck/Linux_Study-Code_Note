#include <iostream>
#include <pthread.h>
#include <unistd.h>


using namespace std;

void* threadrun(void* args)
{
    while (1)
    {
        cout << "I am thread..." << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadrun, nullptr);
    sleep(3);
    pthread_exit(nullptr);
}
