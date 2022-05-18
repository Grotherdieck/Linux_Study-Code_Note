&emsp;&emsp;不要混淆信号和信号量，它俩没有任何关系。

&emsp;&emsp;本章要学习普通信号的全生命周期。

# 一、准备背景知识

&emsp;&emsp;人类的存在比计算机早很多，计算机的信号概念也是来自生活的。

&emsp;&emsp;生活中有没有类似的信号场景呢？如闹钟、红绿灯、烽火台的烽火等等。

&emsp;&emsp;当我们看到这些场景触发的时候，我们是不是都能立马联想到要干什么事。

&emsp;&emsp;信号的产生就代表场景的触发，在Linux中，信号是给**进程**发的，进程要在合适的时候，执行对应的动作。

&emsp;&emsp;考虑，是否是场景真正的在一个人面前，我们才能只要这时候要怎么做吗？显然不是的，我们知道要怎么做和场景触发是没有直接关联的。

&emsp;&emsp;对于信号的处理动作，我们早就知道对应该怎么做了，甚至早于信号的产生。

&emsp;&emsp;人对信号的处理动作，是经过学习经验的结果，那么学习经验的本质就是记住了这种映射。

&emsp;&emsp;类比，所以进程在还没收到信号的时候，进程应该知道怎么处理相应的信号。

&emsp;&emsp;那么进程是怎么知道怎么处理相应的信号呢？是编写OS的工程师在写进程的源代码的时候就设置好了。

&emsp;&emsp;类比也可以得出结论：**进程具有识别处理信号的能力，一定早于信号的产生**。

&emsp;&emsp;那是我们看到信号就一定要处理吗？显然不是的，正常情况下，收到某种信号的时候，并不一定会立即处理这个信号，会综合考虑优先级等情况，再处理。

&emsp;&emsp;原因是信号随时都有可能产生，但是我当前可能做着更重要的事情，也就是说信号产生是异步的。

&emsp;&emsp;类比得到结论：**进程收到某种信号的时候，并不是立即处理的，而是等到合适的时候处理**。

&emsp;&emsp;既然信号不能立即被处理，那么已经到来的信号是不是应该暂时储存起来，**所以进程在收到信号后，要先把信号保存起来，等到合适的时候在处理**。

&emsp;&emsp;那应该保存在哪里呢？毫无疑问，``task_struct``。

&emsp;&emsp;**信号的本质也是数据，信号的发送本质就是往task_struct结构体中写入对应的数据**。

&emsp;&emsp;``task_struct``是一个内核数据结构，定义进程对象，而内核不相信任何人，用户不可对内核数据结构进行写入，所以是谁向``task_struct``中写入信号数据的呢？是``OS``！

&emsp;&emsp;**所以无论我们的信号如何发送，本质都是通过os发送的**。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422213721.png" style="zoom:80%;" />

# 二、信号产生的各种方式

## 1 基本入门—signal函数修改信号处理动作

&emsp;&emsp;一个最简单的死循环进程：

```cpp
//test.c
#include <stdio.h>
#include <unistd.h>

int main()
{
    while (1)
    {
        printf("hello world!\n");
        sleep(1);
    }
    return 0;
}
```

&emsp;&emsp;当它一直跑，影响我们操作的时候，之前讲过，使用``ctrl + c``可以终止进程：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422214212.png" style="zoom:80%;" />

&emsp;&emsp;使用``kill -l``查看Linux系统支持的信号列表:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422214256.png" style="zoom:80%;" />

&emsp;&emsp;前31个(1 ~ 31)是普通信号，后31个(34 ~ 64)是实时信号。

&emsp;&emsp;当我们``ctrl + c``时，**本质是向目标进程发送2号信号**，那么怎么证明呢？

&emsp;&emsp;利用一个函数``signal``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422214614.png" style="zoom:80%;" />

&emsp;&emsp;``sighandler_t``是一个参数为``int``，返回类型为``void``的函数指针。

&emsp;&emsp;第一个参数是一个整数，就是信号名，也可以使用宏值。

&emsp;&emsp;signal函数的作用是修改进程对信号的默认处理动作！

