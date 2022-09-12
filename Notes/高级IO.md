

# 一、五种IO模型

&emsp;&emsp;什么把数据刷到磁盘上、接受网络数据等等，这些工作都是操作系统内核完成的，其实我们调用系统调用的最终目的是进行**拷贝**操作，把数据从用户态缓冲区拷贝到内核缓冲区、或把数据从内核缓冲区拷贝到用户态缓冲区，我们把这些行为统称为**IO**，以读取网络数据为例，其实这就是一个生产者消费者模型，我们的用户态缓冲区是消费者，网络对方是生产者，内核缓冲区是临界区。

&emsp;&emsp;那么对端没有数据时，我们前面写的大部分程序都不得不等待，所以IO不只是有把数据拷过去，把数据读进来（也是一种拷贝），**等待也属于IO的一个环节**。所以定义:
$$
IO=拷贝数据+等待
$$
&emsp;&emsp;那么什么叫做高效的IO呢？

&emsp;&emsp;在软件层，本质上就是**减少单位时间内等待的比重**。

&emsp;&emsp;所有IO的话题基本都是在讨论两个东西：改变等待的方式、减少等待的比重。

## 1 一个小故事

&emsp;&emsp;我们以钓鱼为背景，鱼上钩了就是东西来了，然后把鱼钓上来就是相当于把数据从内核拷贝到用户态。

&emsp;&emsp;假设有一个渔友张三，张三是一个一心一意的人，等待时也不敢干别的事情，不管有没有鱼上钩，都死死的盯着钓竿，不干别的事情；

&emsp;&emsp;还有一个渔友李四，他并不像张三，等待时，他经常干别的事情，比如看看手机，刷刷软件，看看书等，然后检查一下鱼竿，看到鱼竿有上钩的情况，就把鱼钓上来；

&emsp;&emsp;还有一个渔友王五，他很聪明，他把一个铃铛挂在了鱼竿上，然后垂钓后，玩着手机，抽着烟，等着铃铛响了就说明鱼儿到了，就把鱼钓上来；

&emsp;&emsp;后来又来了一个渔友赵六，他比较rich，他想做大做强，然后一下子把500个鱼竿全部垂钓，然后不断的对着500个鱼竿轮训检测，判断是否有鱼儿到了；

&emsp;&emsp;后来又有一个渔友田七，他更加rich，是一个老板，他开车过来后，他也很想钓这个鱼，但是自己有事，于是他招了司机小王，和他说你我给你个桶、鱼竿、板凳，我把车开走去公司开会，你这边鱼钓满了，就给我打电话，我来接你。

&emsp;&emsp;那么从钓鱼效率上来讲，谁钓鱼效率更高呢？显然赵六（4号）的效率是最高的，因为他的鱼竿多，相当于一次可以检测多个文件描舒服``fd``，这种方式称为**多路转接**。

&emsp;&emsp;从钓鱼效率上来讲，张三和李四的效率是一样的，但是张三一直在等待，属于阻塞等待、而李四也会循环的来看是否鱼儿来了，如果时间不就绪，他就会干别的事情，定期循环来看是否就绪，属**非阻塞等待**。但是显然李四比张三做了更多的别的事情。

&emsp;&emsp;王五的思路显然比李四更好，不需要循环等待，干别的事情等着铃铛通知我，这是一种**信号驱动**。

&emsp;&emsp;田七这个思路他根本本身就没有钓鱼，也没有等待，只是发起了钓鱼，事情交给小王去完成，通过电话来让小王通知田七，相当于等待啊，拷贝啊我都不管，这种属于**异步IO**，他是拷贝我都不管了，相当于都不参与IO了，与此对应的，前四种IO称为同步IO。

## 2 阻塞IO

&emsp;&emsp;像我们前面用的大部分IO接口，什么read和write和recvfrom，都是阻塞IO，调用后就等待数据到达，数据到达后，从内核缓冲区拷贝到用户缓冲区，然后返回，这种就是典型的阻塞IO。

## 3 非阻塞IO

&emsp;&emsp;非阻塞等待就是，每次轮训问操作系统，数据准备好了咩，没准备好，立马返回，做别的事情；如果准备好了，就把数据拷贝过来，我们在前面使用非阻塞等待方式等待子进程是同样的思路。

## 4 信号驱动IO

&emsp;&emsp;首先自定义注册一个信号方法``SIGIO``,然后调用后做自己的事情，操作系统东西来了就会发信号，通过信号通知我们，然后就会去执行曾经注册的信号来拷贝数据。

## 5 多路转接IO

&emsp;&emsp;使用``select poll epoll``,他们相当于把等待工作直接交给着几个接口，调用方只负责拷贝，就绪了我来告诉你，你来拷贝，相当于把文件描述符交给这些接口，然后不断的用别的接口去只做拷贝工作。

## 6 异步IO

&emsp;&emsp;我调用异步IO函数，顺便把一个缓冲区给操作系统，当数据准备好了，操作系统给我发个信号，让我来处理就好了，进程本身是不参与IO，只是发起了IO，称为异步IO，只要等或者拷贝有一方你是参与的，你就不是异步IO，是同步IO。

## 7 什么叫做IO事件就绪

&emsp;&emsp;又叫做“等”时间就绪，分为写事件就绪和读事件就绪，就绪了意思就是你要的数据的个数已经超过了某个“阈值”，可以决定来拷贝到内核态了。

# 二、非阻塞IO的使用

## 1 fcntl系统调用

&emsp;&emsp;这个函数的功能是：

- 复制一个现有的描述符（cmd=F_DUPFD）. 
- 获得/设置文件描述符标记(cmd=F_GETFD或F_SETFD). 
- 获得/设置文件状态标记(cmd=F_GETFL或F_SETFL). 
- 获得/设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN). 
- 获得/设置记录锁(cmd=F_GETLK,F_SETLK或F_SETLKW).

