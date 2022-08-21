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
        // 利用read和write提供服务.
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
            // 返回值为0表示对端把链接关了
            cout << "client " << "[" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) 
            << "] quit..." << endl;
            // 关闭了链接线程版本就关闭文件描述符并且退出
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
    // 1. 创建"监听"套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
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
    // 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
    // 把子进程退出的SIGCHLD信号忽略 让OS帮我们回收子进程资源 父进程去持续accept看看有没有新的链接
    // signal(SIGCHLD, SIG_IGN);
    // 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // 利用STLthread库创建线程来处理
        thread t(ServiceIO, new_sock, peer);
        t.detach();
    }
    return 0;
}