```cpp
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signal)
{
    printf("I got a signal, signal id : %d, pid : %d \n", signal, getpid());
}


int main()
{
    // 通过signal函数 把2号动作处理为我们的特定动作
    signal(2, handler);
    while (1)
    {
        printf("hello world! My pid : %d\n", getpid());
        sleep(1);
    }
    return 0;
}
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422215432.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422215522.png" style="zoom:80%;" />

``ctrl \``将这个进程终止掉：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422215558.png" style="zoom:80%;" />

&emsp;&emsp;通过这个看看键盘按键都是啥信号？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427190313.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427190446.png" style="zoom:80%;" />

&emsp;&emsp;信号产生的一种方式就是通过键盘产生，键盘产生的信号只能用来终止前台程序（即阻止你命令的进程），后台启动进程，在命令行最后加``&``就行后台运行，这时没法接受键盘信号。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427190906.png" style="zoom:80%;" />

&emsp;&emsp;后台进程虽然不能被键盘信号杀死，我们可以通过``kill -9 pid``杀掉它。

&emsp;&emsp;总结：进程收到信号后的处理方案有三种：

- 默认动作，一部分是终止自己，暂停等；
- 忽略动作，也是一种信号的处理方式，就是什么也不干；
- 自定义动作，比如我们刚刚用我们的``signal``方法就是在修改进程的信号自定义动作，通过switch语句可以对不同的信号做不同的响应。

&emsp;&emsp;以下实验说明**9**号信号不可被自定义：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427191639.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427191955.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427191818.png" style="zoom:80%;" />

## 2 键盘命令产生信号与运行时软硬件错误收到os发的信号

- 键盘产生；
- 进程中存在异常问题导致软硬件错误，会收到操作系统发的信号，默认行为就是异常退出；

&emsp;&emsp;野指针或数组越界时，有时会发生段错误(``Segmentation fault``)：

&emsp;&emsp;那么为啥会崩溃呢？对比下面两份代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427192644.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427192709.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427192814.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427192842.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427192908.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427194414.png" style="zoom:80%;" />

&emsp;&emsp;再看看除0的浮点数错误：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193045.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193157.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193637.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193702.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193234.png" style="zoom:80%;" />

&emsp;&emsp;在Win和Linux中，进程崩溃的本质是进程收到了对应信号，执行信号对应的默认行为（杀死进程），进而导致了进程崩溃。

&emsp;&emsp;那为啥发现这种行为会被发送对应信号呢？见下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427193835.png" style="zoom:80%;" />

&emsp;&emsp;而一般的处理错误方式就是终止进程，这就是进程崩溃的原因。

&emsp;&emsp;一个进程异常崩溃时，我们希望获得崩溃的原因，即获得对应收到的信号，而前面学习过，``waitpid``时拿到的``status``，它的低七位(``status & 0x7f``)就是对应的信号，即它崩溃的原因，所以进程崩溃的原因我们是可以获取的。

&emsp;&emsp;一般我们还想知道怎么解决啊？**我得知道在哪一行崩溃的哇，这样才能对应解决问题**。

&emsp;&emsp;而我们之前学习过的``core dump``标志就是来解决这一需求的。

&emsp;&emsp;在Linux中，当一个进程正常退出时，退出码和退出信号都会被设置；当一个进程异常退出时，进程的退出信号会被设置，表明进程退出的原因；如果必要，OS会设置退出信息中的``core dump``标志位，并将进程在内存中的数据转出到磁盘中，方便后期调试。

&emsp;&emsp;``ulimit -a``：查看系统资源，可以查看我们的core dump是否开启

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195156.png" style="zoom:80%;" />

&emsp;&emsp;``ulimit -c 10240``：允许我们``core dump``操作：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195339.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195404.png" style="zoom:80%;" />

&emsp;&emsp;这是一个二进制文件，后续我们可以用这个进行调试；

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195437.png" style="zoom:80%;" />

&emsp;&emsp;首先编译时加上-g选项，允许我们的程序被gdb调试，然后再gdb中，使用命令``core-file core.pid``，就可以知道哪错了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195712.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427195815.png" style="zoom:80%;" />

&emsp;&emsp;所以``core dump``标志的含义就是如果进程异常退出时被``core dump``，则该位置会被设置成1。

&emsp;&emsp;并不是所有的信号都有``core dump``，2号信号和9号信号就没有这个机制，3号信号就有。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200158.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200212.png" style="zoom:80%;" />