&emsp;&emsp;我们利用功能三，可以吧文件描述符设置为非阻塞状态。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909164845.png)

&emsp;&emsp;下面的代码，是典型的阻塞等待：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    while (1)
    {
        char buffer[1024];
        ssize_t s = read(0, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = '\0';
            write(1, buffer, strlen(buffer));
        }
    }
    return 0;
}
```

&emsp;&emsp;利用那个系统调用改为非阻塞：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        perror("fcntl");
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

int main()
{
    // 把read设置为非阻塞
    SetNonBlock(0);
    while (1)
    {
        char buffer[1024];
        ssize_t s = read(0, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = '\0';
            write(1, buffer, strlen(buffer));
            printf("read success, s : %d, errno : %d\n", s, errno);
        }
        else 
        {
            printf("read fail, s : %d, errno : %d\n", s, errno);
        }
        sleep(1);
    }
    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909170148.png)

&emsp;&emsp;在非阻塞的情况下，如果数据没有就绪，系统会以错误的形式返回，其实并不是一种错误。

&emsp;&emsp;没有就绪和真正的出错都是以出错形式返回，那么怎么区分呢？利用``errno``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909170650.png)

&emsp;&emsp;这个EAGAIN就是错误码11的宏。

&emsp;&emsp;修改一下这个代码：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        perror("fcntl");
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

int main()
{
    // 把read设置为非阻塞
    SetNonBlock(0);
    while (1)
    {
        char buffer[1024];
        ssize_t s = read(0, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = '\0';
            write(1, buffer, strlen(buffer));
            printf("read success, s : %d, errno : %d\n", s, errno);
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("数据没有准备好，再试试吧.\n");
        }
        else 
        {
            printf("read error, error id : %d", errno);
        }
        sleep(1);
    }
    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909171103.png)

# 三、IO多路转接之select

&emsp;&emsp;实际上多路转接只负责一件事情——它只负责等的过程。

## 1 select是什么？

&emsp;&emsp;select只负责等待，得到fd就绪，通知用户层进行读取或写入，他本身**没有读取和写入的功能**。

&emsp;&emsp;但是read、write、recv、send本身也有等待的功能呀，为啥还要用你select呢？

&emsp;&emsp;select的作用是他可以同时等待多个fd，而上面的接口只能等待一个fd。

&emsp;&emsp;select的系统调用原型：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909172653.png)

**参数含义**

&emsp;&emsp;``nfds``是所等待的最大的文件描述符值+1，``nfds = maxfd + 1``。

&emsp;&emsp;``fd_set``是一个位图结构，比特位的位置代表哪一个文件描述符。

&emsp;&emsp;我们想要什么样的等待结果呢，一般是三类：读就绪、写就绪、异常就绪，所以这三个参数readfds、writefds、exceptfds表示关心这三个情况的位图。

&emsp;&emsp;以读为例，两点：1.用户告诉内核，你要帮我关心那些fd上的读事件就绪 2.内核告诉用户，你所关心的哪些fd的读事件已经就绪。 这就是select的核心功能。

&emsp;&emsp;这三个参数``readfds writefds exceptfds``都是**输入输出型参数**，输入时，几号文件描述符被置1了，就说明几号文件描述符需要别关心，输出时，如果某位被置1了，那么就说明这个事件就续了。

&emsp;&emsp;select有三种等待策略，1.只要不就绪，那就不返回——阻塞等待；2.只要不就绪，立马返回；——非阻塞等待3.设置好deadline，deadline之内如果如果补就绪就等待，就绪了就返回，deadline之外立刻返回。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909181857.png)

&emsp;&emsp;第一个参数是秒，第二个参数单位是微秒。

&emsp;&emsp;timeout参数也是一个输入输出型参数，输入时表示前面说的设置deadline，返回等待还剩余多少秒返回了，如果是超时了，时间就是0.

&emsp;&emsp;timeout一般有3种设置方式，nullptr：阻塞等待，timeout = {0, 0}: 非阻塞，timeout = {5, 0}:等5s。

**返回值含义**

&emsp;&emsp;表示有多少个事件就绪了，=0表示超时，小于0表示出错，如你有文件描述符根本就没打开你让我等。

**设置位图**

&emsp;&emsp;为了操作位图，os也给我们提供了接口：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909190049.png)

FD_CLR：把文件描述符fd设置出位图

FD_ISSET：判定一个文件描述符是否在位图中，

FD_SET：把文件描述符fd设置入位图

FD_ZERO：清空位图

&emsp;&emsp;那么这个位图有多大呢？测试一下：

```cpp
#include <iostream>
#include <sys/select.h>

int main()
{
    std::cout << sizeof(fd_set) * 8 << std::endl;
    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909191241.png)

## 2 select执行过程(select的坑)

&emsp;&emsp;select的执行过程，每一轮都是把位图设置好，然后去调用，然后读取位图看看这些时间的情况，再进行下一轮。

&emsp;&emsp;select因为使用输入输出型参数表示不同的含义，意味着每一轮调用select，都要对fd_set进行重新设置，这个重新设置就是select的缺点。

&emsp;&emsp;因为每次fd_set更新后，相当于原本的文件描述符信息就消失了，我们需要用户自己去保存原有的文件描述符，所以用第三方数组或容器来控制。

## 3 简易select_sever

&emsp;&emsp;我们写一个客户端给服务端发消息的，服务端不返回的select_sever。

&emsp;&emsp;初代版本：

