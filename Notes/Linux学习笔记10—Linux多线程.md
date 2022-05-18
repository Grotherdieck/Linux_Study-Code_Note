# 一、Linux线程概念

## 1 线程是什么

&emsp;&emsp;在传统教材中，线程是在进程内部运行的一个执行分支（执行流），属于进程的一部分，粒度比进程更加轻量化，与进程的传统概念类似，问号太多了。

&emsp;&emsp;根据传统概念，一个进程内存在多个线程吗？可能存在；进程和线程的比率就会是``1：n``，所以在OS内部就可能存在更多的线程，所以OS要不要管理线程呢，当然是要的，OS所以要以先描述再组织的方式管理线程，**线程也应该要有它的控制块—线程控制块TCB**。

&emsp;&emsp;以上是常规操作系统的实现方式，如``windows``的实现,Linux中不是这样，Linux的实现如下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506145014.png" style="zoom:80%;" />

&emsp;&emsp;所以在Linux中，所谓线程在进程内部运行就是线程和进程共享了同一个地址空，线程在进程的地址空间内部。

&emsp;&emsp;CPU调度时，只看PCB，每一个PCB都已经被去指派过了它要执行的方法和数据，这就是线程是一个执行流的概念。

&emsp;&emsp;所以现在我们可以认为，我们之前学习的进程是内部只包含一个执行流（PCB）的进程，而在我们有了线程概念后，我们的进程内部可以有多个执行流（PCB）。

&emsp;&emsp;创建线程只要创建PCB，然后把地址空间数据和代码分配给它就行了，而创建进程你不进要创建PCB，还要创建地址空间、页表等一系列我们前面学习过的和进程有关的内核数据结构的创建，所以**创建进程的成本非常高（时间+空间）**。

&emsp;&emsp;内核视角：进程是承担分配系统资源的基本实体，创建地址空间、PCB、页表等等有关的进程内核数据结构，这就是在分配系统资源。

&emsp;&emsp;内核视角：线程是CPU调度的基本单位，承担进程资源一部分的基本实体，即进程划分资源给线程。

## 2 Linux线程接口和Linux线程的关系

&emsp;&emsp;Linux中的PCB<=(概念强度小于)传统意义上的进程.

- OS创建线程时非常轻量化；
- CPU调度时比较轻松。

&emsp;&emsp;所以Linux下的进程我们一般认为是轻量级的进程。

&emsp;&emsp;Linux因为是用进程模拟的线程的，所以Linux下不会给我们提供直接操作线程的接口，而是给我们在同一个地址空间中创建PCB、分配资源给指定的PCB的接口。

&emsp;&emsp;所以我们要自己去用这一套管理线程，对用户特别不友好，这就是Linux下的线程的解决方案的缺点，``windows``下却可以直接用系统调用来解决这些问题。

&emsp;&emsp;用户需要的仅仅是创建线程的接口、释放线程的接口、等待线程的接口。。。

&emsp;&emsp;幸好有很多系统工程师，对Linux的轻量级进程接口进行了封装，为我们封装成了库，让用户直接使用库接口，这被称为原生线程库（用户层），这也就是我们要重点学习掌握的东西，我们这里用的是``pthread``库。

&emsp;&emsp;Linux下有``vfork``系统调用，创建一个线程共享地址空间，仅做了解。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506151743.png" style="zoom:80%;" />

&emsp;&emsp;同一个进程的所有的线程的PCB都是共享的一个地址空间，理论上所有的线程都可以同时看到进程的所有资源。

&emsp;&emsp;进程具有独立性，可以有部分共享资源（管道、``IPC``资源），但对线程来说，大部分资源是共享的，也可以有部分资源“私有”（PCB,**栈结构、上下文**）。

## 3 线程私有的资源	

- 线程ID 
- 一组寄存器
- 栈 
- errno 
- 信号屏蔽字 
- 调度优先级

## 4 线程公有的资源

&emsp;&emsp;进程的多个线程共享 同一地址空间,因此Text Segment、Data Segment都是共享的,如果定义一个函数,在各线程 中都可以调用,如果定义一个全局变量,在各线程中都可以访问到,除此之外,各线程还共享以下进程资源和环境:

- 文件描述符表 
- 每种信号的处理方式(SIG_ IGN、SIG_ DFL或者自定义的信号处理函数) 
- 当前工作目录 
- 用户id和组id

## 5 第一个线程demo

``pthread_create``：创建线程的接口。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506153204.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数是线程id，第二个参数是线程属性（一般设为NULL），第三个参数是你这个线程要执行哪个函数（是一个类似C语言中的回调函数），最后一个参数是给这个函数传入的参数。

&emsp;&emsp;一个简单的两个线程都是死循环的的程序：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506154009.png" style="zoom:80%;" />

```shell
gcc -o mythread mythread.c -lpthread
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506154321.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506154354.png" style="zoom:80%;" />

&emsp;&emsp;两个``pid``都相同，说明此时依旧只有一个进程，我们可以查看一下。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506154554.png)

&emsp;&emsp;我们``kill``掉以后，直接把两个线程都退出了。

&emsp;&emsp;那么我们怎么区分这两个执行流呢？利用命令：

```shell
ps -aL// 查看轻量级的进程
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506154803.png" style="zoom:80%;" />

L(light、)，W(weight)，P(Process)，轻量级进程的编号。

&emsp;&emsp;操作系统调度时，看的肯定是``LWP``.

&emsp;&emsp;如何理解我们之前单独一个进程的情况？此时``PID``和``LWP``相等，这里``PID == LWP``的线程就是主线程。

&emsp;&emsp;一堆属于同一个进程的线程被称为一个线程组，其组id即``PID``，也称为组``id``。

## 6 线程的优点

- 创建一个新线程的代价要比创建一个新进程小得多 
- 与进程之间的切换相比，线程之间的切换需要操作系统做的工作要少很多 
- 线程占用的资源要比进程少很多 
- 能充分利用多处理器的可并行数量 （多进程也可以做到）
- 在等待慢速I/O操作结束的同时，程序可执行其他的计算任务 （多进程也可以做到）
- 计算密集型应用，为了能在多处理器系统上运行，将计算分解到多个线程中实现 
- I/O密集型应用，为了提高性能，将I/O操作（等待时间）重叠。线程可以同时等待不同的I/O操作。

&emsp;&emsp;计算密集型应用，如加密和大数据运算等，其主要使用的是CPU资源；

&emsp;&emsp;IO密集型应用，如网络下载、云盘、ssh、在线直播、看电影等，主要使用的是内存和外设等IO资源。

&emsp;&emsp;也有很多应用是计算密集型+IO密集型的应用，比如一些网络游戏等，但是一般互联网公司内部都是把这两种类型给分开的，这样能够提高整群的效率。

&emsp;&emsp;那么在计算密集型应用中，线程越多越好吗？不一定，多线程需要保证很多线程都能获得CPU资源，如果你是单核的CPU，那么你多少线程都无济于事，所以线程个数最好不要超过CPU个数，因为你的调度台就那么多，如果线程太多，会导致线程间被过度调度切换，这件事的成本也不小。

&emsp;&emsp;所以并不是多线程一定就能提高效率的，要看具体的场景。

&emsp;&emsp;对于IO密集型应用，线程是不是越多越好呢？不一定，不过，IO型允许多一些线程。这个时候并不是就不考虑线程切换的速度了，因为IO的场景下，大部分时间都在等待，但是也不能太多了。

## 7 线程的缺点

