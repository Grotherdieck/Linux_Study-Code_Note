#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include "thread_pool.hpp"
#include "task.hpp"

using namespace std;
using namespace ns_threadpool;
using namespace Ns_task;

int main()
{
    // 1. ����"����"�׽���
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. �󶨱���ip��˿ں�
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }
    // 3. TCP������֮����sever�����׽���listen״̬����ʾ��ǰ���Խ�������
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
    

    // 4. ʹ��accept��õ��׽������ļ�һ������ͨ��, �ṩ����
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // ����accept ����һ�������ṩIO���׽���
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // ���û���˺��ҽ������� ��û�������� ����ѭ����
            continue;
        }
        // �ߵ�����acceptһ���ɹ��� �ṩ����
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // ʹ���̳߳� ����ģʽ�̳߳�
        // ʹ�÷��� �������� Ȼ��ŵ���������Ｔ��
        Task t(new_sock, peer);
        ThreadPool<Task>::GetInstance()->pushtask(t);
    }
    return 0;
}