&emsp;&emsp;我们利用``fork``和``waitpid``来验证一下``coredump``时，``coredump``标志会被设置。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200644.png" style="zoom:80%;" />

&emsp;&emsp;有浮点数错误且打开了``coredump(ulimit -c 10240)``功能时：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200741.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200842.png" style="zoom:80%;" />

&emsp;&emsp;浮点数没有错误时：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200825.png" style="zoom:80%;" />

&emsp;&emsp;关掉``core dump``功能：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427200933.png" style="zoom:80%;" />

&emsp;&emsp;所以现在信号有两种产生方案了：键盘产生信号、进程异常产生信号。

## 3 系统调用产生信号

&emsp;&emsp;还有一个系统调用也可以产生信号：``kill``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427201215.png" style="zoom:80%;" />

&emsp;&emsp;所以我们可以仿制一个``kill``命令，利用命令行参数：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427201911.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427202253.png" style="zoom:80%;" />

&emsp;&emsp;``raise``函数可以向自己发任意信号。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427202549.png" style="zoom:80%;" />

&emsp;&emsp;验证一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427203147.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427203251.png" style="zoom:80%;" />

&emsp;&emsp;``abort``可以向自己发送6号信号。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427202641.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427203003.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427203022.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427203045.png" style="zoom:80%;" />

## 4 软件条件产生信号

&emsp;&emsp;除以上三点外，软件条件，也能产生信号，通过某种软件（OS)，来触发信号的发送，系统层面设置定时器，或者某种操作而导致条件不就绪这样的情景下，就会触发信号的发送。

&emsp;&emsp;如进程间通信中，读端直接不读且把读``fd``关了，写端还一直在写，最终写进程会收到``sigpipe(13)``，就是一种典型的条件触发的信号发送。

&emsp;&emsp;如系统调用``alarm``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427204605.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427204650.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427205131.png" style="zoom:80%;" />

&emsp;&emsp;``alarm``返回值：

&emsp;&emsp;这个函数的返回值是0或者以前 设定的闹钟时间还余下的描述。比如说，某个人要小睡一会，设定闹钟为30分钟之后响，20分钟之后被人吵醒了，还想多睡一会，于是重新设定闹钟为15分钟之后响，“以前设定闹钟时间还余下的时间就是10分钟”。如果``seconds``值为0，表示取消以前设定的闹钟，函数的返回值仍然是以前设定的闹钟还余下的秒数。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430101231.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430101302.png" style="zoom:80%;" />

&emsp;&emsp;测一测我们的云服务器的自增运算效率：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427205526.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427205550.png" style="zoom:80%;" />

&emsp;&emsp;如果我们捕捉信号后，再来看看这个计数能计多少次：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427210025.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427210142.png" style="zoom:80%;" />

&emsp;&emsp;发现多运行了好多倍，这是因为自增是纯CPU运算，而``printf``是IO操作，它要访问外设，是很慢的，它让我们的进程的自增操作少了好多次。

&emsp;&emsp;信号产生的方式的种类虽然特别多，但无论信号产生的方式千差万别，但是最终一定是OS向目标进程发送信号。

&emsp;&emsp;所以不同的信号产生方式一个相同点是都只有操作系统才能向进程发送信号。

## 5 如何理解操作系统向task_struct写入信号数据

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220427210528.png" style="zoom:80%;" />

&emsp;&emsp;（普通)信号的取值范围是``[1, 31]``，进程的``task_struct``内部一定要有对应的数据变量来保存记录，表明是否收到了对应的信号。

&emsp;&emsp;那么用什么样的数据变量来表示我是否收到信号了呢？我们选择一个无符号的32位int``uint32_t sigs``，它是一个位图结构：

``0000 0000 0000 0000 0000 0000``

&emsp;&emsp;我们这里认为最低位为第1个比特位，最高位为第32个比特位，比特位的第几个位置，代表的就是哪一个信号，比特位的内容就代表是否收到了信号。

``0000 0000 0000 0010 0101``:进程收到了1号信号，3号信号，6号信号。

&emsp;&emsp;所以进程的数据结构中使用位图来储存收到的信号信息。

&emsp;&emsp;所以操作系统向``task_struct``写入信号信息本质就是OS向进程的``task_struct``的位图对应比特位置1，完成信号的发送就是对``task_struct``信号位图的写入。