- **性能损失** 一个很少被外部事件阻塞的计算密集型线程往往无法与共它线程共享同一个处理器。如果计算密集型 线程的数量比可用的处理器多，那么可能会有较大的性能损失，这里的性能损失指的是增加了额外的 同步和调度开销，而可用的资源不变。 
- **健壮性降低** 编写多线程需要更全面更深入的考虑，在一个多线程程序里，因时间分配上的细微偏差或者因共享了 不该共享的变量而造成不良影响的可能性是很大的，换句话说线程之间是缺乏保护的，一个线程崩溃很有可能整个进程都崩溃了。
- **缺乏访问控制** 进程是访问控制的基本粒度，在一个线程中调用某些OS函数会对整个进程造成影响。 
- **编程难度提高** 编写与**调试**一个多线程程序比单线程程序困难得多

## 8 线程的异常

&emsp;&emsp;一个线程的崩溃会导致整个进程退出，因为信号是发给进程的。

## 9 线程用途

- 合理的使用多线程，能提高CPU密集型程序的执行效率 
- 合理的使用多线程，能提高IO密集型程序的用户体验（如生活中我们一边写代码一边下载开发工具，就是 多线程运行的一种表现），比如边下边看电影。

## 10 进程与线程的4种关系

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506193732.png" style="zoom:80%;" />

# 二、Linux线程控制

## 1 pthread_create-创建线程

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506194151.png" style="zoom:80%;" />

``pthread_t``是一个长整型，无符号的（不同的操作系统可能不同），是一个输出型参数，当创建成功时，会把线程id返回。

第二个参数是线程的属性，如优先级等等，但是我们米有OS懂，所以就一般设为NULL。

第三个参数是线程需要执行函数，第四个参数是传给这个函数的参数。

``pthread_self``函数，获得线程id，下面一个函数，让主线程打印新线程的id，新线程利用``pthread_self``打印自己的线程id：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506194742.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506194831.png" style="zoom:80%;" />

&emsp;&emsp;使用``ps -aL``看到的``LWP``怎么和这个新线程的id(``pthread_t``)怎么不一样呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506194901.png" style="zoom:80%;" />

&emsp;&emsp;我们再看看主线程自己的id是啥？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506195134.png" style="zoom:80%;" />

&emsp;&emsp;看到线程id确实不同。

&emsp;&emsp;利用for循环创建一批线程，发现打印出来乱的很：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506195918.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506195706.png" style="zoom:80%;" />

&emsp;&emsp;不让子线程打印，看起来就正常多了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506195828.png" style="zoom:80%;" />

## 2 一个线程崩溃整个进程崩溃的验证

&emsp;&emsp;我们创建五个线程，让3号线程进行野指针访问：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506200620.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506200634.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506200602.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506200550.png" style="zoom:80%;" />

## 3 pthread_join—线程等待

&emsp;&emsp;和进程类似，如果不等待线程退出，也会出现类似僵尸进程的问题。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506200825.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数就是要等待的``tid``；

&emsp;&emsp;进程执行玩的三种结果：代码跑完结果对、代码跑完结果不对、代码异常退出；

&emsp;&emsp;第二个参数``void** retval``是一个输出型参数，用来获得线程退出时函数的返回值（因为你线程函数返回值为``void*``，要拿这个结果必须用``void**``。

&emsp;&emsp;线程等待实验的mode：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506201856.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506201835.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506201928.png" style="zoom:80%;" />

&emsp;&emsp;两个问题，一，``pthread_join``可以获得退出信息，那异常退出呢（即信号呢）？

&emsp;&emsp;答案是异常退出并不应该线程来处理，而是进程处理的任务，因为线程崩溃了主线程也崩溃了，需要由这个多线程程序的父进程去获得退出信号。

&emsp;&emsp;线程在返回时难道只能返回整数吗，``void*``类型指针可以带回任何类型的对象的地址，线程的返回值完全也可以是其他变量或对象的地址，但是要保证这个对象**不是临时的**，也就是说，我们可以让线程去申请资源！

&emsp;&emsp;如果多个线程，则要循环等待。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506202623.png" style="zoom:80%;" />

## 4 线程终止的方案

1. 函数中return；main函数退出，return时代表主线程以及进程退出，其他线程return只代表当前线程退出
2. 函数``pthread_exit(void* retval)``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506204555.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506204658.png" style="zoom:80%;" />

线程监控脚本

```shell
while :; do ps -aL; sleep 1; echo "#######################"; done
```



<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506205006.png" style="zoom:80%;" />

&emsp;&emsp;如果用``exit``，那么会终止整个进程。

3. ``pthread_cancel``:根据线程``id``取消线程

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506205220.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506205353.png" style="zoom:80%;" />

&emsp;&emsp;发现取消后的线程，其退出码是-1.

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506205421.png" style="zoom:80%;" />

&emsp;&emsp;那么新线程能否取消主线程呢，实验一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506205704.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506210052.png" style="zoom:80%;" />

&emsp;&emsp;实验表明这样操作是完全可以的，此时主线程就变成了一种僵尸状态，但是最好不要这样操作，因为这样操作的意义不明。

&emsp;&emsp;``-1``退出码对应的线程退出状态（被``cancel``）：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220507230950.png" style="zoom:80%;" />

&emsp;&emsp;``(void*)-1``就是``PTHREAD_CANCELED``,表明线程被取消而退出的宏。

## 5 pthread_detach—线程分离

&emsp;&emsp;线程分离的目的是不想等待线程退出，不想管线程的退出信息，线程分离可以让我们线程不需要被``pthread_join``，运行完毕后，自动释放线程资源，就类比``signal(SIG_CHLD, SIG_IGN)``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506210837.png" style="zoom:80%;" />

&emsp;&emsp;我们让我们创建的线程与主线程分离，观察一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506211146.png" style="zoom:80%;" />

&emsp;&emsp;顺便看看这时线程如果我们在join会怎么样：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506211203.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506211401.png" style="zoom:80%;" />

&emsp;&emsp;发现这时线程退出码不是0了，说明这样再join属于是异常退出，因此如果线程分离了，就不要再``pthread_join``了。

# 三、Linux下用户级线程概念理解

&emsp;&emsp;回到我们的问题：为啥``pthread_t tid``的值和``os``的``LWP``的值不同的？我们为什么觉得他们要一样呢？

&emsp;&emsp;但其实我们觉得他们要一样是不合理的，因为**我们的线程是从用户级的pthread线程库创建的**，而``LWP``是一个唯一标识一个调度单位的标识，是内核级别的东西，它俩本就不应该相同。

&emsp;&emsp;那么我们知道``LWP``是在``task_struct``中唯一标识它的一个东西，那``pthread_t tid``究竟是啥呢？

&emsp;&emsp;我们先把``thread_t``这个东西以16进制打印出来看看：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506211833.png" style="zoom:80%;" />

&emsp;&emsp;打印两个线程的``thread_t``:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506212208.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220506212304.png" style="zoom:80%;" />

&emsp;&emsp;这里的线程id很像一种**地址**，结论就是，我们查看的线程``id``是``pthread``库中的线程id，不是Linux内核中的``LWP``,``pthread_t``它是一个**虚拟地址**。

&emsp;&emsp;``pthread``线程库依赖的是一个动态库，它是一个文件，它本来在磁盘中，它要被进程访问，必须加载到物理内存中，然后每个线程通过的页表来映射来找到它，这种动态库、共享内存等多个进程都可能要用的东西，它映射到的区域被称为共享区（``mmap``区域）,这样可以让不同的进程都通过同样的虚拟地址找到这个线程库。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220507233639.png" style="zoom:80%;" />

&emsp;&emsp;原理如下图：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220508000516.png)

&emsp;&emsp;而每个线程都要有运行时的临时数据，所以每个线程都有自己私有的栈结构。

&emsp;&emsp;所以我们要拿到描述线程的用户级控制块，并且也要拿到每个线程的栈结构，但是我们没办法从OS哪里获得，因为对Linux来说，没有线程的概念，所以我们本质要到库设计者把线程这些东西放在的具体地方来找。

&emsp;&emsp;而一个线程的这些东西放在某个库区域的结构体中，要在多个线程中找到某个线程的位置，当然需要``struct pthread*``，即它的地址，拿到它的地址，就能找到它的线程栈、它的各种线程有关数据的存储区域。

