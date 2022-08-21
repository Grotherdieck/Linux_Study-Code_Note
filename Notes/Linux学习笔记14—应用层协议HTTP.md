# 一、协议和序列化与反序列化

&emsp;&emsp;我们写的一个一个网络程序，它就是跑在应用层上。

&emsp;&emsp;但是我们前面写的程序，它都是直接按字符串方式发送和接受的，如果我们自己要传一些结构化数据怎么办呢，是不是就需要两边有一些约定呢？这就引出了应用层的协议。

&emsp;&emsp;实际上我们在传信息时，信息里头可能是一个复杂的结构体，我们在传时，是把它转化为一个一个数据包（可以认为四一个长字符串）去传的，对方接到我们的包，再解析为本来的结构化数据，我们的结构化数据变成数据包的过程称为序列化过程，接收方把数据包解析为结构化数据的过程称为反序列化的过程。

&emsp;&emsp;为什么一定要通过序列化和反序列化来处理结构呢？

* 为了应用层网络通信的方便，字符串比较方便传输；
* 为了方便上层方便使用本来的结构成员，使得应用和网络进行了解耦。

&emsp;&emsp;我们之前的tcp/udp服务器是没有序列化和反序列化这个过程的，因为我们没有应用场景。

&emsp;&emsp;这个结构化的数据的序列化和反序列化就是协议的体现。

&emsp;&emsp;那么怎么搞序列化和反序列化这个事情呢？造轮子就是自己写，用轮子就是自己去用别人写好的组件（如xml，json，protobuff）。

# 二、网络版本计算器

&emsp;&emsp;为了体验序列化和反序列化的过程，我们来写一个网络版本的计算器，我们考虑一下通信方案：

约定方案一:

* 客户端发送一个形如"1+1"的字符串;
* 这个字符串中有两个操作数, 都是整形;
* 两个数字之间会有一个字符是运算符, 运算符只能是 +;
* 数字和运算符之间没有空格;
* ...

约定方案二:

* 定义结构体来表示我们需要交互的信息;
* 发送数据时将这个结构体按照一个规则转换成字符串, 接收到数据的时候再按照相同的规则把字符串转
  化回结构体;
* 这个过程就是 "序列化" 和 "反序列化"

## 1 定制协议

&emsp;&emsp;我们先实现一个不序列化和反序列化的程序，思路是先定义好两边用的结构体（request_t response_t），然后封装一下拉TCP服务器的那几个接口，实现如下：

```cpp
// Protocol.hpp
#pragma once
#include <iostream>
#include <string>

using namespace std;

// 定制协议的过程 就是定制结构化数据的过程

// 请求格式
typedef struct request
{
    int _x;
    int _y;
    int _op;// +-/*%
}request_t;

// 相应格式
typedef struct response
{
    int code;// sever运算完毕的计算状态 code(0):success, code(-1):div 0 
    int result;// 计算结果
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

// 封装一下套接字编程
class Sock
{
public:
    // 1.创建套接字
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
    // 2. 绑定
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
// 每次都起一个线程去帮我们运算一下
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
    // version1 无序列化和反序列化
    // 做一个短服务 计算一次就断开
    // (读取字符串)->分析处理为request->制作response->(序列化)->close(sock)
    // 1. 读取请求
    request_t req;
    cout << "get a new link..." << endl;
    ssize_t s = read(sock, &req, sizeof(req));
    if (s == sizeof(req))
    {
        // 2.分析请求
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
// 这是一个短服务，发送一次请求 得到结果后打印结果直接断开
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
    // III send过去
    write(sock, &req, sizeof(req));
    // IV 等待结果并打印
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

&emsp;&emsp;服务端怎么知道用那种结构体呢？客户端怎么知道code什么意思呢？...这是因为我们约定了啊，我们这个约定就是一个很简单的协议，客户端和服务端都必须遵守才能正常运行。

&emsp;&emsp;我们直接传结构体能正常走起来，这是因为客户端和服务端的C++版本是一样的，内存对齐方式都是一样的，int都是4字节而不是两个版本不同，如果我们发生了一些如内存对齐的变化，直接就会出问题，所以直接发结构体看起来可行，实际上对于某些极端场景还是不行的。

## 2 使用JsonCpp进行序列化和反序列化

&emsp;&emsp;使用一条命令安装JsonCpp：

```shell
sudo yum install -y jsoncpp-devel
```

&emsp;&emsp;这一句命令实际上给我们下载了它的头文件和一些动静态库。

&emsp;&emsp;我们测试一下：

&emsp;&emsp;这里编译时要加 ``-ljsoncpp``，表示链接json库，简单用法见注释：

```cpp
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// 请求格式 测试序列化这个东西
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
    // Json::Value是一个KV式的能够承载对象的东西
    // 先把待序列化的数据承载到root中
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer是一个虚基类 有两个子类 FastWriter和StyledWriter
    Json::StyledWriter writer;
    // 调用write方法去把这个Value序列化
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;
    return 0;
}

