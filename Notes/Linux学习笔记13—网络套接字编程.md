# 一、预备知识

## 1 IP地址

&emsp;&emsp;公网IP地址是唯一标识互联网中的一台主机的，我们作为互联网原住民，对这个其实是比较了解的。

&emsp;&emsp;那么有了源IP和目的IP就能在应用层编程了吗，并不是。

&emsp;&emsp;有了源IP和目标IP，最大的意义是能顾指导一个报文该如何进行路径选择。

## 2 端口号

&emsp;&emsp;报文到了目标IP地址的计算机上，难道路程就结束了吗，其实并没有，因为我们的数据是要给一个具体的进程用的，因为网络的通信最终是不同的计算机上的进程在相互交换数据，为了标识我是给哪个进程用的，我是发给哪个进程的，这就引出了端口号(Port)，端口号可以唯一的在一台计算机上标识一个进程。

端口号(port)是传输层协议的内容.

* 端口号是一个2字节16位的整数;
* 端口号用来标识一个进程, 告诉操作系统, 当前的这个数据要交给哪一个进程来处理;
* **IP地址 + 端口号能够标识网络上的某一台主机的某一个进程！**
* 一个端口号只能被一个进程占用。

&emsp;&emsp;它保证了双方的用户可以看到和发送接收的数据。

&emsp;&emsp;而**套接字就是IP地址加端口号组成**的一个实体。

&emsp;&emsp;那不禁有一个问题，为什么大家不用进程ID来表示进程呢？系统中PID就具有唯一性啊，但是通过PID一般很难区别进程的用途，并且操作系统的PID是依赖于OS的，如果OS的PID发生更改了难道意味着我网络的这个也要跟这变吗，这样增加了耦合性，非常的不好。

&emsp;&emsp;所以有了IP地址加端口号表明我们的互联网的世界本质上就是一个进程间通信的世界。

&emsp;&emsp;既然进程间通信需要让两个进程看到同一份资源，我们网络这里也是一样，需要先让两个进程都能看到一份资源，那他们看到的资源是什么呢？就是能够互通的套接字。

&emsp;&emsp;从我们上面的概念可以看出，一个进程可以关联多个端口号，但是一个端口号不可以绑定多个进程。

## 3 TCP协议

&emsp;&emsp;TCP是网络中的传输层中的一个协议，我们为什么要了解它呢，因为我们这里编写用户程序，就相当于在编写应用层程序，一定是会访问到传输层的东西的，因此我们要了解一下TCP协议。

&emsp;&emsp;TCP即传输控制协议，他是一个着重于可靠性的传输协议，他的特点如下：

* 传输层协议
* 有连接
* 可靠传输
* 面向字节流

## 4 UDP协议

&emsp;&emsp;和TCP协议一样，它也是一个传输层协议，UDP(User Datagram Protocol 用户数据报协议)，它的可靠性相对较低，具体特点如下：

* 传输层协议
* 无连接
* 不可靠传输
* 面向数据报

&emsp;&emsp;这里我们先不做详细解释，仅认为TCP是可靠传输，UDP是不可靠传输。这个可靠和不可靠并不是褒义或贬义，TCP既然保证可靠，它一定要画更多的资源，UDP反而不需要话那么多资源，所以不要根据我们的价值来判断好坏，而应该根据具体应用场景。选择时可以认为如果不是UDP有特定的优点需要用，那么统一用TCP。

## 5 网络字节序

&emsp;&emsp;我们已经知道,内存中的多字节数据相对于内存地址有大端和小端之分, 磁盘文件中的多字节数据相对于文件中的偏
移地址也有大端小端之分, 网络数据流同样有大端小端之分. 那么如何定义网络数据流的地址呢?

&emsp;&emsp;网络设计者的做法非常简单粗暴，规定所有到互联网的数据必须是大端的，如果你当前主机是小端，就先把数据转化成大端再发送，当解析数据时，如果是小端机，就把大端数据转化为小端再解析。**这是由TCP/IP协议规定的。**

&emsp;&emsp;为使网络程序具有可移植性,使同样的C代码在大端和小端计算机上编译后都能正常运行,可以调用以下库函数做网络
字节序和主机字节序的转换:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220715221438.png)

* 这些函数名很好记,h表示host,n表示network,l表示32位长整数,s表示16位短整数。
* 例如htonl表示将32位的长整数从主机字节序转换为网络字节序,例如将IP地址转换后准备发送。
* 如果主机是小端字节序,这些函数将参数做相应的大小端转换然后返回;
* 如果主机是大端字节序,这些函数不做转换,将参数原封不动地返回。

# 二、socket编程接口

## 1 socket常见API与一些预备知识

