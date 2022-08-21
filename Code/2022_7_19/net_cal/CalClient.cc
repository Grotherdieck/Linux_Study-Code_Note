#include "Protocol.hpp"
#include "Sock.hpp"

// Use it as ./CalClient ip port

void Usage(string proc)
{
    cout << "Usage : "
         << "\n\t" << proc << " ip port" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return -1;
    }
    // 1. 创建套接字
    int sock = Sock::Socket();
    // 2. 链接
    sockaddr_in svr;
    memset(&svr, 0, sizeof(svr));
    Sock::Connnect(sock, argv[1], atoi(argv[2]));
    cout << "connect success !" << endl;
    cout << "Please Enter # (as num1 op num2):";
    // 3. 业务逻辑
    // version1 无序列化和反序列化
    // I 读入数据
    int n1, n2;
    char op;
    cin >> n1 >> op >> n2;
    // II 封装为request
    request_t req;
    req._op = op;
    req._x = n1;
    req._y = n2;
    // 序列化请求
    string to_send = SerializeRequest(req);
    // III send过去
    write(sock, to_send.c_str(), to_send.size());
    // IV 等待结果并打印
    cout << "request has been send to sever..." << endl;
    // response_t res;
    char buffer[1024] = {0};
    ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
    if (s > 0)
    {
        buffer[s] = 0;
        // 反序列化得到结果
        string json_str(buffer);
        response_t res;
        DeserializationRes(json_str, res);
        if (res.code == 0)
        {
            cout << "cal success, ans = " << res.result << endl;
            cout << "connect again to next cal" << endl;
        }
        else if (res.code == -1)
        {
            cout << "cal failed, wrong : div by zero" << endl;
            cout << "connect again to next cal" << endl;
        }
        else if (res.code == -2)
        {
            cout << "cal failed, wrong input" << endl;
            cout << "connect again to next cal" << endl;
        }
    }
    else 
    {
        cout << "Sever goes wrong..." << endl;
    }
    return 0;
}