```cpp
#include <iostream>
#include <sys/select.h>
#include "Sock.hpp"
#include <string>
#include <cstring>
#define NUM (sizeof(fd_set) * 8)
// >0 合法fd -1 没有值
int fd_array[NUM];

// ./select_sever 8080

static void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = (uint16_t)atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    memset(fd_array, -1, sizeof(fd_array));
    // 下面不该用accept 因为accept是阻塞等待
    // 站在多路转接的视角 认为链接到来就是读事件就绪
    // 对于所有服务器 最开始只有listen_sock
    // 事件循环
    fd_set rfds;
    fd_array[0] = listen_sock;
    while (1)
    {
        FD_ZERO(&rfds);
        int max_fd = fd_array[0];
        for (int i = 0; i < NUM; ++i)
        {
            if (fd_array[i] == -1) continue;
            FD_SET(fd_array[i], &rfds);
            max_fd = max(fd_array[i], max_fd);
        }
        FD_SET(listen_sock, &rfds);
        struct timeval timeout = {5, 0};
        // 我们希望把这个进程的所有fd都交给select来检查
        // 这样recv和accept只需要做真正的读写 而不是阻塞的等待
        int n = select(max_fd + 1, &rfds, nullptr, nullptr, &timeout);
        switch (n)
        {
        case -1:
            std::cout << "select error" << std::endl;
            break;
        case 0:
            std::cout << "select timeout" << std::endl;
            break;
        default:
            std::cout << "有fd的事件就绪了" << std::endl;
            break;
        }
    }

    return 0;
}
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909204057.png)

&emsp;&emsp;用telnet连接这个服务后，发现会有连接就绪：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909204528.png)

&emsp;&emsp;完整的select代码：

```cpp
#include <iostream>
#include <sys/select.h>
#include "Sock.hpp"
#include <string>
#include <cstring>
#define NUM (sizeof(fd_set) * 8)
// >0 合法fd -1 没有值
int fd_array[NUM];

// ./select_sever 8080

static void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = (uint16_t)atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    memset(fd_array, -1, sizeof(fd_array));
    // 下面不该用accept 因为accept是阻塞等待
    // 站在多路转接的视角 认为链接到来就是读事件就绪
    // 对于所有服务器 最开始只有listen_sock
    // 事件循环
    fd_set rfds;
    fd_array[0] = listen_sock;
    while (1)
    {
        FD_ZERO(&rfds);
        int max_fd = fd_array[0];
        for (int i = 0; i < NUM; ++i)
        {
            if (fd_array[i] == -1) continue;
            FD_SET(fd_array[i], &rfds);
            max_fd = max(fd_array[i], max_fd);
        }
        FD_SET(listen_sock, &rfds);
        struct timeval timeout = {1, 0};
        // 我们希望把这个进程的所有fd都交给select来检查
        // 这样recv和accept只需要做真正的读写 而不是阻塞的等待
        int n = select(max_fd + 1, &rfds, nullptr, nullptr, &timeout);
        switch (n)
        {
        case -1:
            std::cout << "select error" << std::endl;
            break;
        case 0:
            std::cout << "select timeout" << std::endl;
            break;
        default:
            std::cout << "有fd的事件就绪了" << std::endl;
            for (int i = 0; i < NUM; ++i)
            {
                if (fd_array[i] == -1) continue;
                // 下面的fd都是合法的fd 但是合法的fd不一定是就绪的fd
                if (FD_ISSET(fd_array[i], &rfds))
                {
                    std::cout << "socket : " << fd_array[i] 
                    << "上面有了读事件,可以读取了" << std::endl;
                    // fd_array[i]就绪了
                    if (fd_array[i] == listen_sock)
                    {
                        // 监听套接字就绪了
                        std::cout << listen_sock <<  "有了新的连接到来" << std::endl;
                        sockaddr_in peer;
                        int sock = Sock::Accept(listen_sock, peer);
                        if (sock >= 0)
                        {
                            // 获取成功
                            // 只是建立了新链接 不知道数据是否到来了
                            // 这里我们没法直接搞进去 但是可以搞进数组
                            std::cout << "获取新链接成功 : " << sock << std::endl;
                            int pos = 1;
                            for (; pos < NUM; ++pos)
                            {
                                if (fd_array[pos] == -1) break;
                            }
                            if (pos < NUM)
                            {
                                std::cout << "新链接: " << sock 
                                << "已经被添加到了数组的" << pos << "位置" << std::endl;
                                fd_array[pos] = sock;
                            }
                            else 
                            {
                                // 说明select已经满了 无法处理新的请求了
                                close(sock);
                            }
                        }
                    }
                    else 
                    {
                        // 普通套接字就绪了
                        // 也就是要读的套接字
                        // 可以用read或者recv读取
                        char recv_buffer[1024];
                        ssize_t s = recv(fd_array[i], recv_buffer, sizeof(recv_buffer) - 1, 0);
                        if (s > 0)
                        {
                            recv_buffer[s] = '\0';
                            std::cout << "client[" << fd_array[i] << "] echo : #" << recv_buffer << std::endl;
                        }
                        else if (s == 0)
                        {
                            // 读到0值 代表对端关闭了链接
                            close(fd_array[i]);
                            std::cout << "链接 : " << fd_array[i] << "关闭...client也已退出" << std::endl;
                            fd_array[i] = -1;
                            std::cout << "已经在数组fd_array下标" << i << "处去掉了套接字" << std::endl;
                        }
                        else 
                        {
                            // 读取失败
                        }
                    }
                }
            }
            break;
        }
    }

    return 0;
}
```

测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909224910.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909224935.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220909224949.png)

&emsp;&emsp;这样我们的服务器就能以单进程的方式处理多个链接了，select是真的复杂。。。

## 4 select的优缺点

&emsp;&emsp;**select**的优点（对比多进程多线程服务器）：可以一次等待多个fd，在一定程度上可以提高IO效率。

&emsp;&emsp;缺点：1.每次都需要重新设置需要关心那些文件描述符，所以每次都要遍历数组 2.fd_set的大小是确定的（1024），所以select能检测的文件描述符是有上限的,虽然进程能打开的文件描述符也是有上限个数的，但是还是比这个1024多：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910144603.png)

3.select底层需要轮询式的检测哪些fd的读/写事件就绪了。所以第一个参数是文件描述符+1是因为操作系统底层的遍历可能类似：

```cpp
for (int i = 0; i < N(max_fd + 1); ++i)
{
}
```

4.select可能会较为高频率的进行用户到内核、内核到用户的拷贝问题，每次都要重新设置文件描述符位图，每次os也要把这些重新关心，每次都要把fd_set从用户拷贝到内核，再从内核拷贝回用户，然后在位图作为输出型参数再修改一遍，然后再设置。。。很麻烦。

# 四、socket就绪条件

## 1 读就绪

- socket内核接收缓冲区中的字节数，大于等于低水位标记``SO_RCVLOWAT``.此时可以无阻塞的读取文件描述符，并且返回值大于0。
- socketTCP通信中，对端关闭链接，此时socket读，则返回0；
- 监听socket上有新的链接请求；
- socket上有未处理的错误

## 2 写就绪

- socket内核中，发送缓冲区中的可用字节数大于等于低水位标记``SO_RCVLOWAT``，此时可以无阻塞的读取该文件描述符，并且返回值大于0；
- socket的鞋操作被关闭（close活shutdown），对一个写关闭的socket进行操作，会触发``SIGPIPE``信号；
- socket上使用非阻塞connect链接成功或失败之后；
- socket上有未读区的错误。

# 五、IO多路转接之poll简介

## 1 poll是什么？

&emsp;&emsp;poll是对于select的进步方案。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910151200.png)

&emsp;&emsp;timeout单位是ms，-1则永久阻塞；

&emsp;&emsp;前两个元素是一个数组，类型是``struct pollfd``，长度是``nfds``。

&emsp;&emsp;struct pollfd中，fd表示哪个文件描述符，events表示你要请求关心哪些事件、revents表示返回给你的关心结果。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910151734.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910151756.png)

&emsp;&emsp;通过``events | POLLIN``运算把事件添加进来即可，检查时用``revents & POLLIN``即可。

&emsp;&emsp;他确实比select好用一点，我们吧select_sever的功能用poll实现一下，代码如下：

```cpp
#include <poll.h>
#include "Sock.hpp"
#include <string>
#include <cstring>
#include <iostream>