```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145207.png)

&emsp;&emsp;运行结果如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145353.png)

&emsp;&emsp;换成 ``FastWriter``试试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719145549.png)

&emsp;&emsp;反序列化：

```cpp
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// 请求格式 测试序列化这个东西
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
    // Json::Value是一个KV式的能够承载对象的东西
    // 先把待序列化的数据承载到root中
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer是一个虚基类 有两个子类 FastWriter和StyledWriter
    //Json::StyledWriter writer;
    Json::FastWriter writer;
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;

    // 反序列化 把一个字符串转化为字符串
    // R"()"表示里头是一个原生字符串,省去了转义
    std::string JsonString = R"({"datax":10,"datay":20,"operator":43})";
    Json::Value Root;
    Json::Reader reader;
    reader.parse(jsonstring, Root);// 让这个序列化数据充斥Root
    // 然后用KV类似map一样去用“名字”拿它即可
    request_t ans;
    // 当做整数去拿
    ans._x = Root["datax"].asInt();
    ans._y = Root["datay"].asInt();
    //ans._op = Root["operator"].asInt();
    // 如果是char
    ans._op = (char)Root["operator"].asUInt();
    cout << "ans._x = " << ans._x << ", ans._y = " << ans._y << ", ans._op = " << ans._op << endl;
    return 0;
}

```

&emsp;&emsp;所以网络中我们只管序列化后的字符串jsonstring怎么收发即可，处理业务逻辑时再把字符串进行反序列化处理。

## 3 给我们的Cal增加序列化与反序列化

&emsp;&emsp;首先封装一下序列化与反序列化:

```cpp
// 序列化请求
std::string SerializeRequest(const request_t& req)
{
    Json::Value root;
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    Json::FastWriter writer;
    return writer.write(root);
}

// 回应序列化
std::string SerializeResponse(const response_t& res)
{
    Json::Value root;
    root["code"] = res.code;
    root["result"] = res.result;
    return Json::FastWriter().write(root);
}

// 反序列化请求
void Deserialization(const string& jsonstring, request_t& req)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring, root);
    req._x = root["datax"].asInt();
    req._y = root["datay"].asInt();
    req._op = (char)root["operator"].asUInt();
}