```cpp
// 创建 socket 文件描述符 (TCP/UDP, 客户端 + 服务器)
int socket(int domain, int type, int protocol);
// 绑定端口号 (TCP/UDP, 服务器) 
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
// 开始监听socket (TCP, 服务器)
int listen(int socket, int backlog);
// 接收请求 (TCP, 服务器)
int accept(int socket, struct sockaddr* address, socklen_t* address_len);
// 建立连接 (TCP, 客户端)
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

&emsp;&emsp;简单看一下这些API，发现它们都有一个结构 ``struct sockaddr``。

&emsp;&emsp;网络通信的标准方式有很多种：基于ip的网络通信（AF_INET）、原始套接字、域间套接字（用于本地通信，AF_UNIX)，那如果有这些不同的种类是不是要有三种套接字的API接口呢，这样并不好，用起来很不爽，既然都是套接字，不如把套接字接口统一化，也就是我们看到的这一套。

&emsp;&emsp;我们这里要使用并讲解的是基于IP的网络通信，它的结构 ``sockaddr_in``如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716135256.png)

&emsp;&emsp;其他的也对应有它的结构，比如 ``sockaddr_un ``如果每种都设计对应的API太过麻烦，所以OS设计者就设计了一个结构：``struct sockaddr``，内部再通过判断进行强转。

&emsp;&emsp;那可能学习过之前的接口的人就要问了，啊既然是C语言的接口，我们为啥不用 ``void*``来传参数呢，这样不是更方便吗？原因是这套接口被提出时，C语言还不支持 ``void*``的语法，后续因为兼容问题也就没改。

## 2 udp echo sever

&emsp;&emsp;我们希望client给sever发一个你好，sever收到你好然后写回给client，最后显示出来。

&emsp;&emsp;远程测试时，我在本机上装了个WSL作为client，使用我的阿里云服务器作为client：

### I 创建套接字

&emsp;&emsp;首先来看一个系统调用 ``socket``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716141031.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716141105.png)

&emsp;&emsp;第一个参数domain是你的套接字是什么种类的，这里的种类就是我们上面说的那种种类，我们可以用一系列宏去控制：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716141206.png)

&emsp;&emsp;第二个参数type是套接字类型，这个类型指的是你希望操作系统系统给你一个什么样套接字，看一些有的参数：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716141349.png)

&emsp;&emsp;``SOCK_STREAM``是指有链接的，可靠的，即TCP；``SOCK_DGRAM``即无连接的不可靠的，即UDP。

&emsp;&emsp;第三个参数protocol指的是协议类型，我们UDP和TCP这里全部设置为0。

&emsp;&emsp;返回的是一个文件描述符，失败则返回-1并且设置错误码。

&emsp;&emsp;先写udp_sever.cc，把基础打开套接字写好：

```cpp
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>

using namespace std;

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

    cout << "sock = " << sock << endl;
    return 0;
}
```

### II 绑定IP与端口号

&emsp;&emsp;那么作为一个服务器，我们必须让用户知道服务器的地址（IP+port），这样才能被客户访问。

&emsp;&emsp;一般服务器的port，必须是众所周知的，并且不可轻易改变。

&emsp;&emsp;所以我们的下一步就是给我们的sever绑定port和IP.

&emsp;&emsp;这就要用下一个API，bind：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716142959.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716143039.png)

&emsp;&emsp;第一个参数就是那个套接字的文件描述符，第二个参数就是要传入一个套接字相关信息的指针，比如我们下面这个图：

![img](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716135256.png)

&emsp;&emsp;第三个参数是传入结构体大小。

&emsp;&emsp;我们用的struct sockaddr_in在 ``<netinet/in.h>``和 ``<arpa/inet.h>``中，可以用下面这条命令查找一下：

```shell
grep -r 'struct sockaddr_in' /usr/include
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716143951.png)

&emsp;&emsp;然后我们就可以找到这个结构的定义：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716144112.png)

&emsp;&emsp;第一个就是填充它是一个什么种类的套接字，我们这里填AF_INET即可，第二个 ``in_port_t``是一个16位整形，第三个in_addr结构体里头就是放了一个32位的IP地址。

&emsp;&emsp;点分法IP转32位整数有系统提供的接口 ``inet_addr``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716145529.png)

&emsp;&emsp;这一步的具体细节写在注释里了，具体看注释：

```cpp
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
    // local.sin_addr.s_addr = inet_addr("*******");
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
```

### III 提供服务

&emsp;&emsp;我们udp并不是像文件一样去读取，而是用系统调用 ``recvfrom``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716151129.png)

&emsp;&emsp;返回值：读取到了多少个字节，-1表示失败。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716151213.png)

&emsp;&emsp;第一个是套接字文件描述符，二三个参数为缓冲区与缓冲区大小，第三个参数为读取方式，默认设置为0即可，后面的一个参数 ``struct sockaddr*``是说你要给我一段空间，我给你写上谁给你发的数据，它是与我的服务器通信的客户端的套接字信息。

&emsp;&emsp;看我们的要求，读了数据还要返回回去，所以还要用一个系统调用接口 ``sendto``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716152214.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716152243.png)

&emsp;&emsp;第一个参数是套接字文件描述符，第二个第三个参数是你要发的数据的缓冲区与它的长度，第四个参数是发送方式，默认设为0即可，第四个是发给谁，套接字信息的指针与后面是套接字结构的大小。