#define NUM 10000

struct pollfd arr[NUM];

int n = 0;

void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int listen_sock = Sock::Socket();
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    for (int i = 0; i < NUM; ++i)
    {
        arr[i].fd = -1;
    }
    arr[0].fd = listen_sock;
    arr[0].events |= POLLIN;
    ++n;
    while (1)
    {
        for (int i = 0; i < NUM; ++i)
        {
            if (arr[i].fd != -1)
                n = max(n, i + 1);
        }
        int ret = poll(arr, n, 1000);
        switch (ret)
        {
        case -1:
            std::cout << "poll error..." << std::endl;
            break;
        case 0:
            std::cout << "poll timeout..." << std::endl;
            break;
        default:
            // 读事件
            for (int i = 0; i < n; ++i)
            {
                if (arr[i].fd != -1 && (arr[i].revents & POLLIN))
                {
                    // 读事件就绪
                    if (arr[i].fd == listen_sock)
                    {
                        // 说明是有新的链接到来
                        sockaddr_in peer;
                        int newsock = Sock::Accept(listen_sock, peer);
                        int pos = -1;
                        for (int j = 0; j < NUM; ++j)
                        {
                            if (arr[j].fd == -1) {
                                arr[j].fd = newsock;
                                arr[j].events |= POLLIN;
                                pos = j;
                                break;
                            }
                        }
                        std::cout << "listen_sock : " << listen_sock << 
                        "上有新的链接到来, 已经放在数组下标[" << pos << "位置" << std::endl;
                    }
                    else 
                    {
                        // 其他套接字有读的内容了
                        char buffer[1024];
                        int s = recv(arr[i].fd, buffer, sizeof(buffer) - 1, 0);
                        if (s > 0)
                        {
                            // 读取成功
                            buffer[s - 1] = '\0';
                            std::cout << "client[" << i << "]$ : " << buffer << std::endl;
                        }
                        else if (s == 0)
                        {
                            // 对端关闭链接了
                            close(arr[i].fd);
                            arr[i].fd = -1;
                            std::cout << "client[" << i << "]已关闭..." << std::endl;
                        }
                        else 
                        {
                            // 读取失败
                            close(arr[i].fd);
                            arr[i].fd = -1;
                            std::cout << "client[" << i << "]读取错误，已关闭..." << std::endl;
                        }
                    }
                }
            }
            break;
        }
    }
}
```

## 2 poll的优势与劣势

&emsp;&emsp;他比select优势首先在于没有select一样的固定长度，但是缺点是我们还是遍历数组来招，如果文件描述符很多，就会很卡顿。

&emsp;&emsp;他的劣势可以总结为：和select一样，poll返回后，也要轮询pollfd数组来获取就去状态的描述符；每次调用poll都要吧大量的pollfd结构从用户态拷贝到内核态；同时链接大量的客户端在同一时刻可能只有恨少的文件描述符处于就绪态，因此随着监视的文件描述符数量增加，其效率也会线性下降。

# 六、IO多路转接之epoll

## 1 初识epoll

&emsp;&emsp;epoll是干嘛的呢，也和select和poll一样，只负责“等”，通过用户设置的某些fd及其事件，告知内核，让内核帮用户关心，一旦就绪，就通知上层，作用和select和poll是一样的。

&emsp;&emsp;按照man手册的说法，是为了处理大批量句柄而作了改进的poll（extend poll），它实在内核2.5.44版本中被引入的（MacOS中就没有epoll，但有与之类似的``kqueue``。

&emsp;&emsp;它具备很多优点，被公认为Linux2.6下性能最好的多路IO就绪通知方法。

## 2 epoll相关的系统调用

&emsp;&emsp;epoll有三个相关的系统调用：

```cpp
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

