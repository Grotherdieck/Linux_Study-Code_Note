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
        // 使用线程池 单例模式线程池
        // 使用方法 创建任务 然后放到任务队列里即可
        Task t(new_sock, peer);
        ThreadPool<Task>::GetInstance()->pushtask(t);
    }
    return 0;
}