&emsp;&emsp;提供服务的代码如下：

```cpp
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
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
        // 把消息打印出来
        cout << "#client say : " << buffer << endl;
        string str = "hello client";
        // 给客户端发数据
        sendto(sock, str.c_str(), str.size(), 0, (struct sockaddr*)&peer, len);
    
    }
```

### IV 服务端代码汇总

&emsp;&emsp;细节后续再完善，基础功能已经有了：

```cpp
//udp_sever.cc
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
    // local.sin_addr.s_addr = inet_addr("*******");
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
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
        // 把消息打印出来
        cout << "#client say : " << buffer << endl;
        string str = "hello client";
        // 给客户端发数据
        sendto(sock, str.c_str(), str.size(), 0, (struct sockaddr*)&peer, len);
  
    }
    return 0;
}
```

## 3 udp echo client

### I 创建套接字

&emsp;&emsp;和服务端差不多：

```cpp
// 1. 创建套接字(udp)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    {
        cerr << "client socket create error, error code : " << errno << endl;
        return 1;
    }
```

### II 客户端为什么不需要bind端口号

首先客户端也是必须要有port的 但是客户端不需要显示绑定

因为如果客户端显示bind端口号，那么就把客户端程序和一个端口绑定了

而这个端口可能被其他程序占用呢

服务器端口是被公司严格管理的 而客户端端口并不会被明确管理规定。

所以客户端对端口的要求是只要有就行 端口不必特定绑定，一般由用户端OS自动给我bind，在client正常发送数据的时候，OS自动给你bind，采用的是随机端口号机制。

### III 使用服务

&emsp;&emsp;就是个收消息再发消息的逻辑，代码实现如下：

```cpp
// 3.使用服务

    // 填充好发送给谁的信息
    struct sockaddr_in sever;
    sever.sin_family = AF_INET;
    sever.sin_port = htons(atoi(argv[2]));
    sever.sin_addr.s_addr = inet_addr(argv[1]);
    const int NUM = 1024;
    char buffer[NUM];
    while (1)
    {
        // a. 客户端的数据从哪里来？
        string message;
        cout << "客户端请输入#:->" << endl;
        cin >> message;
        // b. 客户端数据要发给谁呢？
        // 通过客户端的命令行参数来传你要发给的服务器的IP和端口号
        // ./udp_client IP 端口号

        // 客户端发数据
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr *)&sever, sizeof(sever));
        // 客户端收数据
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&tmp, &len);
        cout << "sever say # : " << buffer << endl;
    }

```

### IV 客户端代码汇总

```cpp
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>

using namespace std;

void Usage(string proc)
{
    cout << "Use this proc as : \n\t" << proc << " sever_ip sever_port" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // 如果命令行参数个数错误 就打印一下使用说明
        Usage(argv[0]);
        return 0;
    }
    // 1. 创建套接字(udp)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cerr << "client socket create error, error code : " << errno << endl;
        return 1;
    }
    // 2. 客户端需要显示的bind吗？
    // 首先客户端也是必须要有port的 但是客户端不需要显示绑定
    // 因为如果客户端显示bind端口号，那么就把客户端程序和一个端口绑定了
    // 而这个端口可能被其他程序占用呢
    // 服务器端口是被公司严格管理的 而客户端端口并不会被明确管理规定。
    // 所以客户端对端口的要求是只要有就行 端口不必特定绑定
    // 3.使用服务

    // 填充好发送给谁的信息
    struct sockaddr_in sever;
    sever.sin_family = AF_INET;
    sever.sin_port = htons(atoi(argv[2]));
    sever.sin_addr.s_addr = inet_addr(argv[1]);
    const int NUM = 1024;
    char buffer[NUM];
    while (1)
    {
        // a. 客户端的数据从哪里来？
        string message;
        cout << "客户端请输入#:->" << endl;
        cin >> message;
        // b. 客户端数据要发给谁呢？
        // 通过客户端的命令行参数来传你要发给的服务器的IP和端口号
        // ./udp_client IP 端口号

        // 客户端发数据
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr *)&sever, sizeof(sever));
        // 客户端收数据
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&tmp, &len);
        cout << "sever say # : " << buffer << endl;
    }

    return 0;
}
```

## 4 本地测试

&emsp;&emsp;``netstat -nlup``可以查看当前的udp，运行服务器程序时，如果错误码是13表明权限不够，sudo即可，本地测试，IP可以用 ``127.0.0.1``表示环回。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716161811.png)

&emsp;&emsp;客户端启动：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716161951.png)

&emsp;&emsp;测试如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716163448.png)

## 5 远程测试

&emsp;&emsp;用自己的电脑与云服务器进行远程测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716165205.png)

PS：这里服务端获取客户端IP的方法

&emsp;&emsp;我们上面的服务端代码，获取客户端的套接字是在 ``struct sockaddr_in peer;``中，这个 ``peer.sin_addr``就是ip地址封装成的一个结构体(``struct in_addr``)，不过它是32位整数，所以我们调用一个函数把它变成我们的点分表示：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716171409.png)