&emsp;&emsp;``epoll_create``是创建一个epoll句柄，其返回值是一个文件描述符，一样的，用完了这个epoll句柄我们要关掉这个文件描述符。

&emsp;&emsp;``epoll_ctl``是和前面的select/poll不同，在前面我们通过就这一个接口把用户通知内核、内核通知用户都完成了，epoll这里则是分开的，``epoll_ctl``的含义则是用户告诉内核哪些事件需要关注，这个通知不止添加文件描述符，也有告诉os不要关心哪些东西了的。

op的参数：ADD是注册新的fd到epfd中、MOD是已注册的fd的事件、DEL是从epfd中删除。

&emsp;&emsp;告诉内核一次想关心什么事件，内核就永远记住了，不像select和poll还要重新设置，如果要删除同样要用epoll_ctl删除。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910195813.png)

epoll_event结构：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910200330.png)

&emsp;&emsp;我们目前只关心events参数，他也是和poll类似，需要我们去用位运算添加或得到事件：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910200548.png)

&emsp;&emsp;同样我们关心``EPOLL_IN``可读了，``EPOLL_OUT``可写了。

&emsp;&emsp;``epoll_wait``则是内核告诉用户，哪些文件描述符就绪了。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910200843.png)

&emsp;&emsp;第二个参数和第三个参数共同构成一个数组缓冲区，得到哪些文件描述符的事件就绪了，timeout事件和select/poll一样，返回值是有几个事件就绪。

&emsp;&emsp;先把简单的epoll_sever，不涉及更多细节如下：

```cpp
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "Sock.hpp"

#define SIZE 128
#define NUM 64
// ./epoll_sever port
void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    // 1. 建立tcp监听套接字
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    // 2. 创建epoll模型
    int epfd = epoll_create(SIZE);
    // 3.添加listen_sock和他所关心的事件添加到内核
    struct epoll_event ev;
    ev.events = EPOLLIN;    
    // ev.data = 
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);
    // 4. 事件循环
    volatile bool quit = false;
    struct epoll_event revs[NUM];
    while (!quit)
    {
        int timeout = 1000;
        int n = epoll_wait(epfd, revs, NUM, timeout);
        switch (n)
        {
        case 0:
            std::cout << "timeout..." << std::endl;
            break;
        case -1:
            std::cout << "epoll error..." << std::endl;
            break;
        default:
            std::cout << "有事件就绪了..." << std::endl;
            break;
        }
    }
    close(epfd);
    return 0;
}
```

&emsp;&emsp;我们先把epoll的原理介绍了后，详细的吧epoll把epoll重新认识一下。

## 3 epoll的原理

&emsp;&emsp;在我们调用epoll时，内核会为我们维护一颗红黑树，红黑树节点的内容是一个文件描述符和它对应的事件，凡是在这个红黑树中的事件，都是os需要关心的。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220910220539.png)

&emsp;&emsp;那么为啥要用红黑树呢，如果我们不关心某个事件了，想把某个关心的节点删除，同时删除回调机制，那么红黑树是一个查找删除效率比较高的数据结构。

&emsp;&emsp;所以调用``epoll_create``，就是在内核中创建红黑树、创建就绪队列、告诉操作系统我们要用回调机制了。

&emsp;&emsp;调用``epoll_ctl``，就是插入节点到红黑树中，并且建立该fd的回调机制``ep_poll_callback``;或者删除节点。

&emsp;&emsp;调用``epoll_wait``，以``O(1)``的时间复杂度检查是否有事件就绪，其实就是检查等待队列是否为空，如果不为空，就把这些等待队列中的事件拷贝到你传的数组中，返回的n是就绪个数，直接根据n来遍历数组即可。

&emsp;&emsp;总结一下，使用``epoll``就是三板斧，调用``epoll_create``创建一个``epoll``句柄，它会为我们在内核中创建红黑树，准备就绪队列；调用``epoll_ctl``，把需要监控的文件描述符进行注册或取消关心，把它添加到红黑树中或从红黑树中删除，并且建立对这个事件建立回调机制；调用``epoll_wait``，等待文件描述符就绪，就是等一阵后，去就绪队列里检查是否为空，不为空则拷贝就绪事件到用户数组中。

&emsp;&emsp;红黑树节点的key就是fd，小的唯一值，非常完美。

## 4 完善epollsever

&emsp;&emsp;一样的，有事件就绪后，我们要判断这个fd是listen_sock还是普通sock，所以需要在``epoll_data``中专门处理，我们关心这个data.fd。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220911160054.png)

&emsp;&emsp;先完善成下面的样子：