# 三、信号的保存状态

## 1 背景知识

&emsp;&emsp;实际执行信号的处理动作称为**信号抵达**：自定义捕捉、默认、忽略。

&emsp;&emsp;信号从产生到抵达之间的状态，称为**信号未决**，本质是这个信号被暂存在``task_struct``的信号位图中，还没被处理。

&emsp;&emsp;进程可以选择**阻塞(Block)**某个信号，本质就是**操作系统允许进程暂时屏蔽指定的信号**，它表明：该信号依旧是未决的；该信号不会被抵达直到解除阻塞方可抵达。

&emsp;&emsp;抵达.忽略 vs 阻塞的区别：

- 忽略是抵达的一种方式，而阻塞是没有被抵达，是一种独立状态。

&emsp;&emsp;信号的处理状态在内核中有三张表：``pending 表``、``block 表``、``handler 表``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430102314.png)

&emsp;&emsp;``pending``就是写入的那个位图，表示已经收到但是还没有被递达的信号。

&emsp;&emsp;``handler``是一个函数指针数组:``void(*handler[31])(int)``，其中``SIG_DFL``表示该信号的动作是默认处理动作，``SIG_IGN``表示该信号被忽略。

&emsp;&emsp;所以这张图是横着看，通过``pending``看看信号有没有到，``handler``则是对其的对应处理方法。

&emsp;&emsp;所以我们的信号捕捉就是把``handler``中写入自定义函数的地址。

&emsp;&emsp;我们查一下这个表里的两个宏，验证一下我们的说法：