&emsp;&emsp;所以``pthread_t``就是库中某个``struct pthread``的地址。

&emsp;&emsp;而PCB有一个``LWP``，它也可以唯一的表示一个PCB，所以这个``PCB``和``pthead_t``需要``1:1``对应，即用户的线程和内核中的轻量级进程``task_struct``需要``1:1``对应。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220508000916.png" style="zoom:80%;" />

&emsp;&emsp;而传统概念上来说，线程和进程应该是``n:1``的，比如``windows``的操作系统，但是在Linux中不是这样，这就是Linux中特殊的地方。

&emsp;&emsp;线程崩溃的影响一定是有限的``->``因为线程归进程管``->``进程具有独立性，一般情况下不会互相影响。

# 四、Linux线程互斥

## 1 相关背景概念

&emsp;&emsp;因为多个线程是共享地址空间的，也就是很多资源都是共享的，优点：通信方便，缺点：缺乏访问控制（如我正在操作这个堆空间，别人给我释放了等等问题）。

&emsp;&emsp;会出现这种因为一个线程的操作问题给其他线程造成了不可控或者引起崩溃、异常、逻辑不正确等，这种现象称为线程安全问题。

&emsp;&emsp;在多线程场景下，函数被重入是很正常的，比如我们刚刚一个for循环创建一系列线程，他们都去走``thread_run``函数。

&emsp;&emsp;因此为防止线程安全问题，我们尽量不要用全局变量、``stl、malloc、new``等会在全局有效的数据（访问控制）;

&emsp;&emsp;尽量使用局部变量，因为每个线程都有自己的独立栈结构。

&emsp;&emsp;但是很多时候我们必须要使用这些东西，不然编程的成本太高了，所以需要进行后续的访问控制：互斥、同步的原因就是为了解决我们就要使用这些全局有效的数据的安全问题。

&emsp;&emsp;在进程间通信中，我们讲过能同时被两个进程共享访问的资源——临界资源，在多线程这里，能够被多个线程同时访问的资源就是临界资源。

&emsp;&emsp;我们之前多线程打印时，显示器这时也是一种临界资源，因为多个执行流这时候都能访问到显示器，如果这时不加保护，必然会胡乱打印。

&emsp;&emsp;临界区：并不是所有的代码都会去访问临界资源，代码中访问临界资源的代码区域我们称为临界区。

&emsp;&emsp;我们要完成对临界区保护的功能，本质就是对临界资源的保护，方式是互斥或同步。

&emsp;&emsp;**互斥**：在任意一个时刻，只允许一个执行流访问某段代码（访问部分资源），就可以称为互斥。

&emsp;&emsp;比如：``printf("hello world")``->``lock();``执行完毕->``unlock()``，这就是一种**原子性行为**。要么不执行，要么执行完毕。

&emsp;&emsp;**同步**：一般而言，让访问临界资源的过程在安全的前提下(一般都是互斥且原子的)，让访问资源具有一定的顺序性，比如访问完了就得回队尾访问。

&emsp;&emsp;加入同步性后，访问临界资源就会更有**合理性**。

## 2 抢票mode

```cpp
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>

using namespace std;
// 抢票逻辑 10000票 同时抢
// tickets 就是临界资源:tickets--是原子的吗(也就是说对tickets--是安全的吗)
// 为了让多个线程进行切换，线程什么时候会切换呢？(1.时间片到了;2.检测的时间点:从内核态返回用户态的时候)
int tickets = 1000;
void *ThreadRun(void *args)
{
    string tname = (char *)args;
    int id = *(int*)args;
    delete (int*)args;
    while (true)
    {
        if (tickets > 0)
        {
            usleep(10000);
            cout << "我是[" << id << "] 我要抢的票是:" << tickets << endl;
            printf("");
            tickets--;
        }
        else break;
    }
    return nullptr;
}

int main()
{
    pthread_t tid[5];
    for (int i = 0; i < 5; ++i)
    {
        int* pid = new int(i);
        pthread_create(tid + i, nullptr, ThreadRun, (void*)pid);
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tid[i], nullptr);
    }
    return 0;
}
```

&emsp;&emsp;5个线程抢票，没有对票数（临界资源）加锁，居然会抢出负数来：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220510200706.png" style="zoom:80%;" />

&emsp;&emsp;所以此时这个``tickets--``它是不安全的。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220510201633.png" style="zoom:80%;" />

&emsp;&emsp;这时候我们就需要互斥锁了——mutex互斥锁。

## 3 mutex互斥锁的使用

&emsp;&emsp;``pthread_mutex_``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220510202002.png" style="zoom:80%;" />

&emsp;&emsp;加锁就是``pthread_mutex_lock(锁的地址)``，解锁就是``pthread_mutex_unlock(锁的地址)``.

&emsp;&emsp;我们用C++的类的方式来创建一个类，包含票数``tickets``和一个mutex互斥锁对象，构造函数初始化锁，析构函数销毁锁，进入抢票函数加锁，离开抢票函数解锁，如下：

```cpp
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <mutex>// C++11语言支持的多线程的锁

using namespace std;
// 抢票逻辑 10000票 同时抢
// tickets 就是临界资源:tickets--是原子的吗(也就是说对tickets--是安全的吗)
// 为了让多个线程进行切换，线程什么时候会切换呢？(1.时间片到了;2.检测的时间点:从内核态返回用户态的时候)
int tickets = 1000;

class Ticket
{
public:
    // 构造函数 初始化一个锁
    Ticket() : tickets(1000)
    {
        pthread_mutex_init(&mxt, nullptr);
    }

    // 析构函数 销毁这个锁
    ~Ticket()
    {
        pthread_mutex_destroy(&mxt);
    }

    // 抢票 抢票过程加锁解锁
    bool GetTicket()
    {
        // 加锁
        pthread_mutex_lock(&mxt);
        bool res = true;
        if (tickets > 0)
        {
            usleep(10000);
            cout << "我是[" << pthread_self() << "] 我要抢的票是:" << tickets << endl;
            printf("");
            tickets--;
        }
        else 
        {
            printf("已经没有票可以抢了\n");
            res = false;
        }
        // 解锁
        pthread_mutex_unlock(&mxt);
        return res;
    }
private:
    int tickets;
    pthread_mutex_t mxt;
};

void *ThreadRun(void *args)
{
    Ticket* t = (Ticket*)args;
    while (true)
    {
        if (!t->GetTicket()) break;
    }
    return nullptr;
}

int main()
{
    Ticket *t = new Ticket();
    pthread_t tid[5];
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(tid + i, nullptr, ThreadRun, (void *)t);
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tid[i], nullptr);
    }
    delete t;
    return 0;
}
```

&emsp;&emsp;然后加上C++的线程库的``<mutex>``的一个类``mutex``来替换原生线程库的加锁与解锁，