```cpp
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "Sock.hpp"
#include <unordered_map>

#define SIZE 128
#define NUM 64
// ./epoll_sever port
void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    // 1. 建立tcp监听套接字
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    // 2. 创建epoll模型
    int epfd = epoll_create(SIZE);
    // 3.添加listen_sock和他所关心的事件添加到内核
    struct epoll_event ev;
    ev.events = EPOLLIN;
    // 这个事件关心的fd是listen_sock
    ev.data.fd = listen_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);
    // 4. 事件循环
    volatile bool quit = false;
    struct epoll_event revs[NUM];
    std::unordered_map<int, std::string> mp;
    while (!quit)
    {
        int timeout = 1000;
        int n = epoll_wait(epfd, revs, NUM, timeout);
        switch (n)
        {
        case 0:
            std::cout << "timeout..." << std::endl;
            break;
        case -1:
            std::cout << "epoll error..." << std::endl;
            break;
        default:
            std::cout << "有事件就绪了..." << std::endl;
            // 5.处理就绪事件
            for (int i = 0; i < n; ++i)
            {
                // 暂时方案
                int sock = revs[i].data.fd;
                std::cout << "文件描述符" << sock << "上有事件就绪..." << std::endl;
                if (revs[i].events & EPOLLIN)
                {
                    std::cout << "文件描述符" << sock << "上有读事件就绪..." << std::endl;
                    if (sock == listen_sock)
                    {
                        // 5.1 处理新链接到来
                        std::cout << "listen_sock : " << listen_sock << "上有新链接到来..." << std::endl;
                        sockaddr_in peer;
                        int new_sock = Sock::Accept(listen_sock, peer);
                        if (new_sock >= 0)
                        {
                            mp[new_sock] = std::string(inet_ntoa(peer.sin_addr));
                            // 把新链接添加到epoll中
                            struct epoll_event curev;
                            curev.events = EPOLLIN;
                            curev.data.fd = new_sock;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &curev);
                        }
                        else
                        {
                            // do nothing...
                        }
                    }
                    else
                    {
                        // 5.2 处理普通sock的读事件
                        std::cout << "sock : " << sock << "上有内容待读..." << std::endl;
                        char buffer[1024];
                        int s = recv(sock, buffer, sizeof(buffer) - 1, 0);
                        if (s > 0)
                        {
                            // 读到内容了
                            buffer[s - 1] = '\0';
                            std::cout << "client[" << sock << "] say $ " << buffer << std::endl;
                            // 读完后设置写响应
                            struct epoll_event curev;
                            curev.events = EPOLLOUT;
                            ev.data.fd = sock;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &curev);
                        }
                        else if (s == 0)
                        {
                            // 客户端关闭链接
                            close(sock);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, nullptr);
                            std::cout << "client[" << sock << "] exit..." << std::endl;
                        }
                        else
                        {
                            // 读取错误
                            close(sock);
                            epoll_event curcv;
                            curcv.events = EPOLLIN;
                            curcv.data.fd = sock;
                            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, &curcv);
                            std::cout << "client[" << sock << "] read error..." << std::endl;
                        }
                    }
                }
                else if (revs[i].events & EPOLLOUT)
                {
                    // 处理写事件
                }
                else
                {
                    // 其他
                }
            }
            break;
        }
    }
    close(epfd);
    return 0;
}
```

## 5 epoll的工作方式：LT模式/ET模式

&emsp;&emsp;LT是level triggered，水平触发；ET是edge triggered，边缘触发。

&emsp;&emsp;LT是只要你还没拿完这个就绪队列中的东西，每轮epoll_wait都会提醒我们；而ET是提醒我们一次后，不管有没有拿完就绪队列中的事情，都不再提醒了，相对效率会高一点。

&emsp;&emsp;显然，select/poll只有LT模式，没有ET模式，epoll默认处于LT模式。

&emsp;&emsp;这个水平的意思是示波器一直处于通知的高电频，称为水平触发，从低电频变化时去告诉的情况，称为边缘触发。

&emsp;&emsp;那么怎么修改epoll为ET模式呢？

&emsp;&emsp;在每次添加事件时，在修改epoll_event中的events成员变量改为``EPOLLIN | EPOLLET``即可。

&emsp;&emsp;修改listen_sock的事件状态，并且注释掉accept试一下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220911172600.png)

&emsp;&emsp;发现确实只提醒一次了。

&emsp;&emsp;但是有一个问题，ET模式下，有事件就绪只会通知一次，那如果准备读取时，怎么保证将所有的过来的东西全部读取完呢？比如来了三个链接，答案是不能保证。。。所以只能**循环读取**，那么什么时候读取完毕呢，循环读取一定在读取的最后一次卡住，也就是被阻塞住，这对服务器来说是不能接受的，**为了解决这个问题**，要将epoll在ET模式下的文件描述符设置为**非阻塞**。

# 七、基于ET模式epoll的Reactor模式

&emsp;&emsp;假设我们这边还是一个网络计算器，发送的请求格式如下，用X隔开：

```cpp
1+2X3+4x4+5x
// 返回
1+2=3X3+4=7X4+5=9X
```

&emsp;&emsp;我们设计一个ET模式的epoll服务器来处理这个情况。

注意：

1. 我们需要给每一个fd，都要有自己专属的输入输出缓冲区，不然内容就被冲掉了；
2. 前面的代码虽然已经对等和拷贝在接口层面实现了分离，但是在代码逻辑上，还是耦合在一起的；
3. epoll的最大优势是在**就绪事件通知机制**。

&emsp;&emsp;我们利用epoll的就绪事件派发机制，令epoll作为一个派发器dispatcher，让sock去自己调用注册好的回调函数进行读写。 

&emsp;&emsp;这里需要注意的点是发送功能不要一上来就让epoll打开，等发送缓冲区里有东西了再打开，不然发送事件一直是就绪的，发完了再关上，并且注意不论是读取还是写入都要使用非阻塞，并且我们把错误事件都交给EPOLLIN或EPOLLOUT然后去调用errorer去处理。

## 1 反应堆模式

&emsp;&emsp;Reactor又称反应堆模式，通过多路转接方案，被动的采用事件派发的方式，去调用对应的回调函数。

## 2 代码汇总

``Reactor.hpp``

构建了事件Event，Event中包含回调和缓冲区；构建了Reactor类，提供插入事件，删除事件，修改事件，派发就绪事件的功能；