&emsp;&emsp;上面我们写的UDP服务器和UDP客户端，它其实就是一个应用层程序，调用了传输层的UDP协议。

&emsp;&emsp;网络中没有字符串的概念，只有

## 6 加入应用逻辑—执行简单shell命令

&emsp;&emsp;我们希望能够把程序改成客户端发送Linux shell指令，服务器去执行相应命令，这样可以吗？

&emsp;&emsp;进程间通信那个shell实在是太麻烦了，我们用一个系统调用**popen**

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716190954.png)

&emsp;&emsp;这个函数底层原理是父进程通过fork创建子进程去执行命令，然后把执行结果放在文件中，后面去读这个文件即可获得结果，下面是我们的测试：

```cpp
// udp_client.cc

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>

using namespace std;

void Usage(string proc)
{
    cout << "Use this proc as : \n\t" << proc << " sever_ip sever_port" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // 如果命令行参数个数错误 就打印一下使用说明
        Usage(argv[0]);
        return 0;
    }
    // 1. 创建套接字(udp)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cerr << "client socket create error, error code : " << errno << endl;
        return 1;
    }
    // 2. 客户端需要显示的bind吗？
    // 首先客户端也是必须要有port的 但是客户端不需要显示绑定
    // 因为如果客户端显示bind端口号，那么就把客户端程序和一个端口绑定了
    // 而这个端口可能被其他程序占用呢
    // 服务器端口是被公司严格管理的 而客户端端口并不会被明确管理规定。
    // 所以客户端对端口的要求是只要有就行 端口不必特定绑定
    // 3.使用服务

    // 填充好发送给谁的信息
    struct sockaddr_in sever;
    sever.sin_family = AF_INET;
    sever.sin_port = htons(atoi(argv[2]));
    sever.sin_addr.s_addr = inet_addr(argv[1]);
    const int NUM = 1024;
    char buffer[NUM];
    while (1)
    {
        // a. 客户端的数据从哪里来？
        string message;
        cout << "客户端请输入#:->";
        getline(cin, message);
        // b. 客户端数据要发给谁呢？
        // 通过客户端的命令行参数来传你要发给的服务器的IP和端口号
        // ./udp_client IP 端口号

        // 客户端发数据
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr *)&sever, sizeof(sever));
        // 客户端收数据
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&tmp, &len);
        cout << "sever say # : " << endl << buffer << endl;
    }

    return 0;
}

// udp_sever.cc

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
```

&emsp;&emsp;测试结果：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716193325.png)

# 三、TCP套接字

## 1 TCP echo sever

&emsp;&emsp;同样的，学习TCP套接字，我们先写一个echo sever与cilent。

### I 创建套接字与绑定IP与端口号

&emsp;&emsp;创建套接字的接口依然是 ``socket``：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716195630.png)

&emsp;&emsp;这里第二个参数，由于我们是TCP套接字，所以设置成 ``SOCK_STREAM``:

&emsp;&emsp;TCP的套接字是流式的，它的读写就会很像我们进行文件流的读取那样，而UDP的套接字是用户数据报式的，所以写起来就和文件读写不太一样。

&emsp;&emsp;不过创建套接字和绑定端口号它和UDP看起来没啥区别，我们直接把代码给出来：

```cpp
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>

using namespace std;

int main()
{
    // 1. 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }

    return 0;
}
```

&emsp;&emsp;TCP和UDP的不同在于TCP是要建立链接的。

### II 设置套接字为listen状态

&emsp;&emsp;比如打电话，必须得对面接了你才能通信啊，所以TCP这里必须要先建立链接，然后才能通信。

&emsp;&emsp;那么建立链接一定是有人主动建立链接，另一个人被动接受链接。

&emsp;&emsp;稍微思考一下就会发现，一般是客户端去建立链接，服务器去接受链接。

&emsp;&emsp;我们现在写的是一个sever，所以需要不断周而复始的不间断的等待客户的到来，即不间断的等待建立链接。

&emsp;&emsp;所以我们要不断地给用户提供一个可以建立链接的状态，这就是**给套接字设置为listen状态，本质是允许用户连接。**

&emsp;&emsp;这里用的系统调用就是 ``listen``:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716202757.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716202853.png)

&emsp;&emsp;第二个参数我们先不管，先设置为5，

```cpp
// 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }

```

### III accept接受建立链接

&emsp;&emsp;这一步的系统调用接口为 ``accept``：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716203509.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716203555.png)

&emsp;&emsp;先看返回值描述，如果成功返回一个 ``descriptor``，即文件描述符，为什么这里还要返回文件描述符呢，不是前面已经有socket了吗？

&emsp;&emsp;想像一个拉客的例子，外面的人不断地拉客，每次拉到了就去店里喊一个服务员去服务客户，这里也是一样，前面的sock文件描述符相当于一个拉客的，我们称为监听套接字，这里我们的accept的返回值的文件描述符就好像出来的给客户服务的服务员，就是对应真正提供IO服务的套接字。

