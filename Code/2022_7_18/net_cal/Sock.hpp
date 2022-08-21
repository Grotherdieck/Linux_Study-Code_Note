#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <thread>

using namespace std;

// 封装一下套接字编程
class Sock
{
public:
    // 1.创建套接字
    static int Socket()
    {
        int res = socket(AF_INET, SOCK_STREAM, 0);
        if (res < 0)
        {
            cerr << "socket failed, error code : " << errno << endl;
            exit(1);
        }
        return res;
    }
    // 2. 绑定
    static void Bind(int sock, uint16_t port)
    {
        sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            cerr << "bind fail, error code : " << errno << endl;
            exit(2);
        }
    }
    // 3. listen
    static void Listen(int sock)
    {
        if (listen(sock, 5) < 0)
        {
            cerr << "listen socket fail, error code : " << errno << endl;
            exit(3);
        }
    }
    // 4.accept
    static int Accept(int sock, sockaddr_in& peer)
    {
        socklen_t len = sizeof(peer);
        int fd = accept(sock, (struct sockaddr*)&peer, &len);
        if (fd < 0)
        {
            cerr << "accept fail" << endl;
            return -1;
        }
        return fd;
    }
    //5. connect
    static void Connnect(int sock, string ip, uint16_t port)
    {
        sockaddr_in svr;
        memset(&svr, 0, sizeof(svr));
        svr.sin_family = AF_INET;
        svr.sin_port = htons(port);
        svr.sin_addr.s_addr = inet_addr(ip.c_str());
        if (connect(sock, (struct sockaddr*)&svr, sizeof(svr)) < 0)
        {
            cerr << "connect fail" << endl;
            exit(4);
        }
    }
};