#include "Protocol.hpp"
#include "Sock.hpp"
#include <unordered_map>

// ./CalSever port

void Usage(string proc)
{
    cout << "Usage : "
         << "\n\t" << proc << "port" << endl;
}

void work(int sock, sockaddr_in peer) throw(string)
{
    // version1 �����л��ͷ����л�
    // ��һ���̷��� ����һ�ξͶϿ�
    // (��ȡ�ַ���)->��������Ϊrequest->����response->(���л�)->close(sock)
    // 1. ��ȡ����
    request_t req;
    cout << "get a new link..." << endl;
    ssize_t s = read(sock, &req, sizeof(req));
    if (s == sizeof(req))
    {
        // 2.��������
        unordered_map<char, function<int(int, int)>> dst;
        dst['+'] = [](int x, int y)
        { return x + y; };
        dst['-'] = [](int x, int y)
        { return x - y; };
        dst['*'] = [](int x, int y)
        { return x * y; };
        dst['/'] = [](int x, int y)
        {
            if (y == 0)
            {
                throw "div by zero";
                return 0;
            }
            return x / y;
        };
        dst['%'] = [](int x, int y)
        {
            if (y == 0)
            {
                throw "div by zero";
                return 0;
            }
            return x % y;
        };
        // 3. ������ && ����response
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
        // 5. send
        write(sock, &res, sizeof(res));
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