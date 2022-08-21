#include "Protocol.hpp"
#include "Sock.hpp"
#include <unordered_map>

// ./CalSever port

void Usage(string proc)
{
    cout << "Usage : "
         << "\n\t" << proc << " port" << endl;
}

unordered_map<char, function<int(int, int)>> dst = {
        {'+', [](int x, int y)->int { return x + y; }}, 
        {'-', [](int x, int y)->int { return x - y; }}, 
        {'*', [](int x, int y)->int { return x * y; }},
        {'/', [](int x, int y)->int
        {
            if (y == 0)
            {
                throw "div by zero";
                return 0;
            }
            return x / y;
        }},
        {'%', [](int x, int y)->int
        {
            if (y == 0)
            {
                throw "div by zero";
                return 0;
            }
            return x % y;
        }
        }
};
void work(int sock, sockaddr_in peer) throw(const char*)
{
    // version1 无序列化和反序列化
    // 做一个短服务 计算一次就断开
    // (读取字符串)->分析处理为request->制作response->(序列化)->close(sock)
    // 1. 读取请求
    // request_t req;
    char buffer[1024] = {0};
    cout << "get a new link..." << endl;
    ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
    if (s > 0)
    {
        // 反序列化
        buffer[s] = 0;
        
        string json_str(buffer);
        cout << "Get a new request, jsonstring = " << json_str;
        request_t req;
        DeserializationReq(json_str, req);
        // 2.分析请求
        
        // 3. 计算结果 && 构建response
        int ans;
        response_t res;
        res.code = 0;
        try
        {
            if (dst.count(req._op) != 0)
                ans = dst[req._op](req._x, req._y);
            else
                res.code = -2;
        }
        catch (...)
        {
            cout << "catch a exception : div by zero..." << endl;
            res.code = -1;
        }
        res.result = ans;
        // 序列化返回结果
        string to_send = SerializeResponse(res);
        // 5. send
        write(sock, to_send.c_str(), to_send.size());
    }
    // 6. close
    close(sock);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return -1;
    }
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, atoi(argv[1]));
    Sock::Listen(listen_sock);

    while (1)
    {
        struct sockaddr_in peer;
        int new_sock = Sock::Accept(listen_sock, peer);
        if (new_sock >= 0)
        {
            thread t(work, new_sock, peer);
            t.detach();
        }
    }

    return 0;
}