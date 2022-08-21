#include "Sock.hpp"

const int SIZE = 1024 * 10;

void Usage(std::string proc)
{
    std::cout << "Usage: " << proc << " port" << std::endl;
}

void* HanderHttpRequest(void* args)
{
    int* del = static_cast<int*>(args);
    int sock = *del;
    delete del;
    char buffer[SIZE] = {0};
    ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (s > 0)
    {
        buffer[s] = 0;
        std::cout << buffer << endl;// 把http请求的格式打印出来
        std::string http_response = "http/1.0 200 OK\n";// 状态码200 状态码描述OK
        // Content-Type 告知有效载荷是什么类型
        http_response += "Content-Type: text/plain\n";// test/plain表示正文是普通的文本
        http_response += "\n";// 空行
        http_response += "hello world";// 正文内容
        send(sock, http_response.c_str(), http_response.size(), 0);
    }
    close(sock);
    return nullptr;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return -1;
    }

    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    while (1)
    {
        sockaddr_in peer;
        int sock = Sock::Accept(listen_sock, peer);
        if (sock > 0)
        {
            pthread_t tid;
            int* parm = new int(sock);
            pthread_create(&tid, nullptr, HanderHttpRequest, parm);
        }
    }
}