#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

// use it as ./tcp_client sever_ip sever_port

void Usage(string op)
{
    cout << "Usage : use it as \n\t" << op << " sever_ip sever_port";
}


int main(int argc, char* argv[])
{
    // 0.说明手册
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    // 获取服务器ip和端口号
    string svr_ip(argv[1]);
    uint16_t svr_port = atoi(argv[2]);
    // 1.创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "socket create error, error code : " << errno << endl;
        return 2;
    }
    // 2.链接服务器
    sockaddr_in svr;
    memset(&svr, 0, sizeof(svr));
    // 这里也可以用bzero
    // bzero(&svr, sizeof(svr));
    svr.sin_family = AF_INET;
    svr.sin_port = htons(svr_port);
    svr.sin_addr.s_addr = inet_addr(svr_ip.c_str());
    socklen_t len = sizeof(svr);
    if (connect(sock, (struct sockaddr*)&svr, len) < 0)
    {
        cerr << "connect fail , error code : " << errno << endl;
        return 3;
    }
    // 3.链接成功 进行正常的业务请求
    cout << "connect success!" << endl;
    while (1)
    {
        // 利用write和read进行数据流的读写
        cout << "client plz enter #: ";
        char buffer[1024] = {0};
        // 从客户端输入
        fgets(buffer, sizeof(buffer) - 1, stdin);
        // 写给服务端
        write(sock, buffer, strlen(buffer));
        // 读服务端给的数据
        ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "sever echo :# " << buffer << endl;
        }
        else if (s == 0)
        {
            cout << "sever close..." << endl;
            break;
        }
        else 
        {
            cerr << "read error, error code : " << errno << endl;
        }
    }

    return 0;
}