```cpp
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <mutex>// C++11语言支持的多线程的锁

using namespace std;
// 抢票逻辑 10000票 同时抢
// tickets 就是临界资源:tickets--是原子的吗(也就是说对tickets--是安全的吗)
// 为了让多个线程进行切换，线程什么时候会切换呢？(1.时间片到了;2.检测的时间点:从内核态返回用户态的时候)
// int tickets = 1000;

class Ticket
{
public:
    // 构造函数 初始化一个锁
    Ticket() : tickets(1000)
    {
        // pthread_mutex_init(&mxt, nullptr);
    }

    // 析构函数 销毁这个锁
    ~Ticket()
    {
        //pthread_mutex_destroy(&mxt);
    }

    // 抢票 抢票过程加锁解锁
    bool GetTicket()
    {
        // 加锁
        //pthread_mutex_lock(&mxt);
        mymtx.lock();
        bool res = true;
        if (tickets > 0)
        {
            usleep(10000);
            cout << "我是[" << pthread_self() << "] 我要抢的票是:" << tickets << endl;
            printf("");
            tickets--;
        }
        else 
        {
            printf("已经没有票可以抢了\n");
            res = false;
        }
        // 解锁
        // pthread_mutex_unlock(&mxt);
        mymtx.unlock();
        return res;
    }
private:
    int tickets;
    //pthread_mutex_t mxt;
    mutex mymtx;
};

void *ThreadRun(void *args)
{
    Ticket* t = (Ticket*)args;
    int cnt = 0;
    while (true)
    {
        usleep(10000);
        if (t->GetTicket()) 
        {
            ++cnt;
        }
        else break;
    }
    cout << "我是" << pthread_self() << "线程," << "我抢了" << cnt << "张票" << endl;
    return nullptr;
}

int main()
{
    Ticket *t = new Ticket();
    pthread_t tid[5];
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(tid + i, nullptr, ThreadRun, (void *)t);
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tid[i], nullptr);
    }
    delete t;
    return 0;
}
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220510204033.png" style="zoom:80%;" />

&emsp;&emsp;一切正常了。

## 4 为什么锁的加锁和解锁要是原子性的

&emsp;&emsp;锁本身也可以被所有线程看到，它也是临界资源，那它怎么保护自己的呢？总不能出现我在加锁的过程中另一个线程也来加锁把，**原理就是它加锁和解锁的过程都是原子性的。**

## 5 mutex的原理

&emsp;&emsp;假设``int lock = 1;``，申请锁就是``lock--``，开锁就是``lock++``。

&emsp;&emsp;那么伪代码可以写为：

```cpp
//申请锁
if (lock > 0)
{
    --lock;
}

//释放锁
lock++;
```

&emsp;&emsp;但是我们前面说了，``lock--``不是原子性的，所以这样的方案是不可以的。

&emsp;&emsp;为了实现互斥锁的原理，大多数体系结构都提供了``swap``或``exchange``指令,该指令的作用是把寄存器和内存单 元的数据相交换,由于只有一条指令,保证了原子性。

&emsp;&emsp;当CPU执行线程A的代码时，CPU内寄存器的数据，就是线程A的上下文，是A的私有数据。我们就利用这一点，完成了互斥锁。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220510221944.png" style="zoom:80%;" />

&emsp;&emsp;``mutex``的本质其实是用一行汇编代码把锁数据交换到了自己上下文中，竞争能力强的线程就把锁的数据交换到自己的上下文中，其他的线程被挂起等待。

&emsp;&emsp;它的原理就是首先，锁是一个内存中的数据``mutex``，假设一开始的数据是1，假设有一个线程A先申请到了锁，它的操作是先把0数据放到寄存器``%al``中，然后把``mutex``中的1交换给这个寄存器，只要这条汇编指令执行完，**然后相当于锁已经被A存到自己的上下文去了**，不管怎么切，其他人过来得到这个数据都是0，没法进入邻接区域，只好挂起等待，等到A线程把锁的数据还回去，才有可能竞争锁成功，才能执行临界区的代码。

## 6 静态区的锁

&emsp;&emsp;如果要在静态变量区创建一把锁，我们可以直接用宏``pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER``来创建一把锁，然后不用去调用``pthread_mutex_init()``函数和``pthread_mutex_destroy()``函数了，即锁的生命周期不需要我们管理了。

## 7 线程安全和可重入的关系

&emsp;&emsp;可重入是函数的概念，如果一个函数是不可重入的，那么多个线程重入这个函数时可能会引发线程安全问题。

&emsp;&emsp;可重入函数是线程安全函数的一种，线程安全函数不一定是可重入的。

&emsp;&emsp;如main函数要向链表中插入结点，在``insert``时先申请锁，然后再访问临界资源时，来了信号，去执行信号函数去了，然后信号函数里头又要去执行``insert``函数，又要去申请锁，可是这时候锁在你手里啊，因此申请不到锁，我把你挂起了，你等待着别人把锁还了，然后就会永久挂起了。

&emsp;&emsp;假如锁申请两次就会发生这种情况，这种现象叫做**死锁**.

## 8 死锁

&emsp;&emsp;死锁是指在一组进程中的各个进程均占有不会释放的资源，但因互相申请被其他进程所站用不会释放的资 源而处于的一种永久等待状态。

**死锁产生的四个必要条件**:(如果出现 死锁，则这四个条件必然存在)

- 互斥条件：一个资源每次只能被一个执行流使用 ，这是互斥锁的基本特点，不可能破坏
- 请求与保持条件：一个执行流因请求资源而阻塞时，对已获得的资源保持不放 ，即拿着锁还要申请锁。
- 不剥夺条件:一个执行流已获得的资源，在末使用完之前，不能强行剥夺 ，破坏此条件则允许一个线程抢夺另一个线程。
- 循环等待条件:若干执行流之间形成一种头尾相接的循环等待资源的关系

## 9 破坏死锁的方法

- 破坏死锁的四个必要条件 
- 加锁顺序一致，每个线程都按同样的顺序申请所，归还锁。
- 避免锁未释放的场景 (避免不开锁就去申请锁)
- 资源一次性分配

&emsp;&emsp;总之锁这个东西要慎用，如果过多应用锁那么很有可能导致效率降低过多。

## 10 避免死锁的算法

- 死锁检测算法 
- 银行家算法

# 五、Linux线程同步

&emsp;&emsp;如果多线程访问临界资源没有顺序限制，那么假如刚开锁的线程同时又申请锁，他又去单独访问临界资源了，其他线程一直访问不到这个临界资源，就会造成饥饿的问题。

&emsp;&emsp;我们假设有一个规则，让刚访问完资源的线程去排到队尾，到其他线程搞完了才能再次轮到它搞。

&emsp;&emsp;线程同步是一种多线程协作的概念，这里我们引出条件变量的概念。

## 1 条件变量的认识与使用

&emsp;&emsp;当一个线程互斥地访问某个变量时，它可能发现在其他线程改变状态之前，它什么都做不了。

&emsp;&emsp;例如一个线程访问队列，发现队列为空，它只能等待，等待其他线程将一个结点加入队列中，它才能执行它的工作。

&emsp;&emsp;对于这种场景，如果是只有锁的情况，其实很难以知道临界资源的状态，因此我们就需要引入条件变量，它就是一方通知另一方已就绪的东西。

&emsp;&emsp;pthread原生线程库中，就有条件变量：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511192108.png" style="zoom:80%;" />

&emsp;&emsp;可以看到与互斥锁的接口非常像，同样的也有全局静态的条件变量，不需要我们手动销毁。

&emsp;&emsp;看一个函数``pthread_cond_t_wait()``，表明当前线程等待这个条件变量。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511192236.png" style="zoom:80%;" />

&emsp;&emsp;``pthread_cond_t_signal``唤醒在该条件变量下等待的线程：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511192446.png" style="zoom:80%;" />

&emsp;&emsp;一个简易的``mode``，让一个线程控制5个线程，让5个线程等待这个线程发送指令再打印东西：

```cpp
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

pthread_mutex_t mtx;
pthread_cond_t cdt;

const int N = 5;
// ctrl thread控制work线程 让它定期运行
void* ctrl(void* args)
{
    string name = (char*)args;
    while (true)
    {
        // 定期下发任务 唤醒在该条件变量下等待的一个线程
        // 那么唤醒的是哪一个呢?
        pthread_cond_signal(&cdt);
        sleep(1);
    }
}


void* work(void* args)
{
    int number = *(int*)args;
    delete (int*)args;
    while (true)
    {
        // 等待ctrl线程的号令
        // 现在我们的互斥锁仅仅只为了满足参数 暂时还没使用
        pthread_cond_wait(&cdt, &mtx);
        cout << "worker:" << number << "is working..." << endl;
    }
}