// 反序列化回应
void Deserialization(const string& jsonstring, response_t& res)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(jsonstring, root);
    res.code = root["code"].asInt();
    res.result = root["result"].asInt();
}
```

&emsp;&emsp;下面在我们给我们的sever和client增加序列化与反序列化：

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
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220719163113.png)

&emsp;&emsp;我们上面写的这个在线计算器，本质就是一个应用层网络服务，其中，通信代码是我们自己完成的，序列化和反序列化我们通过第三方组件jsoncpp完成了，业务逻辑是我们自己定的，请求、结果格式、code定义是我们自己约定的，我们这样就完成了一个**应用层网络服务。**

&emsp;&emsp;对应到OSI七层模型中，会话层就是我们写的通信代码，表示层就是我们的序列化与反序列化，应用层就是我们写的计算器业务逻辑，我们在写的时候发现这三层就是耦合在一起的，我们根本没法把它们彻底分开，所以TCP/IP中把这三层公称为应用层。

# 三、HTTP协议

&emsp;&emsp;虽然说应用层协议是我们自己定的，但是其实早就就很多程序员大佬定义了很多现成的，非常好用的应用层协议给我们直接使用，HTTP（超文本传输协议）协议就是其中之一。

&emsp;&emsp;HTTP协议本质上，和我们刚写的网络计算器没有区别，都是应用层的协议，所以HTTP内部要实现我们写的**网络通信、序列化与反序列化、应用层协议细节，大佬帮我们写好了，我们可以直接用HTTP协议。**

## 1 认识URL(网址)

&emsp;&emsp;网址是定位网络资源的一种方式。我们请求的图片、html、css、js、音频、视频、标签、文档这些都称为资源。

&emsp;&emsp;我们通过IP+port可以唯一的定位一个进程，公网IP是唯一的去确定一个主机的，而所谓的网络资源，它是以文件的方式存在一个网络主机上的。而Linux中确定一个文件的方法是通过绝对路径来标识的，所以IP + 路径就可以唯一的确定一份网络资源。

&emsp;&emsp;而IP通常是以域名的方式呈现的，路径通过目录名+分隔符/实现。

```
http://bs.scu.edu.cn/quanyuanjiaoshi.html
```

&emsp;&emsp;前面的http://表用请求该资源的方法，bs.scu.edu.cn表明域名，后面/quanyuanjiaoshi.html表示我们请求的资源，注意，这里根目录可能不是Linux里头的那个根目录，它可能是网路根目录。

&emsp;&emsp;在Linux中，可以通过命令：

```shell
ping 域名 
```

&emsp;&emsp;来看域名对应的端口号。

&emsp;&emsp;我们这里都不加端口号是因为端口号和我们开的服务类型是强相关的，这是大家已经约定俗称的东西，比如http用80，https用443，mysql用3306等。URL中还可以带参，？后面就是我的资源。

&emsp;&emsp;所以一个URL的作用就是确定网络中的一份资源。

## 2 urlencode与urldecode

&emsp;&emsp;像 / ? : 等这样的字符, 已经被url当做特殊意义理解了. 因此这些字符不能随意出现.

&emsp;&emsp;比如, 某个参数中需要带有这些特殊字符, 就必须先对特殊字符进行转义.

&emsp;&emsp;转义的规则如下:

&emsp;&emsp;将需要转码的字符转为16进制，然后从右到左，取4位(不足4位直接处理)，每2位做一位，前面加上%，编码成%XY格式

&emsp;&emsp;如C++就会被解析为C%2B%2B。

&emsp;&emsp;这些工具都可直接在网上搜索到。

## 3 初始HTTP

&emsp;&emsp;不论是请求还是相应，HTTP都是按行尾单位构建请求或响应的。无论是请求还是相应，基本都是由3或4部分组成。

&emsp;&emsp;它的内容如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721195613.png)

&emsp;&emsp;思考：既然HTTP是以行为单位的，那么如何解包/封装，如何分用呢？

&emsp;&emsp;请求行加请求报头就是HTTP的“报头”，请求正文就是HTTP的有效载荷；状态行加响应报头即HTTP的“报头”，响应正文即HTTP的有效载荷。

&emsp;&emsp;那么传输时，HTTP request和HTTP response如何看待呢？

&emsp;&emsp;在网络上中我们可以直接把这个东西看做长字符串就行，所以它的读取和发送都是按字符串发送的；

&emsp;&emsp;所以如何解包：用空行把长字符串一切为2，我们从头开始读，只要读到空行，就认为把报头读完了，封装反过来即可。

&emsp;&emsp;这个分用过程是通过应用层代码决定的。

## 4 最简单的HTTP lab

&emsp;&emsp;前面讲了这么多理论，我就想看看这些理论具体到代码上是什么样的，我们先写写代码看看吧。

&emsp;&emsp;HTTP底层也是基于TCP的，所以我们首先要把套接字那块代码拿过来。

&emsp;&emsp;前期流程一样，创建套接字，绑定端口号，设置监听，接受链接创建线程处理请求，开缓冲区读请求内容，这里读的时候我们不用read，换一个新的系统调用——``recv``：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721203024.png)

&emsp;&emsp;recv多的这个flags可以允许我们设置阻塞非阻塞，包括后续读取紧急指针这样的东西，我们这里默认设为0即可。

&emsp;&emsp;代码如下：

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
        std::cout << buffer << endl;// 把http请求的格式打印出来
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

&emsp;&emsp;那么怎么发送请求呢，我们有天然的工具，浏览器呀，使用浏览器在地址栏输入 公网IP+port给我们写的这个sever法请求。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721204123.png)

&emsp;&emsp;两个空行一个是HTTP里头的空行，另一个是我们代码里的endl，因为我们没有发送数据，所以没有正文。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721205900.png)

&emsp;&emsp;在比如说用手机请求：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721205428.png)

&emsp;&emsp;接下来我想构建一个响应发回去：

&emsp;&emsp;我们这里用send系统调用发送消息，他是Linux中专为TCP设计的，那么直接send消息就能发回去了吗？肯定不可以啊，你还没构建HTTP响应格式呢。

&emsp;&emsp;我们构建一个包含状态行，告知有效载荷是普通文本的Content-Type再加上空行，然后加一句文本的回复发过去，常见Content-Type对照表可以上网查：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721210918.png)

```cpp
	std::string http_response = "http/1.0 200 OK\n";// 状态码200 状态码描述OK
        // Content-Type 告知有效载荷是什么类型
        http_response += "Content-Type: text/plain\n";// test/plain表示正文是普通的文本
        http_response += "\n";// 空行
        http_response += "hello world";// 正文内容
        send(sock, http_response.c_str(), http_response.size(), 0);
```

&emsp;&emsp;测试一下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721211439.png)

&emsp;&emsp;序列化啊这些任务HTTP已经帮我们完成了，我们读到的就是一个大字符串。

&emsp;&emsp;所以HTTP协议本质就是得到请求字符串，根据请求构建相应，把响应通过套接字发回给对方。

## 5 HTTP解析

&emsp;&emsp;我们登陆百度的服务器，手动构建一个请求，看看所谓网页到底是啥：

&emsp;&emsp;登陆：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721212642.png)

&emsp;&emsp;请求与返回：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220721212825.png)

&emsp;&emsp;返回的东西就是一个html。