&emsp;&emsp;所以我们把前面的 建立套接字 的 ``sock``改为 ``listen_sock``，表明它是监听套接字。

&emsp;&emsp;下面是目前的代码部分：

```cpp
// 4. 
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
    }
```

### IV 提供服务

&emsp;&emsp;我们这里出于方便考虑，因为TCP是面向字节流的，所以我们可以用read和write接口，暂时不去用send和recv接口。

&emsp;&emsp;就读到信息，把它打印出来，顺便打印一下信息来源的ip与端口号，然后把信息还回去，尾巴加上一句话，表明是sever还给你的。

```cpp
// 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        while (1)
        {
            char buffer[1024] = {0};
            // 利用read和write提供服务.
            ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                buffer[s] = 0;
                cout << "client#: " << buffer << endl 
                << "message from ip : " << inet_ntoa(peer.sin_addr)
                << " port : " << ntohs(peer.sin_port) << endl;
                string ret(buffer);
                ret += "\nsever return to you (what you said last time).";
                write(new_sock, ret.c_str(), ret.size());
            }
            else if (s == 0)
            {
                // 返回值为0表示对端把链接关了
                cout << "client quit ... " << endl;
                // 关闭了链接就break出去
                break;
            }
            else 
            {
                cerr << "read error, error code : " << errno << endl;
                break;
            }
        }
    }
```

### V 服务端代码汇总

```cpp
// tcp_sever.cc

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>

using namespace std;

int main()
{
    // 1. 创建"监听"套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }
    // 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
    // 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        while (1)
        {
            char buffer[1024] = {0};
            // 利用read和write提供服务.
            ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                buffer[s] = 0;
                cout << "client#: " << buffer << endl 
                << "message from ip : " << inet_ntoa(peer.sin_addr)
                << " port : " << ntohs(peer.sin_port) << endl;
                string ret(buffer);
                ret += "\nsever return to you (what you said last time).";
                write(new_sock, ret.c_str(), ret.size());
            }
            else if (s == 0)
            {
                // 返回值为0表示对端把链接关了
                cout << "client quit ... " << endl;
                // 关闭了链接就break出去
                break;
            }
            else 
            {
                cerr << "read error, error code : " << errno << endl;
                break;
            }
        }
    }
    return 0;
}
```

## 2 TCP echo client

### I 准备工作

&emsp;&emsp;直接看代码把：

```cpp
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
  
    return 0;
}

```

### II 创建套接字

&emsp;&emsp;都一样，直接看代码吧：

```cpp
// 1.创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "socket create error, error code : " << errno << endl;
        return 2;
    }
```

### III connect向服务器发起链接

&emsp;&emsp;同样的，客户端不必显示的绑定端口号与IP，在下面链接的时候OS会帮我们匹配。

&emsp;&emsp;客户端也不用accept啊，它关心的是去与服务器创建链接，即系统调用 ``connect``:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716215702.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716215852.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716220507.png)

```cpp
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
```

### IV 使用服务

&emsp;&emsp;利用read和write系统调用接口写即可：

```cpp
// 3.链接成功 进行正常的业务请求
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
```

&emsp;&emsp;加一点调优，如果建立链接成功，服务器输出add a new connect，客户端输出connect success!。

### V 客户端代码汇总

```cpp
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

```

## 3 本地测试

&emsp;&emsp;``netstat -nltp``可以查看TCP链接，我们先跑起来服务器：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716222421.png)

&emsp;&emsp;启动客户端程序：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716222530.png)

&emsp;&emsp;测试成功：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716223413.png)

## 4 多台主机链接服务器调优(多进程版本)

&emsp;&emsp;我们当前写的这个程序，仅支持同时一个主机去链接我们的服务，我们当前服务器只有一个执行流，进去死循环后必须处理了一个链接才能管下一个，这种版本根本没人使用= =。

&emsp;&emsp;一般一个多执行流的sever是这样处理TCP服务的，主进程不断accept，accept成功后得到新套接字就立刻创建一个子执行流去处理这个新套接字的IO。

```cpp
// tcp_sever.cc
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

```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220716230506.png)

&emsp;&emsp;虽然我们现在做到了多主机链接服务器，但是发现链接断开后，文件描述符没有正常被我们关闭，父子进程之间的文件描述符我们还没有处理好怎么管理。

&emsp;&emsp;为了方便看，我们在服务端建立链接成功后打印一下ip：

```cpp
	cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;

	else if (s == 0)
        {
            // 返回值为0表示对端把链接关了
            cout << "client " << "[" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) 
            << "] quit..." << endl;
            // 关闭了链接就break出去
            break;
        }
```

&emsp;&emsp;为了防止文件描述符“泄漏”的问题，断开连接后，我们利用close关闭文件描述符：