int main()
{
    pthread_t worker[N];// 5个工作线程
    pthread_t master;
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cdt, nullptr);
    pthread_create(&master, nullptr, ctrl, (void*)"master");
    for (int i = 0; i < N; ++i)
    {
        int* number = new int(i);
        pthread_create(&worker[i], nullptr, work, (void*)number);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_join(worker[i], nullptr);
    }
    pthread_join(master, nullptr);

    // 释放互斥锁
    pthread_cond_destroy(&cdt);
    pthread_mutex_destroy(&mtx);
    return 0;
}
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511194459.png" style="zoom:80%;" />

&emsp;&emsp;发现好像很有顺序：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511194622.png" style="zoom:80%;" />

&emsp;&emsp;这里1号线程打印完这句话，它又去等了，然后2打印完，它又去等了，1打印完了，它又去等了，而且顺序是一直不变的，说明条件变量内部一定存在一个**等待队列**。

```cpp
// 条件变量的伪结构
struct cond
{
    int status;// 表明条件是否完成
    task_struct* q;// 队列
};
```

&emsp;&emsp;而``pthread_cond_signal``唤醒的是一个在等待队列中等待的第一个线程，``pthread_cond_broadcast``，``broadcast``的含义是广播，它的作用是唤醒等待队列中的所有线程。

&emsp;&emsp;果然一瞬间全部工作起来了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511195153.png" style="zoom:80%;" />

## 2 初识生产者消费者模型

&emsp;&emsp;从函数调用理解它。

&emsp;&emsp;初步理解如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511195902.png" style="zoom:80%;" />

&emsp;&emsp;生活中也有生产者消费者模型，典型的就是超市，我们以超市作为中间层再认识一下生产者消费者模型及其优势，如下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511202509.png" style="zoom:80%;" />

## 3 基于阻塞队列的生产者消费者模型

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220511204258.png" style="zoom:80%;" />

&emsp;&emsp;设计思路：

&emsp;&emsp;我们设计一个阻塞队列模板类，其中内包含数据成员：``std:queue<T>``(用来实现队列功能)、``pthread_mutex_t``(用来在生产和消费的过程中，也就是访问这个队列的过程中上锁)、``int cap``（阻塞队列大小上限)、``pthread_cond_t is_full_``(用来通知消费者可以来队列取元素了的信号量)、``pthread_cond_t is_empty_``（用来通知生产者可以向队列中生产了）。

&emsp;&emsp;只有生产者知道消费者什么时候应该消费，只有消费者知道生产者应该什么时候来生产，所以让生产者(消费者)从等待状态唤醒的任务是消费者(生产者)的任务。

```cpp
#pragma once
#include <iostream>
#include <queue>
using namespace std;

const int defaultcap = 5;

namespace ns_blockqueue
{
    template <class T>
    class BlockQueue
    {
    public:
        BlockQueue(int cap = defaultcap) :cap_(cap)
        {
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&is_full_, nullptr);
            pthread_cond_init(&is_empty_, nullptr);
        }
        ~BlockQueue() 
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&is_full_);
            pthread_cond_destroy(&is_empty_);
        }
        bool isFull()
        {
            return bq_.size() == cap_;
        }

        bool isEmpty()
        {
            return bq_.size() == 0;
        }

        // const& 输入
        // &输入输出型参数
        // *输出型参数
        void push(const T& in)
        {
            // 先队列中放数据 生产函数
            // 还有bug?
            LockQueue();
            if (isFull()) 
            {
                ProducterWait();
                // pthead_cond_wait返回时自动再次竞争锁 竞争到了才回来
                // 所以此时是有锁状态
            }
            bq_.push(in);

            // 生产者通知消费者可以消费了
            // 策略:如果队列中的元素大于2个 唤醒消费者
            if (bq_.size() > 2) WakeupConsumer();
            UnlockQueue();
        }

        void pop(T* out)
        {  
            LockQueue();
            // 从队列中拿数据 消费函数
            if (isEmpty())
            {
                ConsumerWait();
            }
            *out = bq_.front();
            bq_.pop();

            // 通知反正肯定不满了 生产者可以来生产了
            WakeupProducer();
            UnlockQueue();
        }
    private:
        // 命名习惯 带_是成员
        queue<T> bq_;
        pthread_mutex_t mtx_;
        // 当满了的时候 就不要让生产者生产了 让消费者来消费
        // 当消费空了 就不要让生产者生产了 让生产者来生产
        int cap_;// 队列元素上限
        pthread_cond_t is_full_;// bq_是满的 消费者在该条件变量下等待
        pthread_cond_t is_empty_;// bq_是空的 生产者在该条件变量下等待
        void LockQueue()
        {
            pthread_mutex_lock(&mtx_);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&mtx_);
        }

        void ProducterWait()
        {
            // 带把锁的原因是防止自己去等的时候仍然上着锁不让访问临界资源
            // 1.调用这个函数时，会首先释放mtx_, 然后再挂起自己
            // 2.当它返回的时候，会首先自动竞争锁，获取到锁之后才能返回
            pthread_cond_wait(&is_empty_, &mtx_);
        }
        void ConsumerWait()
        {
            pthread_cond_wait(&is_full_, &mtx_);
        }
        void WakeupConsumer()
        {
            // 唤醒消费者 它在is_full_等
            pthread_cond_signal(&is_full_);
        }
        void WakeupProducer()
        {
            // 唤醒生产者 它在is_empty_等
            pthread_cond_signal(&is_empty_);
        }
    };
}

// CpTest.cc
// hpp文件在开源库中常见 不同于普通的头文件.h 它可以做到模板类的声明和定义放在一起 
#include "BlockQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace ns_blockqueue;

void* producter(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        // 生产者生产的慢一点
        // sleep(2);
        int data = rand() % 20 + 1;
        bq->push(data);
        cout << "生产者生产了一个数据:" << data << endl;
    }
}


void* consumer(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        sleep(2);
        int data;
        bq->pop(&data);
        cout << "消费者消费了一个数据:" << data << endl;
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c, p;
    pthread_create(&p, nullptr, producter, (void*)bq);
    pthread_create(&c, nullptr, consumer, (void*)bq);

    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete bq;
}
```

&emsp;&emsp;回忆：进程间通信中的管道也是写满了``os``就不让再写了，等读的那边读，用的也是生产者消费者模型，所以进程间通信的本质就是一种生产者消费者模型，只不过缓冲区在操作系统手里罢了。

&emsp;&emsp;简单写一个多个生产者多个消费者的情况：