```shell
grep -ER 'SIG_DFL|SIG_IGN' /usr/include/
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430102617.png" style="zoom:80%;" />

&emsp;&emsp;所以``handler 表``就是保证进程能够识别信号的结构，它是由写``os``的程序员实现的。

&emsp;&emsp;``block 表``，本质上也是一个位图结构，即也是一个``uint32_t block``，其含义为：比特位的位置代表信号的编号，比特位的内容代表信号是否被**阻塞(屏蔽)**。

&emsp;&emsp;所以``os``检测处理信号的伪代码如下：

```cpp
int isHandler(int signo)
{
    if (block & signo) {
        // 阻塞了 根本不管收没收到信号
    }
    else // 该信号没有被block
    {
        if (signo & pending)
            // 该信号已经收到了
        {
            handler_array[signo](signo);
            return 0;
        }
    }
    return 1;
}
```

&emsp;&emsp;所以Linux在进程中识别信号的三张表应该横着看：该信号是否被阻塞、该信号是否被收到、该信号对应的处理动作。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430103727.png)

1号信号没有被阻塞，也没有没收到，当前是1号信号的行为是默认行为；

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430103751.png)

2号信号已经被阻塞，现在又已经收到了，当前2号信号的行为是忽略；

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430103803.png)

3号信号已经被阻塞了，当前还没收到，3号信号的行为是自定义函数。

&emsp;&emsp;所以处不处理信号是block说了算，一旦阻塞，一定不处理，**block表**又称为**信号屏蔽字**。

&emsp;&emsp;这三张表结合起来，让进程做到了识别信号。

&emsp;&emsp;我们之前实验时，发现9号信号不能被捕捉，那么我们是否可以把所有信号都屏蔽呢？这样我们的进程不就为所欲为了呢？显然这样是不可以的，所以推测9号信号也无法被屏蔽。

## 2 相关系统调用接口

&emsp;&emsp;不是只有接口才是系统调用，OS也会给用户提供数据类型，配合系统调用来完成，比如``shmget``中的``key_t``、``struct ipc_perm``等，这些是配合接口使用的数据类型。

``sigset_t``:

&emsp;&emsp;从上面的信号保存状态图来看,每个信号只有一个bit的未决标志,非0即1,不记录该信号产生了多少次,阻塞标志也是这样表示的。 因此,未决和阻塞标志可以用相同的数据类型``sigset_t``来存储,``sigset_t``称为**信号集**,

&emsp;&emsp;这个类型可以表示每个信号 的“有效”或“无效”状态,在阻塞信号集中“有效”和“无效”的含义是该信号是否被阻塞,而在未决信号集中“有 效”和“无效”的含义是该信号是否处于未决状态。阻塞信号集也叫做当 前进程的信号屏蔽字(Signal Mask),这里的“屏蔽”应该理解为阻塞而不是忽略。

信息集操作函数：

```cpp
#include <signal.h>
int sigemptyset(sigset_t *set);// 把位图集合清空 全部置0
int sigfillset(sigset_t *set);// 全部置1
int sigaddset (sigset_t *set, int signo);// 把一个信号添加到这个位图里 也就是把这个信号对应的位图的位置1
int sigdelset(sigset_t *set, int signo);
int sigismember（const sigset_t *set, int signo);// 判定一个信号是否在集合中 
```

&emsp;&emsp;除``sigismember``外，这四个函数都是成功返回0,出错返回-1。``sigismember``是一个布尔函数,用于判断一个信号集的有效信号中是否包含 某种 信号,若包含则返回1,不包含则返回0,出错返回-1。

``sigprocmask``:修改进程的block位图。

```cpp
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oset); 
//返回值:若成功则为0,若出错则为-1 
```

&emsp;&emsp;如果``oset``是非空指针,则读取进程的当前信号屏蔽字通过``oset``参数传出。如果set是非空指针,则 更改进程的信 号屏蔽字,参数how指示如何更改。如果``oset``和``set``都是非空指针,则先将原来的信号 屏蔽字备份到``oset``里,然后 根据set和how参数更改信号屏蔽字。假设当前的信号屏蔽字为mask,下表说明了``how``参数的可选值。

| SIG_BLOCK   | set包含了我们希望添加到当前信号屏蔽字的信号，相当于mask = mask \| set |
| ----------- | ------------------------------------------------------------ |
| SIG_UNBLOCK | set包含了我们希望从当前信号屏蔽字中解除阻塞的信号，相当于mask = mask & ~set |
| SIG_SETMAEK | 设置当前信号屏蔽字为set所指的值，相当于mask = set            |

&emsp;&emsp;简单做个实验，屏蔽一下2号信号：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430111639.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430111604.png" style="zoom:80%;" />

&emsp;&emsp;接下来我们验证一下9号信号不可被屏蔽：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430111833.png" style="zoom:80%;" />

&emsp;&emsp;发现还是可以``kill -9``干掉，说明屏蔽9号信号是无效的。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430111911.png" style="zoom:80%;" />

&emsp;&emsp;9号信号被称为管理员信号，不能被屏蔽，不能被捕捉，必须永远遵守默认行为，也就是说不存在金刚不坏的进程。

``sigpending``:不是对``pending``位图做修改，而是获取进程当前的``pending``位图。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430112239.png" style="zoom:80%;" />

&emsp;&emsp;那么``pending``位图谁来改呢？显然程序员没必要去修改一个进程的``pending``位图，就只是一个储存接收到的信号的位图，我们可以通过``kill abort``等系统调用发送信号，显然不必去修改``pending``位图。

&emsp;&emsp;如果我的进程预先屏蔽掉2号信号，然后不断获取当前进程的``pending``位图，并打印显示，然后手动发送2号信号，因为2号信号被屏蔽了不会被抵达，因此打印时``pending``位图中2号信号对应的比特位一定有一个1，就会观察到这个``pending``位图中的1位由0置1的过程。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430113302.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430113541.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430113958.png" style="zoom:80%;" />

&emsp;&emsp;我们再增加一个计数器，20秒后取消对2号信号的屏蔽，具体做法就是把老的``block``位图``outset``传进去。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430114536.png" style="zoom:80%;" />

&emsp;&emsp;``vim``突然我们客户端突然断网退出后，它会形成一个临时文件，并且出现以下报警：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430114803.png" style="zoom:80%;" />

&emsp;&emsp;我们先按``r``，然后保存退出，然后把临时文件复制一下，利用rm把它删除，然后再打开就没有问题了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430115033.png" style="zoom:80%;" />

&emsp;&emsp;但是我们没看到2号信号的``pending``位图的位由1回0，这是因为2号信号的默认行为是终止进程，所以我们可以自定义捕捉一下2号进程：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430115321.png" style="zoom:80%;" />

&emsp;&emsp;而handler表的修改就是我们的``signal``函数就可以进行修改的，至此，我们完成了对``block``表、``pending``表、``handler``表的常见系统调用接口的使用。

# 四、信号处理的方式

## 1 信号处理的时机

&emsp;&emsp;信号发送后为什么是合适的时候进程才选择处理信号呢？这是因为信号的产生是异步的，当前进程的可能正在做更重要的事情，所以信号延时处理取决于操作系统和进程，那么什么是合适的时候呢？

&emsp;&emsp;那么信号什么时候被处理呢？

&emsp;&emsp;因为信号是被保存在进程的``task_struct``中，即在``pending``位图中，做处理（检测，递达（默认、忽略、自定义））。

&emsp;&emsp;**结论是当进程从内核态返回到用户态的时候，进行上面的检测与处理**。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430122335.png" style="zoom:80%;" />

&emsp;&emsp;用户身份是以进程为代表的，内核态和用户态的关系如下图：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430124153.png)

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430125304.png" style="zoom:80%;" />

&emsp;&emsp;为何一定要切换成为用户态才能执行自定义信号捕捉方法呢？

&emsp;&emsp;首先OS在理论上是可以直接执行用户的代码的，但是这样非常不安全，因为OS的权限非常高，如果用户代码有些危险的行为呢？所以OS不会去直接执行用户的代码。所以这就是为啥用户态只能执行用户的代码，内核态只能执行内核的代码的原因。

&emsp;&emsp;所以合适的时机就是由内核态切换到用户态的时候，做信号检测与信号处理。

## 2 sigaction—注册信号捕捉动作

```cpp
#include <signal.h>
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact); 
```

&emsp;&emsp;它修改的是``handler``表，它也可以处理实时信号，第二个参数是输入型参数，动作方法填入这个结构体中，``oact``是一个输出型参数，返回老的信号处理方法。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430130037.png" style="zoom:80%;" />

&emsp;&emsp;第二个、第四个、第五个是和实时信号有关的，我们不关心，一般第四个参数设为0，函数指针设为``NULL``，第一个参数就是那个自定义捕捉信号动作的函数指针。

&emsp;&emsp;我们来捕捉一下二号信号：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430130703.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430130759.png" style="zoom:80%;" />

&emsp;&emsp;当然也可以用宏``SIG_IGN``和``SIG_DFL``来表示忽略信号和默认信号处理方式，我们上面验证过了，它们都是函数指针：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430130958.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430131125.png" style="zoom:80%;" />

``sa_mask``的含义：**处理信号时希望暂时屏蔽其他信号，不让其他信号影响当前信号的处理**。

&emsp;&emsp;当某个信号的处理函数被调用时,内核自动将当前信号加入进程的信号屏蔽字,当信号处理函数返回时自动恢复原来 的信号屏蔽字,这样就保证了在处理某个信号时,如果这种信号再次产生,那么 它会被阻塞到当前处理结束为止。 如果 在调用信号处理函数时,除了当前信号被自动屏蔽之外,还希望自动屏蔽另外一些信号,则用``sa_mask``字段说明这些需 要额外屏蔽的信号,当信号处理函数返回时自动恢复原来的信号屏蔽字。

&emsp;&emsp;验证一下，这个``sa_mask``是一个``sigset_t``，所以我们用那些处理位图的接口来处理：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430132050.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430132108.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220430132215.png" style="zoom:80%;" />

&emsp;&emsp;普通信号记录的比特位只有一个，所以信号是可能被丢失的，在Linux中，实时信号就是不可以被丢失的，它们使用链表组织的，来一个链一个，不可被丢失。

# 五、可重入函数

&emsp;&emsp;我们目前写过的代码都是单执行流的代码，尽管学习fork创建子进程时，我们出现了多进程的执行，但是进程具有独立性，所以多进程的执行一般不会出一些问题，但是在下一节将会学习的多线程的知识中，多执行流的代码就会出现很多的问题。

&emsp;&emsp;比如下面的图片中，main执行流和信号捕捉执行流同时插入结点导致的结点地址丢失。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506130746.png)

&emsp;&emsp;这种现象我们就可以称为**insert**函数被重复进入了，此insert函数一旦被重入了，很有可能出现问题，这种函数被称为不可重入函数，反之被称为可冲入函数。

&emsp;&emsp;我们学到的大部分函数，``STL``、boost中的大部分函数，都是不可重入的。

如果一个函数符合以下条件之一则是不可重入的:

- 调用了``malloc``或``free``,因为``malloc``也是用全局链表来管理堆的。 
- 调用了标准I/O库函数。I/O库的很多实现都以不可重入的方式使用全局数据结构

# 六、volatile关键字

## 1 引入

&emsp;&emsp;volatile是一个C/C++中非常重要的关键字。

&emsp;&emsp;下面先看一组实验，我们的逻辑是一个死循环，当捕捉到2号信号时，自定义2号信号的行为，把死循环标记取反，从而使进程退出死循环正常退出。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506132015.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506131948.png" style="zoom:80%;" />

&emsp;&emsp;目前看来好像没什么问题。

&emsp;&emsp;改为gcc中的O3优化再编译运行呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506132323.png" style="zoom:80%;" />

&emsp;&emsp;发现编译器在优化后，对同一份代码却展现出了不同的行为，我们直接就不能退出死循环了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506132412.png" style="zoom:80%;" />

&emsp;&emsp;因为编译器的优化不能识别出多执行流的情况，原理就是编译器检测main函数时，发现这个flag在main执行流中一直是0，所以我就把flag直接优化为一个常量放到寄存器中，不需要自己再去内存中寻址读取。

&emsp;&emsp;怎么解决这个问题呢？加一个``volatile``关键字。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506132807.png" style="zoom:80%;" />

&emsp;&emsp;发现就解决问题了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506132840.png" style="zoom:80%;" />

&emsp;&emsp;那么为什么呢？``volatile``关键字是做什么的呢？

## 2 解释

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506133800.png" style="zoom:80%;" />

``volatile``在这里的作用之一：告诉编译器，不要对我这个变量做任何优化，读取必须是从内存中贯穿式的读取，不要读取中间缓冲区寄存器中的，简言之，**保持内存的可见性**。

``volatile``的另一个作用是防止指令重排。

# 七、SIGCHLD信号的简单了解

&emsp;&emsp;之前我们学习fork时，父进程需要等待子进程的退出以回收子进程的资源，前面我们学过，父进程阻塞式等待子进程退出。

&emsp;&emsp;进程一章讲过用``wait``和``waitpid``函数清理僵尸进程,父进程可以阻塞等待子进程结束,也可以非阻塞地查询是否有子进 程结束等待清理(也就是轮询的方式)。采用第一种方式,父进程阻塞了就不 能处理自己的工作了;采用第二种方式,父进程在处理自己的工作的同时还要记得时不时地轮询一 下,程序实现复杂。

&emsp;&emsp;子进程在退出时，会给父进程发送``SIGCHLD``信号，该信号的默认处理动作是忽略，父进程可以自定义``SIGCHLD``信号的处理动作，在信号处理动作中调用``wait``清理子进程的资源，这样父进程就可以专心关注自己的工作，而不必去做什么轮询和阻塞等待了。

&emsp;&emsp;事实上,由于UNIX 的历史原因,要想不产生僵尸进程还有另外一种办法:父进程调 用``sigaction``将``SIGCHLD``的处理动作 置为``SIG_IGN``,这样fork出来的子进程在终止时会自动清理掉,不 会产生僵尸进程,也不会通知父进程。系统默认的忽 略动作和用户用``sigaction``函数自定义的忽略 通常是没有区别的,但这是一个特例。此方法对于Linux可用,但不保证 在其它UNIX系统上都可用。

&emsp;&emsp;一个简单的实验验证子进程退出会向父进程发SIGCHLD信号：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506141107.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506141013.png" style="zoom:80%;" />

&emsp;&emsp;发现确实形成了僵尸状态。

&emsp;&emsp;验证一下17号信号设置为``SIG_IGN``后可以直接让子进程释放资源，而不形成僵尸：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506141331.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506141559.png" style="zoom:80%;" />

&emsp;&emsp;所以不关心子进程退出信息而直接回收子进程的僵尸进程的方式：``signal(SIG_CHLD, SIG_IGN)``。

&emsp;&emsp;一种注册信号处理多个子进程退出的方法：

```cpp
void handler(int sig)
{
    pid_t id;
    // 处理多个子进程 并且WNOHANG防止有子进程
    while( (id = waitpid(-1, NULL, WNOHANG)) > 0){
        printf("wait child success: %d\n", id);
    }
    printf("child is quit! %d\n", getpid());
}
```