```cpp
#pragma once

#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <unordered_map>
#include <unistd.h>
#include <cstdlib>

#define SIZE 128
#define NUM 128
// 缓冲区结构体

// 一般处理IO时 我们有3种接口需要处理
// 处理读取
// 处理写入
// 处理异常
class Event;
class Reactor;
typedef int (*callback_t)(Event *ev);

// 这个就是我们需要交给epoll管理的基本节点
class Event
{
public:
    int sock;              // 对应的文件描述符
    std::string inbuffer;  // socket对应的输入缓冲区
    std::string outbuffer; // socket对应的输出缓冲区
    // 给socket设置回调
    callback_t recver;
    callback_t sender;
    callback_t errorer;
    // 设置Event回指Reactor的指针
    Reactor *R = nullptr;

public:
    Event() : sock(-1), recver(nullptr), sender(nullptr), errorer(nullptr) {}
    // 注册回调
    void RegisterCallback(callback_t _recver, callback_t _sender, callback_t _errorer)
    {
        recver = _recver;
        sender = _sender;
        errorer = _errorer;
    }
    ~Event() = default;
};

// Reactor : Event = 1 : n
// 不需关心任何的sock类型（listen 读 写）
// 只关心如何使用该类对上面的Event进行管理
class Reactor
{
public:
    Reactor() = default;
    ~Reactor() = default;
    void InitReactor()
    {
        epfd = epoll_create(SIZE);
        if (epfd < 0)
        {
            std::cerr << "epoll_create error..." << std::endl;
            exit(2);
        }
        std::cout << "Init Reactor success ..." << std::endl;
    }
    bool InsertEvents(Event *evp, uint32_t evs)
    {
        // 1. 将ev中的sock插入到epoll中
        struct epoll_event ev;
        ev.events = evs | EPOLLET;
        ev.data.fd = evp->sock;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, evp->sock, &ev) < 0)
        {
            std::cerr << "epoll_ctl add event failed..." << std::endl;
            return false;
        }
        // 2. 将ev本身插入到哈希表中
        events[evp->sock] = evp;
        return true;
    }
    void DeleteEvents(Event *evp)
    {
        int sock = evp->sock;
        if (events.count(sock) != 0)
        {
            // 1. 从epoll中删除
            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, nullptr);
            // 2. 从哈希表中删除
            events.erase(sock);
            // 3.关闭套接字
            close(sock);
            // 4.evp指针的内容是new来的 必须delete掉
            delete evp;

        }
    }
    // 就绪事件的派发器逻辑
    // 根据epoll_wait的结果 去调用对应回调函数
    // 把就绪事件和IO正式读取进行了解耦
    void Dispatcher(int timeout)
    {
        struct epoll_event revs[NUM];
        int n = epoll_wait(epfd, revs, NUM, timeout);
        for (int i = 0; i < n; ++i)
        {
            int sock = revs[i].data.fd;
            uint32_t revents = revs[i].events;
            // 如果有出错事件 归类为读事件或写事件就绪
            // 也就是把所有的错误问题转化为让IO函数去解决
            if (revents & EPOLLERR)
                revents |= (EPOLLIN | EPOLLOUT);
            if (revents & EPOLLHUP)
                revents |= (EPOLLIN | EPOLLOUT);
            // 读事件就绪
            if (revents & EPOLLIN)
            {
                if (IsSockOK(sock) && events[sock]->recver != nullptr)
                {
                    // 直接调用回调函数 处理读取
                    events[sock]->recver(events[sock]);
                }
            }
            // 写事件就绪
            if (revents & EPOLLOUT)
            {
                if (IsSockOK(sock) && events[sock]->sender != nullptr)
                {
                    events[sock]->sender(events[sock]);
                }
            }
        }
    }
    // 修改部分
    bool EnableRW(int sock, bool enable_read, bool enable_write)
    {
        struct epoll_event ev;
        ev.events = EPOLLET |
                    (enable_read ? EPOLLIN : 0) | (enable_write ? EPOLLOUT : 0);
        ev.data.fd = sock;
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &ev) < 0)
        {
            std::cout << "epoll_ctl add event failed..." << std::endl;
            return false;
        }
        std::cout << "epoll_ctl enable success ..." << std::endl;
        return true;
    }

private:
    bool IsSockOK(int sock)
    {
        return events.count(sock) != 0;
    }
    int epfd = -1;
    // Epoll类管理的所有Event的集合 由文件描述符映射过来
    std::unordered_map<int, Event *> events;
};
```

``Accepter.hpp``

构造listen_sock的回调函数，他的回调函数是非阻塞的轮询accept，有新链接到来，就为新链接构建事件，并把它加入Reactor

```cpp
#pragma once

#include "Reactor.hpp"
#include "Sock.hpp"
#include "io_service.hpp"

int Accepter(Event* evp)
{
    // ET模式 持续读
    std::cout << "有新的链接到来了,监听的sock是 : " << evp->sock << std::endl;
    int listen_sock = evp->sock;
    while (1)
    {
        sockaddr_in peer;
        int sock = Sock::Accept(listen_sock, peer);
        if (sock < 0)
        {
            std::cout << "Accept Done" << std::endl;
            break;
        }
        // 获取链接成功
        SetUnBlock(sock);
        std::cout << "Accept success, sock = " << sock << std::endl;
        Event* newevp = new Event();
        newevp->sock = sock;
        newevp->R = evp->R;
        // 注册回调
        newevp->RegisterCallback(Recver, Sender, Errorer);
        // 新的链接到来 不能直接加EPOLLOUT 不然写肯定一直就绪 要按发过来的需要来回
        evp->R->InsertEvents(newevp, EPOLLIN);
    }
    return 0;
}
```

``io_service.hpp``

普通套接字的回调方法，sender、recver、errorer，他们分别完成了把数据读取后分割并计算结果，把结果写到输出缓冲区并激活sender功能、发送内容并关闭sender功能、统一处理错误。