```cpp
// BlockQu
#pragma once
#include <iostream>
#include <queue>
using namespace std;

const int defaultcap = 100;

namespace ns_blockqueue
{
    template <class T>
    class BlockQueue
    {
    public:
        BlockQueue(int cap = defaultcap) :cap_(cap)
        {
            // 初始化锁和两个条件变量
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&is_full_, nullptr);
            pthread_cond_init(&is_empty_, nullptr);
        }
        ~BlockQueue() 
        {
            // 销毁锁和条件变量
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&is_full_);
            pthread_cond_destroy(&is_empty_);
        }
        // 队列是否满
        bool isFull()
        {
            return bq_.size() == cap_;
        }
        // 队列是否空
        bool isEmpty()
        {
            return bq_.size() == 0;
        }

        // const T& 输入型参数
        // T&输入输出型参数
        // T*输出型参数
        void push(const T& in)
        {
            // 先队列中放数据 生产函数
            // 还有bug?

            // 先把队列锁上
            LockQueue();
            // 如果满了就去等待
            if (isFull()) 
            {
                ProducterWait();
                // pthead_cond_wait返回时自动再次竞争锁 竞争到了才回来
                // 所以此时是有锁状态
            }
            bq_.push(in);

            // 生产者生产了 自然可以通知消费者可以消费了
            // 策略:如果队列中的元素大于2个 唤醒消费者
            if (bq_.size() > 2) WakeupConsumer();
            UnlockQueue();
        }

        void pop(T* out)
        {  
            LockQueue();
            // 从队列中拿数据 消费函数
            // 如果队列为空 消费者去等待
            if (isEmpty())
            {
                ConsumerWait();
            }
            // 拿走数据
            *out = bq_.front();
            bq_.pop();

            // 通知反正肯定不满了 生产者可以来生产了
            // WakeupProducer();

            // 策略 个数小于30时就去唤醒生产者。
            if (bq_.size() <= 30) WakeupProducer();
            UnlockQueue();
        }
    private:
        // 命名习惯 带_是成员
        queue<T> bq_;
        pthread_mutex_t mtx_;
        // 当满了的时候 就不要让生产者生产了 让消费者来消费
        // 当消费空了 就不要让生产者生产了 让生产者来生产
        int cap_;// 队列元素上限
        pthread_cond_t is_full_;// bq_是满的 消费者在该条件变量下等待
        pthread_cond_t is_empty_;// bq_是空的 生产者在该条件变量下等待
        void LockQueue()
        {
            pthread_mutex_lock(&mtx_);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&mtx_);
        }

        void ProducterWait()
        {
            // 带把锁的原因是防止自己去等的时候仍然上着锁不让访问临界资源
            // 1.调用这个函数时，会首先释放锁mtx_, 然后再挂起自己
            // 2.当它返回的时候，会首先自动竞争锁，获取到锁之后才能返回
            pthread_cond_wait(&is_empty_, &mtx_);
        }
        void ConsumerWait()
        {
            pthread_cond_wait(&is_full_, &mtx_);
        }
        void WakeupConsumer()
        {
            // 唤醒消费者 它在is_full_等
            pthread_cond_signal(&is_full_);
        }
        void WakeupProducer()
        {
            // 唤醒生产者 它在is_empty_等
            pthread_cond_signal(&is_empty_);
        }
    };
}

// CpTest.cc
// hpp文件在开源库中常见 不同于普通的头文件.h 它可以做到模板类的声明和定义放在一起 
#include "BlockQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace ns_blockqueue;

pthread_mutex_t Pprintmtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t Cprintmtx = PTHREAD_MUTEX_INITIALIZER; 

void* producter(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        // 生产者生产的慢一点
        // sleep(2);
        int data = rand() % 20 + 1;
        bq->push(data);
        pthread_mutex_lock(&Pprintmtx);
        cout << "生产者生产了一个数据:" << data << endl;
        pthread_mutex_unlock(&Pprintmtx);
    }
}


void* consumer(void* args)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)args;
    while (true)
    {
        sleep(2);
        int data;
        bq->pop(&data);
        pthread_mutex_lock(&Cprintmtx);
        cout << "消费者消费了一个数据:" << data << endl;
        pthread_mutex_unlock(&Cprintmtx);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c[10], p[10];
    for (int i = 0; i < 10; ++i)
    {
        pthread_create(&p[i], nullptr, producter, (void*)bq);
        pthread_create(&c[i], nullptr, consumer, (void*)bq);
    }
    for (int i = 0; i < 10; ++i)
    {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }
    delete bq;
}
```

## 4 以上代码的两点补充

&emsp;&emsp;第一点注意条件：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517190811.png" style="zoom:80%;" />

&emsp;&emsp;问题：因为这里是``if``判断，如果生产条件不具备却被唤醒了或者挂起失败了，就会往临界区里头插入不该插入的东西，所以这里使用``if``来判断是不太完善的。

&emsp;&emsp;解决方式就是if改成while即可，直到不满足``isFull()``条件后才退出：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517191235.png" style="zoom:80%;" />

&emsp;&emsp;挂起失败就是函数调用失败了，未能把自己放到等待队列中；

&emsp;&emsp;还有可能被伪唤醒，假如计算机是多CPU的，如果条件变量信息过量，那么可能因为收到历史信息而不断唤醒自己，但实际上条件现在并不满足。

&emsp;&emsp;生产和消费，传输数据只是第一步，另外两个问题？

- 数据怎么来的？？耗时吗？
- 数据后续怎么处理？耗时吗？

&emsp;&emsp;上面两个问题都是显然要耗时的，但是仅仅传一个int完全无法体现这个，我们要插入一个场景，来给生产消费者模型具体化。

&emsp;&emsp;我们用一个场景：任务处理。

&emsp;&emsp;生产者生产出一批数据：参与运算的x y和``+-*/``，封装成一个任务，消费者帮我运算这个任务。

&emsp;&emsp;新增一个``Task.hpp``，里头放我们的任务类：

```cpp
#pragma once
#include <iostream>
#include <pthread.h>
using namespace std;
    


namespace Ns_task
{
    class Task
    {
    private:
        int x_;
        int y_;
        char op_;// 操作 + - * / 
    public:
        Task() {}
        Task(int x, int y, char op) : x_(x), y_(y), op_(op) {}
        ~Task() {}
        int Run()
        {
            cout << "当前任务正在被" << pthread_self() << "处理," << x_ << ' ' << op_ << ' ' << y_ << "=";
            int res = 0;
            if (op_ == '+') res =  x_ + y_;
            else if (op_ == '-') res =  x_ - y_;
            else if (op_ == '*') res =  x_ * y_;
            else if (op_ == '/') res =  x_ / y_;
            else if (op_ == '%') res=  x_ % y_;
            else 
            {
                cout << "op error" << endl;
                return -1;
            }
            cout << res << endl;
            return res;
        }
        int operator()()
        {
            return Run();
        }
    };
}
```

&emsp;&emsp;修改一下``CpTest.cpp``：

```cpp
#include "BlockQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "task.hpp"
using namespace ns_blockqueue;
using namespace Ns_task;
pthread_mutex_t Pprintmtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t Cprintmtx = PTHREAD_MUTEX_INITIALIZER; 

void* producter(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;
    string ops = "+-*/%";
    while (true)
    {
        // 生产者生产的慢一点
        // sleep(2);
        /*int data = rand() % 20 + 1;
        bq->push(data);
        pthread_mutex_lock(&Pprintmtx);
        cout << "生产者生产了一个数据:" << data << endl;
        pthread_mutex_unlock(&Pprintmtx);*/

        // 构造一个任务
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = ops[rand() % 5];
        Task t(x, y, op);
        bq->push(t);
        cout << "生产者生产了一个任务" << endl;
        sleep(1);
    }
}


void* consumer(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;
    while (true)
    {
        // sleep(2);
        // int data;
        // bq->pop(&data);
        // pthread_mutex_lock(&Cprintmtx);
        // cout << "消费者消费了一个数据:" << data << endl;
        // pthread_mutex_unlock(&Cprintmtx);
        Task t;
        bq->pop(&t);// 取出了任务
        int res = t();
        cout << pthread_self() << "线程已经解决问题:" << res << endl;
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c[1], p[1];
    for (int i = 0; i < 1; ++i)
    {
        pthread_create(&p[i], nullptr, producter, (void*)bq);
        pthread_create(&c[i], nullptr, consumer, (void*)bq);
    }
    for (int i = 0; i < 1; ++i)
    {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }
    delete bq;
}
```

&emsp;&emsp;生产消费者模型的关键是它解决了当**生产者生产任务很慢的时候，或者消费者计算任务很慢的时候，**可以同步的进行另一端的任务，大大提高了并发性。

&emsp;&emsp;后续结合网络以后，这个不就是来了消息我们就去处理吗，显然生产者消费者模型特别适合这个场景。

# 六、POSIX信号量

## 1 信号量的概念

- 信号量本质就是一个计数器，描述临界资源中资源数目的大小，即最多能有多少资源能分配给线程。
- 与电影院中的票的售卖类似，买票的本质是预订资源。临界资源如果可以划分为一个一个小资源，如果处理得当，我们也有可能让多个线程访问临界资源的不同区域，从而实现并发，**信号量就是多线程预订临界资源的手段**。

