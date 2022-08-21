#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>


using namespace std;

const int port = 999;

// udp_sever 
int main()
{
    // 1 创建套接字(打开网络文件)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket create error : " << errno << endl;
        return 1;
    }
    // cout << "sock = " << sock << endl;

    // 2 给该套接字绑定IP和端口号
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    // 因为端口号是要在网络里头发来发去的数据，所以从主机数据转网络数据htons
    local.sin_port = htons(port);
    // 下一个要填ip地址 
    // a.先把人识别的点分IP转化为4字节的整数
    // b.是网络数据 转大小端
    // 这里有API可以用捏
    // int_addr_t inet_addr(const char* cp) 自动完成上面两个工作
    // 云服务器不允许绑定公网IP 实际正常编写时，也不会指明IP
    // local.sin_addr.s_addr = inet_addr("**********");
    local.sin_addr.s_addr = INADDR_ANY;
    // INADDR_ANY:因为一个云服务器可能有多个网卡，配置多个IP，
    // 我们需要的不是某个IP上的数据，我们需要的是所有发给当前主机对应端口号的数据
    // INADDR_ANY的作用就是这样 它是最常用的

    // 这里强转是为了参数类型匹配
    if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        // 如果创建失败返回值小于0 则打印错误码并终止程序
        std::cerr << "bind fail, error code : " << errno << endl;
        return 2;
    }

    // 3. 提供服务
    bool quit = false;
    // 缓冲区
    const int NUM = 1024;
    char buffer[NUM];
    while (!quit)
    {
        // 收集客户端套接字信息的结构
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int s = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
        // 把消息打印出来
        buffer[s] = '\0';
        cout << "#client say : " << buffer << " from ip : " << inet_ntoa(peer.sin_addr) << endl;
        // cout << "#client say : " << buffer << endl;
        // string str = "hello client, You said : ";
        // str += buffer;
        // 给客户端发数据
        string str;
        // buffer[s] = 0;
        FILE* fp = popen(buffer, "r");

        char line[1024] = {0};
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            str += line;
        }

        pclose(fp);
        sendto(sock, str.c_str(), str.size(), 0, (struct sockaddr*)&peer, len);
       
    }
    return 0;
}