```cpp
while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // 提供服务
        // 原来的版本就是直接去调用这个函数 非常弱智
        // 先写一个子进程版本 通过创建子进程来创建服务
        pid_t id = fork();
        if (id < 0)
        {
            cerr << "fork child process to process ServiceIO error, error code : " << errno << endl;
            return 4;
        }
        else if (id == 0)
        {
            // child proc
            ServiceIO(new_sock, peer);
            // 服务处理完了 关闭文件描述符
            close(new_sock);
            exit(0);
        }
        else 
        {
            // father proc
            // do nothing;
        }
        // ServiceIO(new_sock, peer);
    }
```

&emsp;&emsp;曾经被父进程打开的文件描述符会被子进程进程，不然也就没有匿名管道了。

&emsp;&emsp;那么子进程显然也是可以看到我们这个监听套接字listen_sock的，为了防止子进程或父进程误读，所以建议关闭不需要的文件描述符，即子进程应当关闭 ``listen_sock``，父进程应当关闭new_sock防止误读。

&emsp;&emsp;这里多进程版本才算没有文件描述符泄漏的写完了。

```cpp
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <signal.h>

using namespace std;

void ServiceIO(int new_sock, struct sockaddr_in& peer)
{
    while (1)
    {
        char buffer[1024] = {0};
        // 利用read和write提供服务.
        ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "client#: " << buffer;
            if (buffer[s - 1] != '\n') cout << endl;
            if (buffer[s - 1] == '\n') buffer[s - 1] = 0;
            cout << "message from ip : " << inet_ntoa(peer.sin_addr)
                 << " port : " << ntohs(peer.sin_port) << endl;
            cout << endl;
            string ret(buffer);
            ret += "\nsever return to you (what you said last time).";
            write(new_sock, ret.c_str(), ret.size());
        }
        else if (s == 0)
        {
            // 返回值为0表示对端把链接关了
            cout << "client " << "[" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) 
            << "] quit..." << endl;
            // 关闭了链接就break出去
            break;
        }
        else
        {
            cerr << "read error, error code : " << errno << endl;
            break;
        }
    }
}

int main()
{
    // 1. 创建"监听"套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }
    // 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
    // 把子进程退出的SIGCHLD信号忽略 让OS帮我们回收子进程资源 父进程去持续accept看看有没有新的链接
    signal(SIGCHLD, SIG_IGN);
    // 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // 提供服务
        // 原来的版本就是直接去调用这个函数 非常弱智
        // 先写一个子进程版本 通过创建子进程来创建服务
        pid_t id = fork();
        if (id < 0)
        {
            cerr << "fork child process to process ServiceIO error, error code : " << errno << endl;
            return 4;
        }
        else if (id == 0)
        {
            // child proc
            close(listen_sock);
            ServiceIO(new_sock, peer);
            // 服务处理完了 关闭文件描述符
            close(new_sock);
            exit(0);
        }
        else 
        {
            // father proc
            close(new_sock);
        }
        // ServiceIO(new_sock, peer);
    }
    return 0;
}

```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718094021.png)

&emsp;&emsp;这里不管怎么建立链接，父进程看到的accept文件描述符永远是4，因为监听套接字不关闭，就是从4开始，每次得到了这个accept套接字4，父进程创建子进程给子进程用这个文件描述符后，父进程关闭了4，下一个再建立链接，对父进程来说4又可以用了，之后再创建新的子进程即可，这样把父进程的文件描述符资源控制住了。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718094907.png)

## 5 技巧:不注册信号，让父进程无需等待子进程

&emsp;&emsp;去掉信号注册，加上这一行代码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718095401.png)

&emsp;&emsp;它的原理如下：

```cpp
	else if (id == 0)
        {
            // child proc
            close(listen_sock);
            // 加的那行代码
            if (fork() > 0) exit(0);// 这里退出子进程
            // 往后走的是孙进程
            // 孙进程是一个孤儿进程 OS帮我们管理资源
            ServiceIO(new_sock, peer);
            // 服务处理完了 关闭文件描述符
            close(new_sock);
            exit(0);
        }
```

&emsp;&emsp;但是我们这里还要回收子进程。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718095836.png)

## 6 多线程版本TCP服务器

&emsp;&emsp;PS，Linux使用STLthread库编译时需要链接pthread库。

