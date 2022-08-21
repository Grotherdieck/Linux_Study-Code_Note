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
    // 1. �����׽���
    int sock = Sock::Socket();
    // 2. ����
    sockaddr_in svr;
    memset(&svr, 0, sizeof(svr));
    Sock::Connnect(sock, argv[1], atoi(argv[2]));
    cout << "connect success !" << endl;
    cout << "Please Enter # (as num1 op num2):";
    // 3. ҵ���߼�
    // version1 �����л��ͷ����л�
    // I ��������
    int n1, n2;
    char op;
    cin >> n1 >> op >> n2;
    // II ��װΪrequest
    request_t req;
    req._op = op;
    req._x = n1;
    req._y = n2;
    // III send��ȥ
    write(sock, &req, sizeof(req));
    // IV �ȴ��������ӡ
    cout << "request has been send to sever..." << endl;
    response_t res;
    ssize_t s = read(sock, &res, sizeof(res));
    if (s == sizeof(res))
    {
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