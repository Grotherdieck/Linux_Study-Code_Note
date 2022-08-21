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

using namespace std;

void ServiceIO(int new_sock, struct sockaddr_in peer)
{
    while (1)
    {
        char buffer[1024] = {0};
        // ����read��write�ṩ����.
        ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "client#: " << buffer;
            if (buffer[s - 1] != '\n') cout << endl;
            if (buffer[s - 1] == '\n') buffer[s - 1] = 0;
            cout << "message from ip : " << inet_ntoa(peer.sin_addr)
                 << " port : " << ntohs(peer.sin_port) << endl;
            cout << endl;
            string ret(buffer);
            ret += "\nsever return to you (what you said last time).";
            write(new_sock, ret.c_str(), ret.size());
        }
        else if (s == 0)
        {
            // ����ֵΪ0��ʾ�Զ˰����ӹ���
            cout << "client " << "[" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) 
            << "] quit..." << endl;
            // �ر��������̰߳汾�͹ر��ļ������������˳�
            close(new_sock);
            break;
        }
        else
        {
            cerr << "read error, error code : " << errno << endl;
            break;
        }
    }
}

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
    // ���ӽ����˳���SIGCHLD�źź��� ��OS�����ǻ����ӽ�����Դ ������ȥ����accept������û���µ�����
    // signal(SIGCHLD, SIG_IGN);
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
        // ����STLthread�ⴴ���߳�������
        thread t(ServiceIO, new_sock, peer);
        t.detach();
    }
    return 0;
}