```cpp
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>

using namespace std;

void ServiceIO(int new_sock, struct sockaddr_in peer)
{
    while (1)
    {
        char buffer[1024] = {0};
        // 利用read和write提供服务.
        ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "client#: " << buffer;
            if (buffer[s - 1] != '\n') cout << endl;
            if (buffer[s - 1] == '\n') buffer[s - 1] = 0;
            cout << "message from ip : " << inet_ntoa(peer.sin_addr)
                 << " port : " << ntohs(peer.sin_port) << endl;
            cout << endl;
            string ret(buffer);
            ret += "\nsever return to you (what you said last time).";
            write(new_sock, ret.c_str(), ret.size());
        }
        else if (s == 0)
        {
            // 返回值为0表示对端把链接关了
            cout << "client " << "[" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) 
            << "] quit..." << endl;
            // 关闭了链接线程版本就关闭文件描述符并且退出
            close(new_sock);
            break;
        }
        else
        {
            cerr << "read error, error code : " << errno << endl;
            break;
        }
    }
}

int main()
{
    // 1. 创建"监听"套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }
    // 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
    // 把子进程退出的SIGCHLD信号忽略 让OS帮我们回收子进程资源 父进程去持续accept看看有没有新的链接
    // signal(SIGCHLD, SIG_IGN);
    // 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // 利用STLthread库创建线程来处理
        thread t(ServiceIO, new_sock, peer);
        t.detach();
    }
    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718102109.png)

&emsp;&emsp;这里不必像多进程那里去关闭不必要的文件描述符，因为创建出的线程和主线程是共享文件描述符的，你关了主线程也没了，主线程关了子线程也没了，所以只能等断开链接时关闭accept返回的new_sock套接字。

&emsp;&emsp;利用监控脚本看看多线程的起来：

```shell
while :; do ps -aL | head -1 && ps -aL | grep 'tcp_sever'; sleep 1; echo "##################################"; done
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718104433.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718104534.png)

## 7 线程池版本TCP服务器

&emsp;&emsp;我们目前多进程(线程)的服务器有三个问题：

* 创建进程线程无上限，容易被恶意分子攻击或同时链接服务器而导致没那么多资源而挂掉；
* 新的链接到了才创建线程，这本来就是服务器该做的事情，不应该把时间转移到客户来了再创建。

&emsp;&emsp;为此，我们可以搞个线程池版本，把之前线程池代码贴过来，稍作修改即可：

