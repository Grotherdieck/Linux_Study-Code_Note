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
    // 0.˵���ֲ�
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    // ��ȡ������ip�Ͷ˿ں�
    string svr_ip(argv[1]);
    uint16_t svr_port = atoi(argv[2]);
    // 1.�����׽���
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "socket create error, error code : " << errno << endl;
        return 2;
    }
    // 2.���ӷ�����
    sockaddr_in svr;
    memset(&svr, 0, sizeof(svr));
    // ����Ҳ������bzero
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
    // 3.���ӳɹ� ����������ҵ������
    cout << "connect success!" << endl;
    while (1)
    {
        // ����write��read�����������Ķ�д
        cout << "client plz enter #: ";
        char buffer[1024] = {0};
        // �ӿͻ�������
        fgets(buffer, sizeof(buffer) - 1, stdin);
        // д�������
        write(sock, buffer, strlen(buffer));
        // ������˸�������
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
