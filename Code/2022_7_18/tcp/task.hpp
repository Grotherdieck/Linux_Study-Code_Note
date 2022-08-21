#pragma once
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
using namespace std;

namespace Ns_task
{
    // 新的任务里头只有一个套接字
    class Task
    {
    private:
        int _sock;
        sockaddr_in _peer;

    public:
        Task() : _sock(-1) {}
        Task(int sock, sockaddr_in peer) : _sock(sock), _peer(peer) {}
        ~Task() {}
        void Run()
        {
            while (1)
            {
                char buffer[1024] = {0};
                // 利用read和write提供服务.
                ssize_t s = read(_sock, buffer, sizeof(buffer) - 1);
                if (s > 0)
                {
                    buffer[s] = 0;
                    cout << "client#: " << buffer;
                    if (buffer[s - 1] != '\n')
                        cout << endl;
                    if (buffer[s - 1] == '\n')
                        buffer[s - 1] = 0;
                    cout << "message from ip : " << inet_ntoa(_peer.sin_addr)
                         << " port : " << ntohs(_peer.sin_port) << endl;
                    cout << endl;
                    string ret(buffer);
                    ret += "\nsever return to you (what you said last time).";
                    write(_sock, ret.c_str(), ret.size());
                }
                else if (s == 0)
                {
                    // 返回值为0表示对端把链接关了
                    cout << "client "
                         << "[" << inet_ntoa(_peer.sin_addr) << ' ' << ntohs(_peer.sin_port)
                         << "] quit..." << endl;
                    // 关闭了链接线程版本就关闭文件描述符并且退出
                    close(_sock);
                    break;
                }
                else
                {
                    cerr << "read error, error code : " << errno << endl;
                    close(_sock);
                    break;
                }
            }
        }
        void operator()()
        {
            Run();
        }
    };
}