```cpp
// ThreadPool.hpp
// 有一个队列 一把锁保护队列 一个条件变量告诉线程啥时候队列非空 是懒汉模式单例
// 第一次 GetInstance时初始化条件变量和锁 并且创建g_num个线程去等待条件变量(即任务假如队列)
// 使用这个线程池只要在获取单例时把任务放进来就行
#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "task.hpp"

namespace ns_threadpool
{
    const int g_num = 5;
    template <class T>
    class ThreadPool
    {
    private:
        int num_; // 线程池中的线程数量
        std::queue<T> task_queue_;
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
        static ThreadPool<T> *ins; //创建类内对象的指针
  

    public:
        static ThreadPool<T> *GetInstance()
        {
            static pthread_mutex_t sigmtx_ = PTHREAD_MUTEX_INITIALIZER;
            if (ins == nullptr)
            {
                pthread_mutex_lock(&sigmtx_);
                if (ins == nullptr)
                {
                    // 当前单例对象尚未被创建
                    ins = new ThreadPool<T>();
                    ins->InitThreadPool();
                    std::cout << "首次加载对象" << std::endl;
                }
                pthread_mutex_unlock(&sigmtx_);
            }
            return ins;
        }
        // 封装一下上锁
        void Lock()
        {
            pthread_mutex_lock(&mtx_);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&mtx_);
        }

        bool isEmpty()
        {
            return task_queue_.empty();
        }
        void WakeUp()
        {
            pthread_cond_signal(&cond_);
        }

    private:
        void poptask(T *out)
        {
            *out = task_queue_.front();
            task_queue_.pop();
        }

    private:
        // 构造函数弄成私有的 不能类外构造对象
        ThreadPool(int num = g_num) : num_(num)
        {
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&cond_, nullptr);
        }
        // 在类中，要让线程执行类内成员方法是不可行的
        // 因为pthread_create的语法是void* function(void*)
        // 而非静态类成员方法有this指针
        // 所以我们需要调用静态方法

        // 单例模式 删掉拷贝构造和赋值
        ThreadPool(const ThreadPool<T> &tp) = delete;
        ThreadPool<T> &operator=(const ThreadPool<T> &tp) = delete;

    public:
        static void *Rountine(void *args)
        {
            ThreadPool<T> *tp = (ThreadPool<T> *)args;
            pthread_detach(pthread_self());
            while (true)
            {
                // 从任务队列中拿任务 为了能拿任务队列里的东西，需要传递this指针
                tp->Lock();
                while (tp->isEmpty())
                {
                    // 任务队列为空 线程池的线程应该做什么呢？应该让线程挂起
                    // 利用条件变量
                    tp->wait();
                }
                T t;
                tp->poptask(&t);

                tp->Unlock();
                // 到锁外处理任务
                t();
                // sleep(1);
            }
        }
	// 创建线程
        void InitThreadPool()
        {
            pthread_t tid;
            for (int i = 0; i < num_; ++i)
            {
                pthread_create(&tid, nullptr, Rountine, (void *)this /*?*/);
            }
        }

        // 向任务队列放任务
        void pushtask(const T &in)
        {
            Lock();

            task_queue_.push(in);

            Unlock();
            WakeUp();
        }

        void wait()
        {
            pthread_cond_wait(&cond_, &mtx_);
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }
    };
    // 类外初始化静态成员
    template <class T>
    ThreadPool<T> *ThreadPool<T>::ins = nullptr;
}

// task.hpp
// 提供operator()来处理任务
// 它去调用Run()函数 在它里头就可以放我们本来的ServiceIO
// 因为我们用到了文件描述符new_sock和sockaddr_in peer
// 所以任务中增加这两个类型方便调用

#pragma once
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
using namespace std;

namespace Ns_task
{
    // 新的任务里头只有一个套接字
    class Task
    {
    private:
        int _sock;
        sockaddr_in _peer;

    public:
        Task() : _sock(-1) {}
        Task(int sock, sockaddr_in peer) : _sock(sock), _peer(peer) {}
        ~Task() {}
        void Run()
        {
            while (1)
            {
                char buffer[1024] = {0};
                // 利用read和write提供服务.
                ssize_t s = read(_sock, buffer, sizeof(buffer) - 1);
                if (s > 0)
                {
                    buffer[s] = 0;
                    cout << "client#: " << buffer;
                    if (buffer[s - 1] != '\n')
                        cout << endl;
                    if (buffer[s - 1] == '\n')
                        buffer[s - 1] = 0;
                    cout << "message from ip : " << inet_ntoa(_peer.sin_addr)
                         << " port : " << ntohs(_peer.sin_port) << endl;
                    cout << endl;
                    string ret(buffer);
                    ret += "\nsever return to you (what you said last time).";
                    write(_sock, ret.c_str(), ret.size());
                }
                else if (s == 0)
                {
                    // 返回值为0表示对端把链接关了
                    cout << "client "
                         << "[" << inet_ntoa(_peer.sin_addr) << ' ' << ntohs(_peer.sin_port)
                         << "] quit..." << endl;
                    // 关闭了链接线程版本就关闭文件描述符并且退出
                    close(_sock);
                    break;
                }
                else
                {
                    cerr << "read error, error code : " << errno << endl;
                    break;
                }
            }
        }
        void operator()()
        {
            Run();
        }
    };
}

// tcp_sever_ThreadPool.cc
// 这里头使用线程池
// 建立新链接后 创建任务 获取单例并把任务塞到队列里即可

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include "thread_pool.hpp"
#include "task.hpp"

using namespace std;
using namespace ns_threadpool;
using namespace Ns_task;

int main()
{
    // 1. 创建"监听"套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        cerr << "socket create fail, error code : " << errno << endl;
        return 1;
    }
    // 2. 绑定本地ip与端口号
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(711);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        cerr << "bind ip or port fail, error code : " << errno << endl;
        return 2;
    }
    // 3. TCP的特殊之处：sever设置套接字listen状态，表示当前可以建立链接
    const int backlog = 5;
    if (listen(listen_sock, backlog) < 0)
    {
        cerr << "listen fail, error code : " << errno << endl;
        return 3;
    }
  

    // 4. 使用accept获得的套接字像文件一样进行通信, 提供服务
    while (1)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        // 调用accept 创建一个真正提供IO的套接字
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            // 如果没有人和我建立链接 即没拉到客人 继续循环呗
            continue;
        }
        // 走到这里accept一定成功了 提供服务
        cout << "get a new link : [" << inet_ntoa(peer.sin_addr) << ' ' << ntohs(peer.sin_port) << "]" 
        << new_sock << endl;
        // 使用线程池 单例模式线程池
        // 使用方法 创建任务 然后放到任务队列里即可
        Task t(new_sock, peer);
        ThreadPool<Task>::GetInstance()->pushtask(t);
    }
    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718114121.png)

&emsp;&emsp;懒汉的创建过程：

![img](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718120047.png)

# 四、TCP协议通信流程

&emsp;&emsp;我们刚刚写的代码原理上是在干嘛呢？

* 创建socket的过程，即调用socket()的过程，本质是打开了一个文件，它里头没有网络相关的信息，仅有系统的信息；
* 绑定bind()时，要填IP和port，本质是将IP+port和socket的文件信息进行关联；
* 监听listen()时，本质是设置该socket的状态是允许别人来链接我；
* 链接connect()时，本质是发起链接，在系统层面就是发送一个请求报文过去；在网络层面，就是发起**TCP链接的三次握手；**
* 接受accept()，本质是获取新链接到应用层，即新的文件描述符，就是对应同一个监听套接字会有很多新链接，为了管理他们，创建一系列结构体的过程，然后以新的文件描述符的方式让进程能够找到这个新的链接，也就是说，这个新连接也是文件。
* read/write，本质是进行网络通信，对于用户来说，相当于在进行正常的文件读写。
* close(fd)，关闭文件，系统层面是释放曾经申请的文件资源；在网络层面，就是去通知对方我的链接已经关闭，在网络层面**进行四次挥手。**

&emsp;&emsp;那么我们究竟在干什么呢，如下图：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220718132653.png)

&emsp;&emsp;接下来我们就要把应用层到底是要做些什么（应用层逻辑、应用层的协议），然后自顶向下研究每一层（物理层除外）。