```cpp
#pragma once

#include "Reactor.hpp"
#include <vector>
#include <cerrno>
#include "Util.hpp"

#define ONCE_SIZE 128

// 1 本轮读取完成
// -1 读取出错
// 0 对端关闭链接
static int RecverCore(int sock, std::string &inbuffer)
{
    while (1)
    {
        char buffer[ONCE_SIZE];
        ssize_t s = recv(sock, buffer, ONCE_SIZE - 1, 0);
        if (s > 0)
        {
            buffer[s - 1] = 0;

            // 读取成功
            inbuffer += buffer;
            if (inbuffer.back() == '\n') inbuffer.pop_back();
            std::cout << "RecverCore inbuffer : " << inbuffer << std::endl;
        }
        else if (s < 0)
        {
            // IO被信号打断
            if (errno == EINTR)
                continue;
            // 1.读完底层没数据了
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return 1;
            }
            // 2.真的出错了
            return -1;
        }
        else
        {
            // 对端关闭链接了
            return 0;
        }
    }
}

int Recver(Event *evp)
{
    // 1.真正的读取
    std::cout << "Recver has been called..." << std::endl;
    int result = RecverCore(evp->sock, evp->inbuffer);
    if (result <= 0)
    {
        // 差错处理 都放到注册的差错处理函数中
        if (evp->errorer)
            evp->errorer(evp);
        return -1;
    }
    // std::cout << "Split..." << std::endl;
    // 2.分包 解决粘包问题
    std::vector<std::string> tokens;
    std::string sep = "X";
    SplitSegment(evp->inbuffer, &tokens, sep);
    // 3.反序列化 针对一个报文
    for (auto &&seg : tokens) // 1+2 2+3
    {
        std::string data1, data2;
        // std::cout << "D..." << seg << std::endl;

        if (Deserialize(seg, data1, data2)) // 反序列化
        {
            // 4.业务逻辑 得到结果
            std::cout << data1 << ' ' << data2 << std::endl;
            int x = std::stoi(data1);
            int y = std::stoi(data2);
            int z = x + y;
            // 5.构建响应
            std::string res = data1;
            res += "+";
            res += data2;
            res += "=";
            res += std::to_string(z);
            res += "X";
            // 放到outbuffer中
            evp->outbuffer += res;
            std::cout << "outbuffer : " << evp->outbuffer << std::endl;
        }
    }
    // 6.尝试进行发送
    // 必须条件满足了(写事件就绪) 才能发送
    // 你的写事件就绪 但对方用户可能不一定是就绪的
    // 对于写事件 通常是按需设置

    if (!evp->outbuffer.empty())
    {
        // 如果outbuffer非空 说明用户有东西要发了
        // 刚刚我们增加了修改事件功能
        // 现在用户发送缓冲区里有东西了 按需来说 我们现在可以发了
        // 让Reactor给我们这个事件开启读写功能
        std::cout << "EnableRW..." << std::endl;
        evp->R->EnableRW(evp->sock, true, true);
    }
    return 1;
}

// return val
// 1:数据全部发完
// 0:数据没发完 但是不能再发了 等下次再发吧
// -1:错误值
static int SendCore(int sock, std::string &outbuffer)
{
    // 非阻塞发送
    int total = 0;
    int size = outbuffer.size();
    while (1)
    {
        const char *start = outbuffer.c_str();
        ssize_t curcnt = send(sock, start + total, size - total, 0);
        if (curcnt > 0)
        {
            // 发送成功
            total += curcnt;
            if (total == size)
            {
                // 全部数据发送完成
                outbuffer.clear();
                return 1;
            }
        }
        else
        {
            // 发送失败
            // 数据没发完 但是不能再发了
            if (errno == EINTR)
                continue; // 信号中断了IO
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                outbuffer.erase(0, total);
                return 0;
            }
            return -1;
        }
    }
}

int Sender(Event *evp)
{
    std::cout << "Sender has been called..." << std::endl;
    evp->outbuffer += "\n";
    // 直接向缓冲区写入 写完后注意要把Reactor的写事件关闭
    int result = SendCore(evp->sock, evp->outbuffer);
    if (result == 1)
    {
        // 全部发完了
        // 关闭写 保留读
        evp->R->EnableRW(evp->sock, true, false);
    }
    else if (result == 0)
    {
        evp->R->EnableRW(evp->sock, true, true);
    }
    else
    {
        // 差错处理交给Errorer
        if (evp->errorer)
            evp->errorer(evp);
    }
    return 1;
}

int Errorer(Event *evp)
{
    std::cout << "Errorer has been called..." << std::endl;
    // 发生错误了就删掉evp事件吧
    evp->R->DeleteEvents(evp);
    return 1;
}
```

``Util.hpp``

一些工具，分割输入、读取数字、设置文件描述符非阻塞。

```cpp
#pragma once 

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>

// 工具
// 设置一个接口为非阻塞
void SetUnBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        std::cerr << "fcntl error..." << std::endl;
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

void SplitSegment(std::string& inbuffer, std::vector<std::string>* tokens, std::string sep)
{
    while (1)
    {
        std::cout << "inbuffer : " << inbuffer << std::endl;
        auto pos = inbuffer.find(sep);
        if (pos == std::string::npos)
        {
            // 半个报文的情况 总之是报文不完整
            break;
        }
        std::string sub = inbuffer.substr(0, pos);
        tokens->push_back(sub);
        inbuffer.erase(0, pos + sep.size());
    }
}
bool Deserialize(const std::string& seg, std::string& out1, std::string& out2)
{
    // 1+2
    std::string op = "+";
    auto pos = seg.find(op);
    if (pos == std::string::npos) return false;
    out1 = seg.substr(0, pos);
    out2 = seg.substr(pos + op.size());
    return true;
}
```

``epoll_sever.cc``

主函数，创建Reactor并且把事件加入Reactor中，然后不断的派发事件。

```cpp
#include "Reactor.hpp"
#include "Sock.hpp"
#include "Accepter.hpp"
#include "Util.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    // 1.创建socket
    int listen_sock = Sock::Socket();
    SetUnBlock(listen_sock);
    uint16_t port = atoi(argv[1]);
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    // 2.创建Reactor对象
    Reactor* R = new Reactor();
    R->InitReactor();
    // 3.给Reactor中加东西
    Event* evp = new Event();
    evp->sock = listen_sock;
    evp->R = R;
    // 注册一个链接管理器Accepter
    evp->RegisterCallback(Accepter, nullptr, nullptr);
    R->InsertEvents(evp, EPOLLIN);
    // 4.开始进行事件派发
    while (1)
    {
        R->Dispatcher(1000);
    }
}
```





