&emsp;&emsp;每个线程想要访问临界资源，都要先申请信号量资源，类似看电影之前我们必须得先买票。

&emsp;&emsp;一个线程只要申请到了信号量，那么线程一定有那块资源的，就好像买到电影票后，那个位置一定是你的。

&emsp;&emsp;信号量本质是一把计数器，合理使用信号量可以达到多线程预订临界资源。

## 2 认识信号量对应的函数接口

&emsp;&emsp;信号计数器的原理与申请信号量和释放信号量的伪代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517201542.png" style="zoom:80%;" />

### I sem_init—创建信号量

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517201643.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数和我们的其他的那些一样；第二个参数表示是否想进程间共享，我们不想设为0即可；第三个参数表示定义的信号量的初始值是多少。

&emsp;&emsp;返回值成功就是0，失败就是-1.

### II sem_destroy—销毁信号量

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517201834.png" style="zoom:80%;" />

### III sem_wait—典型的P操作

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517201956.png" style="zoom:80%;" />

### IV sem_post—典型的V操作

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517202012.png" style="zoom:80%;" />



## 3 认识环形队列

&emsp;&emsp;所谓的环形队列就是一个环状结构。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517203631.png" style="zoom:80%;" />

&emsp;&emsp;接下来我们要在多线程情况下，进行环形队列的并发访问，即基于环形队列的生产者消费者模型。

## 4 结合信号量与环形队列的生产者消费者模型代码

### I 基本原理

&emsp;&emsp;生产者消费者刚开始的时候，指向同一个位置，也就是环形队列为空的时候；

&emsp;&emsp;当生产者和消费者在队列为满的时候，肯定也指向同一个位置。

&emsp;&emsp;那么当队列不为空，不为满的时候，生产者和消费者一定指向的不是同一个位置。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220517205855.png" style="zoom:80%;" />

### II 基本实现思想

&emsp;&emsp;生产者，最关心什么资源呢？**生产者最关心的是环形队列中空的位置。**

&emsp;&emsp;消费者，最关心什么资源呢？**消费者最关心的环形队列中的数据。**

&emsp;&emsp;规则1：生产者不能把消费者套一个圈（这样就覆盖数据了）；

&emsp;&emsp;规则2：消费者不能超过生产者。

&emsp;&emsp;规则3：当生产者和消费者指向同一个位置时，要根据空满的状态，决定让谁先执行。

&emsp;&emsp;除此室外，生产者和消费者可以并发执行。

&emsp;&emsp;那么如何实现呢？

```cpp
// 伪代码
// 起始
sem blank = 10;// 开始时空位置的个数
sem data = 0;// 刚开始数据的位置为0

// 生产
P(blank)// 申请空格子资源
    
V(data)// 表明数据已经生产出来了

// 消费
P(data)// 申请数据资源
    
V(blank)// 消费完了 格子空出来了
```

&emsp;&emsp;模拟一下可以知道，上面这段伪代码就实现了空的时候生产者先访问，满的时候消费者先访问，这种申请自己的资源，释放对方的资源，完成了一个环形的生产者消费者模型。

### III 代码实现

&emsp;&emsp;单生产单消费的代码如下：

```cpp
// ring_queue.hpp
#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <mutex>

namespace ns_ring_queue
{
    const int g_cap_default = 10;
    template <class T>
    class RingQueue
    {
    private:
        std::vector<T> ring_queue_;
        int cap_; // 固定长度
        // 生产者关心空位置资源
        sem_t blank_sem_;
        // 消费者关心数据资源
        sem_t data_sem_;
        // 生产和消费的位置
        int c_step_;
        int p_step_;
    public:
        RingQueue(int cap = g_cap_default) : cap_(cap), ring_queue_(cap), c_step_(0), p_step_(0)
        {
            sem_init(&blank_sem_, 0, cap);
            sem_init(&data_sem_, 0, 0);
        }
        ~RingQueue()
        {
            // 销毁信号量
            sem_destroy(&blank_sem_);
            sem_destroy(&data_sem_);
        }

    public:
        // 当前高优先级实现单生产者消费者模型
        // 生产接口 
        void push(const T &in)
        {
            // 申请blank_sem
            sem_wait(&blank_sem_); // P(空位置)
            // 可以生产了 但是往啥位置生产呢？
            ring_queue_[p_step_] = in;
            // 数据增加了一个 V(数据)
            sem_post(&data_sem_);

            // 更新位置
            p_step_ = (p_step_ + 1) % cap_;
        }
        // 消费接口
        void pop(T *out)
        {
            // P(data)
            sem_wait(&data_sem_);
            *out = ring_queue_[c_step_];
            sem_post(&blank_sem_);// V(空格子)
            // 更新位置
            c_step_ = (c_step_ + 1) % cap_;

        }
    };

}


//ring_cp.cc
#include "ring_queue.hpp"

using namespace ns_ring_queue;

std::mutex mtx;

void* producter(void* args)
{
    auto* rq = (RingQueue<int>*)args;
    while (true)
    {
        // rq->pop();
        int data = rand() % 20 + 1;
        mtx.lock();
        std::cout << "生产数据是" << data << std::endl;
        mtx.unlock();
        rq->push(data);
        // sleep(1);
    }
}

void* consumer(void* args)
{
    auto* rq = (RingQueue<int>*)args;
    while (true)
    {
        // rq->push();
        int data = 0;
        rq->pop(&data);
        mtx.lock();
        std::cout << "消费数据是" << data << std::endl;
        mtx.unlock();
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t c, p; 
    auto* rq = new RingQueue<int>();
    pthread_create(&c, nullptr, consumer, (void*)rq);
    pthread_create(&p, nullptr, producter, (void*)rq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    return 0;
}
```

&emsp;&emsp;如果要改成多生产者多消费怎么弄呢？生产者和生产者之间，消费者和消费者之间，都是互斥的，无非就是加锁就可以做到，生产者和消费者的互斥我们已经通过信号量完成了，多给两把锁，一把去锁生产者的生产过程，另一把去锁消费者的消费过程，并且最好把锁放到申请信号量后面，这样可以多个线程可以提前把信号量准备好，效率更高一些，如果在申请信号量前面加锁，那么申请信号量都是单执行流的，和单消费也就没啥区别了。

&emsp;&emsp;多生产多消费的优势在于并发的获得和处理数据，并不在拿数据和放数据的过程

```cpp
// ring_queue.hpp
#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <mutex>

namespace ns_ring_queue
{
    const int g_cap_default = 50;
    template <class T>
    class RingQueue
    {
    private:
        std::vector<T> ring_queue_;
        int cap_; // 固定长度
        // 生产者关心空位置资源
        sem_t blank_sem_;
        // 消费者关心数据资源
        sem_t data_sem_;
        // 生产和消费的位置
        int c_step_;
        int p_step_;
        pthread_mutex_t c_mtx_;
        pthread_mutex_t p_mtx_;
    public:
        RingQueue(int cap = g_cap_default) : cap_(cap), ring_queue_(cap), c_step_(0), p_step_(0)
        {
            sem_init(&blank_sem_, 0, cap);
            sem_init(&data_sem_, 0, 0);
            pthread_mutex_init(&c_mtx_, nullptr);
            pthread_mutex_init(&p_mtx_, nullptr);
        }
        ~RingQueue()
        {
            // 销毁信号量
            sem_destroy(&blank_sem_);
            sem_destroy(&data_sem_);
            pthread_mutex_destroy(&c_mtx_);
            pthread_mutex_destroy(&p_mtx_);
        }

    public:
        // 当前高优先级实现单生产者消费者模型
        // 生产接口 
        void push(const T &in)
        {
            // 把上锁放到申请信号量前面的话 只有一个线程能来申请信号量 其实和单生产者单消费者没啥区别
            // 申请blank_sem
            sem_wait(&blank_sem_); // P(空位置)
            pthread_mutex_lock(&p_mtx_);// 有资格竞争锁的前提是先拿信号量 这样就可以预先分配信号量 效率更高
            // 可以生产了 但是往啥位置生产呢？
            ring_queue_[p_step_] = in;
            // 数据增加了一个 V(数据)
            sem_post(&data_sem_);

            // 更新位置
            p_step_ = (p_step_ + 1) % cap_;
            pthread_mutex_unlock(&p_mtx_);
        }
        // 消费接口
        void pop(T *out)
        {
            // P(data)
            sem_wait(&data_sem_);
            pthread_mutex_lock(&c_mtx_);
            *out = ring_queue_[c_step_];
            sem_post(&blank_sem_);// V(空格子)
            // 更新位置
            c_step_ = (c_step_ + 1) % cap_;
            pthread_mutex_unlock(&c_mtx_);
        }
    };
}

// ring_cp.cc
#include "ring_queue.hpp"

using namespace ns_ring_queue;
const int num = 5;
std::mutex mtx;  // 打印的锁
//std::mutex pmtx; // 生产者的锁 不能多个生产者同时生产
// std::mutex cmtx; // 消费者的锁 不能多个消费者同时消费

void *producter(void *args)
{
    auto *rq = (RingQueue<int> *)args;
    while (true)
    {
        // rq->pop();
        //pmtx.lock();
        int data = rand() % 20 + 1;
        mtx.lock();
        std::cout << "线程" << pthread_self() << "生产数据的是" << data << std::endl;
        mtx.unlock();
        rq->push(data);
        // pmtx.unlock();
        sleep(2);
    }
}

void *consumer(void *args)
{
    auto *rq = (RingQueue<int> *)args;
    while (true)
    {
        // rq->push();
        // cmtx.lock();
        int data = 0;
        rq->pop(&data);
        mtx.lock();
        std::cout << "线程" << pthread_self() << "消费数据是" << data << std::endl;
        mtx.unlock();
        // cmtx.unlock();
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t c[num], p[num];
    auto *rq = new RingQueue<int>();
    for (int i = 0; i < num; ++i)
    {
        pthread_create(c + i, nullptr, consumer, (void *)rq);
        pthread_create(p + i, nullptr, producter, (void *)rq);
    }
    for (int i = 0; i < num; ++i)
    {
        pthread_join(c[i], nullptr);
        pthread_join(p[i], nullptr);
    }
    return 0;
}
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518192602.png" style="zoom:80%;" />

&emsp;&emsp;通过泛型把int改为Task类型，试一试：

```cpp
// ring_cp.cc
#include "ring_queue.hpp"
#include "task.hpp"
using namespace ns_ring_queue;
using namespace Ns_task;
const int num = 5;
std::mutex mtx;  // 打印的锁
//std::mutex pmtx; // 生产者的锁 不能多个生产者同时生产
// std::mutex cmtx; // 消费者的锁 不能多个消费者同时消费

void *producter(void *args)
{
    auto *rq = (RingQueue<Task> *)args;
    const string ops = "+-*/%";
    while (true)
    {
        // rq->pop();
        //pmtx.lock();
        int x = rand() % 20 + 1;
        int y = rand() % 20 + 1;
        char op = ops[rand() % 5];
        Task t(x, y, op);
        mtx.lock();
        std::cout << "线程" << pthread_self() << "生产数据的是" << t.show() << std::endl;
        mtx.unlock();
        rq->push(t);
        // pmtx.unlock();
        sleep(2);
    }
}

void *consumer(void *args)
{
    auto *rq = (RingQueue<Task> *)args;
    while (true)
    {
        // rq->push();
        // cmtx.lock();
        Task t;
        rq->pop(&t);
        mtx.lock();
        // std::cout << "线程" << pthread_self() << "消费数据是" << << t() << std::endl;
        t.Run();
        mtx.unlock();
        // cmtx.unlock();
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t c[num], p[num];
    auto *rq = new RingQueue<Task>();
    for (int i = 0; i < num; ++i)
    {
        pthread_create(c + i, nullptr, consumer, (void *)rq);
        pthread_create(p + i, nullptr, producter, (void *)rq);
    }
    for (int i = 0; i < num; ++i)
    {
        pthread_join(c[i], nullptr);
        pthread_join(p[i], nullptr);
    }
    return 0;
}

// task.hpp
#pragma once
#include <iostream>
#include <pthread.h>
using namespace std;
    


namespace Ns_task
{
    class Task
    {
    private:
        int x_;
        int y_;
        char op_;// 操作 + - * / 
    public:
        Task() {}
        Task(int x, int y, char op) : x_(x), y_(y), op_(op) {}
        ~Task() {}
        int Run()
        {
            cout << "当前任务正在被" << pthread_self() << "处理," << x_ << ' ' << op_ << ' ' << y_ << "=";
            int res = 0;
            if (op_ == '+') res =  x_ + y_;
            else if (op_ == '-') res =  x_ - y_;
            else if (op_ == '*') res =  x_ * y_;
            else if (op_ == '/') res =  x_ / y_;
            else if (op_ == '%') res=  x_ % y_;
            else 
            {
                cout << "op error" << endl;
                return -1;
            }
            cout << res << endl;
            return res;
        }
        int operator()()
        {
            return Run();
        }
        string show()
        {
            string message = to_string(x_);
            message.push_back(' ');
            message += op_;
            message += to_string(y_);
            message += '=';
            message += '?';
            return message;
        }
    };
}

// ring_queue.hpp
#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <mutex>

namespace ns_ring_queue
{
    const int g_cap_default = 50;
    template <class T>
    class RingQueue
    {
    private:
        std::vector<T> ring_queue_;
        int cap_; // 固定长度
        // 生产者关心空位置资源
        sem_t blank_sem_;
        // 消费者关心数据资源
        sem_t data_sem_;
        // 生产和消费的位置
        int c_step_;
        int p_step_;
        pthread_mutex_t c_mtx_;
        pthread_mutex_t p_mtx_;
    public:
        RingQueue(int cap = g_cap_default) : cap_(cap), ring_queue_(cap), c_step_(0), p_step_(0)
        {
            sem_init(&blank_sem_, 0, cap);
            sem_init(&data_sem_, 0, 0);
            pthread_mutex_init(&c_mtx_, nullptr);
            pthread_mutex_init(&p_mtx_, nullptr);
        }
        ~RingQueue()
        {
            // 销毁信号量
            sem_destroy(&blank_sem_);
            sem_destroy(&data_sem_);
            pthread_mutex_destroy(&c_mtx_);
            pthread_mutex_destroy(&p_mtx_);
        }

    public:
        // 当前高优先级实现单生产者消费者模型
        // 生产接口 
        void push(const T &in)
        {
            // 把上锁放到申请信号量前面的话 只有一个线程能来申请信号量 其实和单生产者单消费者没啥区别
            // 申请blank_sem
            sem_wait(&blank_sem_); // P(空位置)
            pthread_mutex_lock(&p_mtx_);// 有资格竞争锁的前提是先拿信号量 这样就可以预先分配信号量 效率更高
            // 可以生产了 但是往啥位置生产呢？
            ring_queue_[p_step_] = in;
            // 数据增加了一个 V(数据)
            sem_post(&data_sem_);

            // 更新位置
            p_step_ = (p_step_ + 1) % cap_;
            pthread_mutex_unlock(&p_mtx_);
        }
        // 消费接口
        void pop(T *out)
        {
            // P(data)
            sem_wait(&data_sem_);
            pthread_mutex_lock(&c_mtx_);
            *out = ring_queue_[c_step_];
            sem_post(&blank_sem_);// V(空格子)
            // 更新位置
            c_step_ = (c_step_ + 1) % cap_;
            pthread_mutex_unlock(&c_mtx_);
        }
    };
}
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220518194016.png" style="zoom:80%;" />



