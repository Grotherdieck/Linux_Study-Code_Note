# һ��Э������л��뷴���л�

&emsp;&emsp;����д��һ��һ�������������������Ӧ�ò��ϡ�

&emsp;&emsp;��������ǰ��д�ĳ���������ֱ�Ӱ��ַ�����ʽ���ͺͽ��ܵģ���������Լ�Ҫ��һЩ�ṹ��������ô���أ��ǲ��Ǿ���Ҫ������һЩԼ���أ����������Ӧ�ò��Э�顣

&emsp;&emsp;ʵ���������ڴ���Ϣʱ����Ϣ��ͷ������һ�����ӵĽṹ�壬�����ڴ�ʱ���ǰ���ת��Ϊһ��һ�����ݰ���������Ϊ��һ�����ַ�����ȥ���ģ��Է��ӵ����ǵİ����ٽ���Ϊ�����Ľṹ�����ݣ����ǵĽṹ�����ݱ�����ݰ��Ĺ��̳�Ϊ���л����̣����շ������ݰ�����Ϊ�ṹ�����ݵĹ��̳�Ϊ�����л��Ĺ��̡�

&emsp;&emsp;Ϊʲôһ��Ҫͨ�����л��ͷ����л�������ṹ�أ�

* Ϊ��Ӧ�ò�����ͨ�ŵķ��㣬�ַ����ȽϷ��㴫�䣻
* Ϊ�˷����ϲ㷽��ʹ�ñ����Ľṹ��Ա��ʹ��Ӧ�ú���������˽��

&emsp;&emsp;����֮ǰ��tcp/udp��������û�����л��ͷ����л�������̵ģ���Ϊ����û��Ӧ�ó�����

&emsp;&emsp;����ṹ�������ݵ����л��ͷ����л�����Э������֡�

&emsp;&emsp;��ô��ô�����л��ͷ����л���������أ������Ӿ����Լ�д�������Ӿ����Լ�ȥ�ñ���д�õ��������xml��json��protobuff����

# ��������汾������

&emsp;&emsp;Ϊ���������л��ͷ����л��Ĺ��̣�������дһ������汾�ļ����������ǿ���һ��ͨ�ŷ�����

Լ������һ:

* �ͻ��˷���һ������"1+1"���ַ���;
* ����ַ�����������������, ��������;
* ��������֮�����һ���ַ��������, �����ֻ���� +;
* ���ֺ������֮��û�пո�;
* ...

Լ��������:

* ����ṹ������ʾ������Ҫ��������Ϣ;
* ��������ʱ������ṹ�尴��һ������ת�����ַ���, ���յ����ݵ�ʱ���ٰ�����ͬ�Ĺ�����ַ���ת
  ���ؽṹ��;
* ������̾��� "���л�" �� "�����л�"

## 1 ����Э��

&emsp;&emsp;������ʵ��һ�������л��ͷ����л��ĳ���˼·���ȶ���������õĽṹ�壨request_t response_t����Ȼ���װһ����TCP���������Ǽ����ӿڣ�ʵ�����£�

```cpp
// Protocol.hpp
#pragma once
#include <iostream>
#include <string>

using namespace std;

// ����Э��Ĺ��� ���Ƕ��ƽṹ�����ݵĹ���

// �����ʽ
typedef struct request
{
    int _x;
    int _y;
    int _op;// +-/*%
}request_t;

// ��Ӧ��ʽ
typedef struct response
{
    int code;// sever������ϵļ���״̬ code(0):success, code(-1):div 0 
    int result;// ������
}response_t;

// Sock.hpp
#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <thread>

using namespace std;

// ��װһ���׽��ֱ��
class Sock
{
public:
    // 1.�����׽���
    static int Socket()
    {
        int res = socket(AF_INET, SOCK_STREAM, 0);
        if (res < 0)
        {
            cerr << "socket failed, error code : " << errno << endl;
            exit(1);
        }
        return res;
    }
    // 2. ��
    static void Bind(int sock, uint16_t port)
    {
        sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            cerr << "bind fail, error code : " << errno << endl;
            exit(2);
        }
    }
    // 3. listen
    static void Listen(int sock)
    {
        if (listen(sock, 5) < 0)
        {
            cerr << "listen socket fail, error code : " << errno << endl;
            exit(3);
        }
    }
    // 4.accept
    static int Accept(int sock, sockaddr_in& peer)
    {
        socklen_t len = sizeof(peer);
        int fd = accept(sock, (struct sockaddr*)&peer, &len);
        if (fd < 0)
        {
            cerr << "accept fail" << endl;
            return -1;
        }
        return fd;
    }
    //5. connect
    static void Connnect(int sock, string ip, uint16_t port)
    {
        sockaddr_in svr;
        memset(&svr, 0, sizeof(svr));
        svr.sin_family = AF_INET;
        svr.sin_port = htons(port);
        svr.sin_addr.s_addr = inet_addr(ip.c_str());
        if (connect(sock, (struct sockaddr*)&svr, sizeof(svr)) < 0)
        {
            cerr << "connect fail" << endl;
            exit(4);
        }
    }
};

// CalSever.cc
// ÿ�ζ���һ���߳�ȥ����������һ��
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

// CalClient.cc
// ����һ���̷��񣬷���һ������ �õ�������ӡ���ֱ�ӶϿ�
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
```

&emsp;&emsp;�������ô֪�������ֽṹ���أ��ͻ�����ô֪��codeʲô��˼�أ�...������Ϊ����Լ���˰����������Լ������һ���ܼ򵥵�Э�飬�ͻ��˺ͷ���˶��������ز����������С�

&emsp;&emsp;����ֱ�Ӵ��ṹ����������������������Ϊ�ͻ��˺ͷ���˵�C++�汾��һ���ģ��ڴ���뷽ʽ����һ���ģ�int����4�ֽڶ����������汾��ͬ��������Ƿ�����һЩ���ڴ����ı仯��ֱ�Ӿͻ�����⣬����ֱ�ӷ��ṹ�忴�������У�ʵ���϶���ĳЩ���˳������ǲ��еġ�

## 2 ʹ��JsonCpp�������л��ͷ����л�

&emsp;&emsp;ʹ��һ�����װJsonCpp��

```shell
sudo yum install -y jsoncpp-devel
```

&emsp;&emsp;��һ������ʵ���ϸ���������������ͷ�ļ���һЩ����̬�⡣

&emsp;&emsp;���ǲ���һ�£�

&emsp;&emsp;�������ʱҪ�� ``-ljsoncpp``����ʾ����json�⣬���÷���ע�ͣ�

```cpp
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// �����ʽ �������л��������
typedef struct request
{
    int _x;
    int _y;
    int _op;// +-/*%
}request_t;

int main()
{
    request_t req = {10, 20, '+'};
    Json::Value root;
    // Json::Value��һ��KVʽ���ܹ����ض���Ķ���
    // �ȰѴ����л������ݳ��ص�root��
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer��һ������� ���������� FastWriter��StyledWriter
    Json::StyledWriter writer;
    // ����write����ȥ�����Value���л�
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;
    return 0;
}

```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145207.png)

&emsp;&emsp;���н�����£�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145353.png)

&emsp;&emsp;���� ``FastWriter``���ԣ�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145549.png)

&emsp;&emsp;�����л���

```cpp
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// �����ʽ �������л��������
typedef struct request
{
    int _x;
    int _y;
    char _op;// +-/*%
}request_t;

int main()
{
    request_t req = {10, 20, '+'};
    Json::Value root;
    // Json::Value��һ��KVʽ���ܹ����ض���Ķ���
    // �ȰѴ����л������ݳ��ص�root��
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer��һ������� ���������� FastWriter��StyledWriter
    //Json::StyledWriter writer;
    Json::FastWriter writer;
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;

    // �����л� ��һ���ַ���ת��Ϊ�ַ���
    // R"()"��ʾ��ͷ��һ��ԭ���ַ���,ʡȥ��ת��
    std::string JsonString = R"({"datax":10,"datay":20,"operator":43})";
    Json::Value Root;
    Json::Reader reader;
    reader.parse(jsonstring, Root);// ��������л����ݳ��Root
    // Ȼ����KV����mapһ��ȥ�á����֡���������
    request_t ans;
    // ��������ȥ��
    ans._x = Root["datax"].asInt();
    ans._y = Root["datay"].asInt();
    //ans._op = Root["operator"].asInt();
    // �����char
    ans._op = (char)Root["operator"].asUInt();
    cout << "ans._x = " << ans._x << ", ans._y = " << ans._y << ", ans._op = " << ans._op << endl;
    return 0;
}

```

&emsp;&emsp;��������������ֻ�����л�����ַ���jsonstring��ô�շ����ɣ�����ҵ���߼�ʱ�ٰ��ַ������з����л�����

## 3 �����ǵ�Cal�������л��뷴���л�

&emsp;&emsp;���ȷ�װһ�����л��뷴���л�:

```cpp
// ���л�����
std::string SerializeRequest(const request_t& req)
{
    Json::Value root;
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    Json::FastWriter writer;
    return writer.write(root);
}

// ��Ӧ���л�
std::string SerializeResponse(const response_t& res)
{
    Json::Value root;
    root["code"] = res.code;
    root["result"] = res.result;
    return Json::FastWriter().write(root);
}

// �����л�����
void Deserialization(const string& jsonstring, request_t& req)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring, root);
    req._x = root["datax"].asInt();
    req._y = root["datay"].asInt();
    req._op = (char)root["operator"].asUInt();
}

// �����л���Ӧ
void Deserialization(const string& jsonstring, response_t& res)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(jsonstring, root);
    res.code = root["code"].asInt();
    res.result = root["result"].asInt();
}
```

&emsp;&emsp;���������Ǹ����ǵ�sever��client�������л��뷴���л���

```cpp
// CalSever.cc
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
void work(int sock, sockaddr_in peer) throw(string)
{
    // version1 �����л��ͷ����л�
    // ��һ���̷��� ����һ�ξͶϿ�
    // (��ȡ�ַ���)->��������Ϊrequest->����response->(���л�)->close(sock)
    // 1. ��ȡ����
    // request_t req;
    char buffer[1024] = {0};
    cout << "get a new link..." << endl;
    ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
    if (s > 0)
    {
        // �����л�
        buffer[s] = 0;
        string json_str(buffer);
        request_t req;
        DeserializationReq(json_str, req);
        // 2.��������
    
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
        // ���л����ؽ��
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
// CalClient.cc
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
    // ���л�����
    string to_send = SerializeRequest(req);
    // III send��ȥ
    write(sock, to_send.c_str(), to_send.size());
    // IV �ȴ��������ӡ
    cout << "request has been send to sever..." << endl;
    // response_t res;
    char buffer[1024] = {0};
    ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
    if (s > 0)
    {
        buffer[s] = 0;
        // �����л��õ����
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
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719163113.png)

&emsp;&emsp;��������д��������߼����������ʾ���һ��Ӧ�ò�����������У�ͨ�Ŵ����������Լ���ɵģ����л��ͷ����л�����ͨ�����������jsoncpp����ˣ�ҵ���߼��������Լ����ģ����󡢽����ʽ��code�����������Լ�Լ���ģ����������������һ��**Ӧ�ò��������**

&emsp;&emsp;��Ӧ��OSI�߲�ģ���У��Ự���������д��ͨ�Ŵ��룬��ʾ��������ǵ����л��뷴���л���Ӧ�ò��������д�ļ�����ҵ���߼���������д��ʱ������������������һ��ģ����Ǹ���û�������ǳ��׷ֿ�������TCP/IP�а������㹫��ΪӦ�ò㡣

# ����HTTPЭ��

&emsp;&emsp;��Ȼ˵Ӧ�ò�Э���������Լ����ģ�������ʵ��;ͺܶ����Ա���ж����˺ܶ��ֳɵģ��ǳ����õ�Ӧ�ò�Э�������ֱ��ʹ�ã�HTTP�����ı�����Э�飩Э���������֮һ��

&emsp;&emsp;HTTPЭ�鱾���ϣ������Ǹ�д�����������û�����𣬶���Ӧ�ò��Э�飬����HTTP�ڲ�Ҫʵ������д��**����ͨ�š����л��뷴���л���Ӧ�ò�Э��ϸ�ڣ����а�����д���ˣ����ǿ���ֱ����HTTPЭ�顣**

## 1 ��ʶURL(��ַ)

&emsp;&emsp;��ַ�Ƕ�λ������Դ��һ�ַ�ʽ�����������ͼƬ��html��css��js����Ƶ����Ƶ����ǩ���ĵ���Щ����Ϊ��Դ��

&emsp;&emsp;����ͨ��IP+port����Ψһ�Ķ�λһ�����̣�����IP��Ψһ��ȥȷ��һ�������ģ�����ν��������Դ���������ļ��ķ�ʽ����һ�����������ϵġ���Linux��ȷ��һ���ļ��ķ�����ͨ������·������ʶ�ģ�����IP + ·���Ϳ���Ψһ��ȷ��һ��������Դ��

&emsp;&emsp;��IPͨ�����������ķ�ʽ���ֵģ�·��ͨ��Ŀ¼��+�ָ���/ʵ�֡�

```
http://bs.scu.edu.cn/quanyuanjiaoshi.html
```

&emsp;&emsp;ǰ���http://�����������Դ�ķ�����bs.scu.edu.cn��������������/quanyuanjiaoshi.html��ʾ�����������Դ��ע�⣬�����Ŀ¼���ܲ���Linux��ͷ���Ǹ���Ŀ¼������������·��Ŀ¼��

&emsp;&emsp;��Linux�У�����ͨ�����

```shell
ping ���� 
```

&emsp;&emsp;����������Ӧ�Ķ˿ںš�

&emsp;&emsp;�������ﶼ���Ӷ˿ں�����Ϊ�˿ںź����ǿ��ķ���������ǿ��صģ����Ǵ���Ѿ�Լ���׳ƵĶ���������http��80��https��443��mysql��3306�ȡ�URL�л����Դ��Σ�����������ҵ���Դ��

&emsp;&emsp;����һ��URL�����þ���ȷ�������е�һ����Դ��

## 2 urlencode��urldecode

&emsp;&emsp;�� / ? : ���������ַ�, �Ѿ���url�����������������. �����Щ�ַ������������.

&emsp;&emsp;����, ĳ����������Ҫ������Щ�����ַ�, �ͱ����ȶ������ַ�����ת��.

&emsp;&emsp;ת��Ĺ�������:

&emsp;&emsp;����Ҫת����ַ�תΪ16���ƣ�Ȼ����ҵ���ȡ4λ(����4λֱ�Ӵ���)��ÿ2λ��һλ��ǰ�����%�������%XY��ʽ

&emsp;&emsp;��C++�ͻᱻ����ΪC%2B%2B��

&emsp;&emsp;��Щ���߶���ֱ����������������

## 3 ��ʼHTTP

&emsp;&emsp;��������������Ӧ��HTTP���ǰ���β��λ�����������Ӧ�ġ���������������Ӧ������������3��4������ɡ�

&emsp;&emsp;�����������£�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721195613.png)

&emsp;&emsp;˼������ȻHTTP������Ϊ��λ�ģ���ô��ν��/��װ����η����أ�

&emsp;&emsp;�����м�����ͷ����HTTP�ġ���ͷ�����������ľ���HTTP����Ч�غɣ�״̬�м���Ӧ��ͷ��HTTP�ġ���ͷ������Ӧ���ļ�HTTP����Ч�غɡ�

&emsp;&emsp;��ô����ʱ��HTTP request��HTTP response��ο����أ�

&emsp;&emsp;�������������ǿ���ֱ�Ӱ���������������ַ������У��������Ķ�ȡ�ͷ��Ͷ��ǰ��ַ������͵ģ�

&emsp;&emsp;������ν�����ÿ��аѳ��ַ���һ��Ϊ2�����Ǵ�ͷ��ʼ����ֻҪ�������У�����Ϊ�ѱ�ͷ�����ˣ���װ���������ɡ�

&emsp;&emsp;������ù�����ͨ��Ӧ�ò��������ġ�

## 4 ��򵥵�HTTP lab

&emsp;&emsp;ǰ�潲����ô�����ۣ��Ҿ��뿴����Щ���۾��嵽��������ʲô���ģ�������дд���뿴���ɡ�

&emsp;&emsp;HTTP�ײ�Ҳ�ǻ���TCP�ģ�������������Ҫ���׽����ǿ�����ù�����

&emsp;&emsp;ǰ������һ���������׽��֣��󶨶˿ںţ����ü������������Ӵ����̴߳������󣬿����������������ݣ��������ʱ�����ǲ���read����һ���µ�ϵͳ���á���``recv``��

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721203024.png)

&emsp;&emsp;recv������flags��������������������������������������ȡ����ָ�������Ķ�������������Ĭ����Ϊ0���ɡ�

&emsp;&emsp;�������£�

```cpp
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
        std::cout << buffer << endl;// ��http����ĸ�ʽ��ӡ����
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
```

&emsp;&emsp;��ô��ô���������أ���������Ȼ�Ĺ��ߣ������ѽ��ʹ��������ڵ�ַ������ ����IP+port������д�����sever������

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721204123.png)

&emsp;&emsp;��������һ����HTTP��ͷ�Ŀ��У���һ�������Ǵ������endl����Ϊ����û�з������ݣ�����û�����ġ�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721205900.png)

&emsp;&emsp;�ڱ���˵���ֻ�����

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721205428.png)

&emsp;&emsp;���������빹��һ����Ӧ����ȥ��

&emsp;&emsp;����������sendϵͳ���÷�����Ϣ������Linux��רΪTCP��Ƶģ���ôֱ��send��Ϣ���ܷ���ȥ���𣿿϶������԰����㻹û����HTTP��Ӧ��ʽ�ء�

&emsp;&emsp;���ǹ���һ������״̬�У���֪��Ч�غ�����ͨ�ı���Content-Type�ټ��Ͽ��У�Ȼ���һ���ı��Ļظ�����ȥ������Content-Type���ձ���������飺

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721210918.png)

```cpp
	std::string http_response = "http/1.0 200 OK\n";// ״̬��200 ״̬������OK
        // Content-Type ��֪��Ч�غ���ʲô����
        http_response += "Content-Type: text/plain\n";// test/plain��ʾ��������ͨ���ı�
        http_response += "\n";// ����
        http_response += "hello world";// ��������
        send(sock, http_response.c_str(), http_response.size(), 0);
```

&emsp;&emsp;����һ�£�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721211439.png)

&emsp;&emsp;���л�����Щ����HTTP�Ѿ�����������ˣ����Ƕ����ľ���һ�����ַ�����

&emsp;&emsp;����HTTPЭ�鱾�ʾ��ǵõ������ַ������������󹹽���Ӧ������Ӧͨ���׽��ַ��ظ��Է���

## 5 HTTP����

&emsp;&emsp;���ǵ�½�ٶȵķ��������ֶ�����һ�����󣬿�����ν��ҳ������ɶ��

&emsp;&emsp;��½��

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721212642.png)

&emsp;&emsp;�����뷵�أ�

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721212825.png)

&emsp;&emsp;���صĶ�������һ��html��
