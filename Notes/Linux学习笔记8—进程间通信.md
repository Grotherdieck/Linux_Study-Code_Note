# 一、进程间通信引出

&emsp;&emsp;IPC（Inter-[Process](https://baike.baidu.com/item/Process/1170280)[ Communication](https://baike.baidu.com/item/ Communication/20394231)，[进程间通信](https://baike.baidu.com/item/进程间通信/1235923)）。进程间通信是指两个进程的数据之间产生交互。

&emsp;&emsp;我们在之前的学习中，进程都是各跑各的，没有互相协作。

&emsp;&emsp;而进程是程序员起的，这说明我们前面的讲解中，程序员没有什么相互合作。

&emsp;&emsp;但实际上，处理一些业务逻辑是很需要程序员之间的相互合作的，所以进程之间也需要存在特定的相互协同工作场景。

&emsp;&emsp;如下例，三个进程相互协作，完成了统计``test.cc``中有含``int``有多少行。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417095139.png" style="zoom:80%;" />

&emsp;&emsp;一个进程可能要把自己的数据交付给另一个进程，让其进行处理，这个操作就叫做**进程间通信**。

&emsp;&emsp;然而进程是具有独立性的，交互数据成本一定很高，我们都是独立的，有独立的页表，映射到不同的物理空间，我怎么可能看到你的数据，即一个进程是看不到另一个进程的资源的。

&emsp;&emsp;所以想要完成进程间通信，必须要有操作系统的干预。

&emsp;&emsp;两个进程具有独立性，两个进程要相互通信，两个进程必须得先能看到一份公共资源。

&emsp;&emsp;这里的资源，就是一段内存就行。

&emsp;&emsp;那这份公共资源应该属于谁呢？首先肯定不能属于这两个进程，因为进程具有独立性，如果属于一个进程，那另一个进程肯定看不到这份资源，所以这份资源只能属于操作系统。

&emsp;&emsp;进程间通信的本质：其实是OS参与，提供一份所有进程都能看到的一份公共资源，可能以文件方式提供，也可能以队列方式提供，也可能就是原始的内存块。

&emsp;&emsp;因为提供的公共资源的组织形式不同，所以进程间通信的方式也有很多种。

# 二、进程间通信的目的

- 数据传输：一个进程需要将它的数据发送给另一个进程 
- 资源共享：多个进程之间共享同样的资源。 
- 通知事件：一个进程需要向另一个或一组进程发送消息，通知它（它们）发生了某种事件（如进程终止 时要通知父进程）。 
- 进程控制：有些进程希望完全控制另一个进程的执行（如Debug进程），此时控制进程希望能够拦截另 一个进程的所有陷入和异常，并能够及时知道它的状态改变。

# 三、进程间通信的发展

&emsp;&emsp;由各种各样的实验，定了许多进程间通信的标准，我们这里用的是``System V IPC``标准，主要标准还有``POSIX IPC``标准。

&emsp;&emsp;System V IPC主要有三种进程间通信方式：System V消息队列、System V共享内存、System V信号量。

# 四、管道

## 1 管道的原理

&emsp;&emsp;首先考虑一个事情：父子进程是两个独立的进程，所以它们之间的通信也属于进程间通信，匿名管道的实现就基于父子进程。

&emsp;&emsp;父进程创建子进程后，子进程是否需要拷贝父进程的``struct files_struct``(文件描述符表)呢？

&emsp;&emsp;答案是必须要拷贝的，第一个原因是``struct files_struct``是属于进程的，不是属于文件的，因为进程具有独立性，所以子进程也得拷贝一份；另一个原因是如果不拷贝，父进程的读写会影响子进程看这个文件，就会有不好的影响。

&emsp;&emsp;不过``struct file``不用拷贝，因为这个东西属于文件。

&emsp;&emsp;但是子进程会完成父进程``struct files_struct``的拷贝，所以其指向文件的指针也会拷贝，父子进程此时会指向同样的文件！

&emsp;&emsp;write其实就是把数据从用户缓冲区写到内核缓冲区，然后触发底层的写入函数写入对应的硬件。

&emsp;&emsp;父子进程看到了同一份公共资源！就是那个文件，所以父进程可以把内容写到内核缓冲区去，不去写到硬件上，然后子进程就可以通过那个文件描述符读到这些数据，就实现了进程间的通信。

&emsp;&emsp;这个原理就是管道。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417135717.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417140317.png" style="zoom:80%;" />

&emsp;&emsp;管道的建立过程也说明了**管道只是一个能单向通信的通信通道**，上面的图片仅仅是一种管道，也可以让父进程关闭写端，子进程关闭读端，父进程读子进程写。

## 2 匿名管道的系统调用—pipe

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417140800.png" style="zoom:80%;" />

&emsp;&emsp;创建成功返回0，创建失败返回-1.

&emsp;&emsp;``pipefd[2]``是一个输出型参数，我们通过这个参数读取到打开的两个``fd``。

&emsp;&emsp;简单测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417141410.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417141337.png" style="zoom:80%;" />

&emsp;&emsp;我们让父进程读，子进程写，进行一个简单的测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417143140.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417143207.png" style="zoom:80%;" />

&emsp;&emsp;改为父进程睡眠1s，子进程疯狂写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417143831.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417143743.png" style="zoom:80%;" />

&emsp;&emsp;因为此时writepipe中只要有缓冲区就一直写入，read里头缓冲区只要有缓冲区就一直读取，这种连续的流读取称为字节流。

&emsp;&emsp;因为这样每次读取消息也是一下子会读很多很乱，甚至可能读到下一条，这是不好的，因此其实进程间通信也需要像网络通信那样定制**协议**。

&emsp;&emsp;我们上面的管道被称为匿名管道：特点是管道是面向字节流的（字符串怎么分隔我不管，你读多少自己定）、并且是一个只能单向通信的通信信道，并且仅限于父子通信。

&emsp;&emsp;下面我们改为子进程一直写，父进程不读取呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417145528.png" style="zoom: 80%;" />

&emsp;&emsp;发现写到65536(64kb)就不写了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417145936.png" style="zoom: 67%;" />

&emsp;&emsp;当没有读，写满``64kb``时，写端就不再写入了，为什么呢？

&emsp;&emsp;因为**管道有大小**，经过我们的测试，我们云服务器上的管道的大小是``64kb``。

&emsp;&emsp;不写了本质是缓冲区满了，我们要等对方来读。

&emsp;&emsp;修改为父进程每次拿一个，发现子进程并没有继续写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417150732.png" style="zoom:80%;" />

&emsp;&emsp;改为一次拿64个：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417150909.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417151020.png" style="zoom:80%;" />

&emsp;&emsp;发现子进程还是不动弹。

&emsp;&emsp;把读的东西再开大一点。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417151216.png" style="zoom:80%;" />

&emsp;&emsp;子进程又重新写入了。

&emsp;&emsp;这说明管道自带一定的同步机制，man手册中管道的大小于容量。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417151741.png" style="zoom:80%;" />

&emsp;&emsp;当写入的字节数小于PIPE_BUF时，Linux保证原子性写入，否则Linux不保证原子性写入。

&emsp;&emsp;验证我们的管道缓冲区满了以后，我们要拿走4kb的数据后，另一方才能继续写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417152214.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417152224.png" style="zoom:80%;" />

&emsp;&emsp;两个10s过后，读了4KB数据，子进程开始接着写，这里的唤醒机制和管道采用的策略有关系。

&emsp;&emsp;当我们写的特别慢，读的特别快时，父进程就会等子进程把东西写进来。

&emsp;&emsp;所以不仅有读端等待写端写，也有写端等待读端读取，这叫做同步机制。

&emsp;&emsp;下面测试一端退出后另一端的状态。

&emsp;&emsp;先看看写端退出读端的情况：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417153439.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417153506.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417153550.png" style="zoom:80%;" />

&emsp;&emsp;写端一直写，关闭读端；

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417153854.png" style="zoom:80%;" />

&emsp;&emsp;发现写端自动退出了循环：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417153959.png" style="zoom:80%;" />

&emsp;&emsp;读端关闭，写端还在写入，从OS层面上合理吗？显然不合理，已经不能读了，你还在写入，本质就是在浪费OS的资源，OS会直接终止写入进程。

&emsp;&emsp;OS会给写入进程发送``SIGPIPE``信号。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417154542.png" style="zoom:80%;" />

&emsp;&emsp;我们可以用父进程等待去验证子进程被异常退出了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417155113.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417155047.png" style="zoom:80%;" />

## 3 匿名管道的四种情况和五个特点

**四种情况**：

&emsp;&emsp;读端不读或读的慢，写端要等读端；

&emsp;&emsp;读端关闭，写端收到SIGPIPE，直接终止；

&emsp;&emsp;写端不写或写的慢，读端要等写端；

&emsp;&emsp;写端关闭，读端读完全部pipe中的数据，然后再读会读到0，表明管道文件结尾。

**五个特点**:

- 管道是一个单向通信的通道
- 管道是面向字节流的
- 由于孙进程可以继承子进程的文件描述符，所以管道是可以用于具有血缘关系的进程进行通信，常用于父子的通信。
- 管道是自带互斥同步机制的，4kb以内保证原子性写入。
- 管道是文件吗？如果一个文件只被当前进程打开，相关进程退出了，被打开的文件会怎么样呢？会被操作系统自动关闭。 但是**管道的生命周期是随进程的**。

# 五、命名管道

##  1 mkfifo——创建命名管道的命令

&emsp;&emsp;为了解决匿名管道只能用用于父子通信的特征，我们引入了**命名管道**，其特点基本继承了匿名管道的所有特点，但不限制进程的血缘关系。

&emsp;&emsp;系统中的命令``mkfifo``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417160226.png" style="zoom:80%;" />

&emsp;&emsp;简单测试：命名管道的命令行操作。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420185706.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417160429.png" style="zoom:80%;" />

&emsp;&emsp;下面讲解命名管道的原理。

&emsp;&emsp;因为进程是具有独立性的，所以其实进程间通信的成本比较高，必须先解决一个问题——让不同的进程看到同一份资源（内存文件、内存、队列），这份资源既不属于进程A，也不属于进程B，所以这份资源只能由操作系统提供。

&emsp;&emsp;所以之前我们的匿名管道的原理就是通过子进程继承父进程资源的特性，使得不同的资源看到同一份资源。

&emsp;&emsp;我们在磁盘中标识一个文件，是通过路径/文件名来标识一个文件，这种表示方法是唯一的。

&emsp;&emsp;那么假如a进程打开一个文件，写东西，然后关闭，然后b进程打开这个文件，然后读取里头的东西，然后关闭文件，这显然是ok的，做到了一次很原始的进程间通信。

&emsp;&emsp;这是不是太挫了，我们可以让两个进程同时以读写方式同时打开这个文件，这应该是可以的，标准输入和标准输出这不就可以被多个进程同时打开吗？

&emsp;&emsp;那么这个文件就在内存中了，一个进程写，一个进程读，并且没必要把文件的内容刷新到磁盘上，因为这样做没有意义。

&emsp;&emsp;那么进程A和进程B是如何看到并且打开同一个文件的呢？需要通过路径加文件名找到这个文件，并且这个文件需要具有一个特性：数据不会刷到磁盘上（为了效率，刷到磁盘上完全没必要），这种文件就叫命名管道。

## 2 系统调用—mkfifo

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420191551.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数是路径+文件名，第二个参数是命名管道文件的权限。

&emsp;&emsp;返回值：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420191642.png" style="zoom:80%;" />

&emsp;&emsp;如果管道文件已经存在 则返回错误。

&emsp;&emsp;``sever.c``创建一个命名管道：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420191916.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420192013.png" style="zoom:80%;" />

&emsp;&emsp;发现这个权限和我们的创建权限不同，根据我们之前的学习，这和权限掩码``umask``有关。

&emsp;&emsp;使用系统调用``umask(0)``把当前进程创建文件的权限掩码设为0即可，这个系统调用不会影响其他情况的``umask``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420192319.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420192352.png" style="zoom:80%;" />

&emsp;&emsp;接下来就像操作文件一样操作这个命名管道文件就行了，为温习文件操作，这里我们用文件操作的系统调用接口。

``sever.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420194648.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420194632.png" style="zoom:80%;" />

``client.c``:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420194524.png" style="zoom:80%;" />

&emsp;&emsp;看看效果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420194420.png" style="zoom:80%;" />

&emsp;&emsp;命名管道也是基于**字节流**的，所以通信双方其实也要制定“协议”。

&emsp;&emsp;假设我们让客户端控制服务器执行对应命令，就是让服务器进程通过命名管道读取命令后，利用``fork``和``execl``创建子进程去执行对应命令：

``sever.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422151604.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422151757.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422151815.png" style="zoom:80%;" />

``client.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422152026.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422152704.png" style="zoom:80%;" />

&emsp;&emsp;这就是命名管道通信，任何关系的进程都可以这样通信。

&emsp;&emsp;我们上面的mode，通过进程间通信，完成了进程间传输数据，并且完成了一个进程控制另一个进程。

&emsp;&emsp;接下来我们让``sever``进程慢点读，验证一下命名管道文件不会把数据刷到磁盘上，做法就是让``sever``sleep一段时间再读取。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422153856.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422153947.png" style="zoom:80%;" />

&emsp;&emsp;为什么命名管道需要名字呢，因为保证毫无关联的进程看到同一份资源，管道文件必须有文件名，而匿名管道可以没有名字，因为它是通过父子进程继承的方式来看到同一份资源，所以不需要名字来标识同一份资源。

# 六、SystemV标准的进程间的通信方式

## 1 概论

&emsp;&emsp;我们之前讲的进程间通信方案都是基于文件的通信方案，而``SystemV``标准是在OS层面专门为进程间通信设计的一个方案，他是上世纪设计操作系统的计算机科学家和程序员设计的。

&emsp;&emsp;这个接口当然也要给用户用，那么一种OS层面的给用户用的方式是什么呢？系统调用！

&emsp;&emsp;``SystemV``标准一定会存在专门用来进程间通信的接口。

&emsp;&emsp;那么如果要给操作系统层面提供的进程间通信系统调用，一定需要有个人、组织、机构来制定标准。

&emsp;&emsp;``SystemV``方案的全称：在同一主机内的进程间通信方案。

&emsp;&emsp;进程间通信的本质：先让不同的进程看到同一份资源，``SystemV``提供了三种方案：共享内存、消息队列(有点落伍)、信号量。

&emsp;&emsp;前两个以传送数据为目的，后一个以实现进程间同步或互斥为目的，我们重点关注共享内存和信号量。

## 2 共享内存

&emsp;&emsp;原理图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420201712.png" style="zoom:80%;" />

&emsp;&emsp;准备工作：

- OS内可能存在多个进程，同时使用不同的共享内存进行进程间通信，那么共享内存在系统中可能存在多份，所以OS需要管理这一堆共享内存的释放与申请。那么如何管理呢，六字真言，先描述再组织。
- 怎样保证两个或多个进程看到的是同一个共享内存呢？就好像假设我们去上课，你怎么知道你去的教室和讲这个课的老师去的教室是同一个呢？显然这里需要一个**唯一标识符**可以表示其唯一性，方便不同的进程能识别同一个共享内存资源,那么这个标识符(id)应该在哪呢？和进程的``pid``一样，这个id一定在描述共享内存的结构体中。

&emsp;&emsp;了解接口：

1 ``shmget``创建共享内存

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205301.png" style="zoom:80%;" />

2 ``shmctl``控制共享内存

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205346.png" style="zoom:80%;" />

3 ``shmat``挂接这块内存 关联这块内存

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205507.png" style="zoom:80%;" />

4 ``shmdt``把进程和共享内存去关联

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205549.png" style="zoom:80%;" />

## 3 创建共享内存:ftok+shmget

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205628.png" style="zoom:80%;" />

&emsp;&emsp;第二个参数``size``是创建共享内存的大小，一般建议是``4kb``的整数倍。

&emsp;&emsp;第三个参数``shmflg``是标记位，常见两个标记：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420205748.png" style="zoom:80%;" />

&emsp;&emsp;如果单独使用``IPC_CREATE``或者``flag=0``创建一个共享内存，如果创建的共享内存已经存在了，则直接返回已经存在的共享内存，不存在异常返回值的情况。

&emsp;&emsp;第二个选项``IPC_EXCL``单独使用没有意义，``IPC_CREATE | IPC_EXCL``一般都这么使用，组合使用的意义是：如果不存在共享内存，则创建；如果已经存在了共享内存，则返回出错。意义在于如果我调用成功，得到的一定是一个最新的，没有别人使用的共享内存。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420210422.png" style="zoom:80%;" />

&emsp;&emsp;那么第一个参数是什么呢？这是一个唯一的标识符，是内核用来识别共享内存的，本质就是为了让不同的进程能看到同一份资源，得先让不同的进程，看到同一个``id``也就看到了同块共享内存，这里也就是``key``。

&emsp;&emsp;这个``key``是要用户自己设置的，但是我们可能设置了以后和系统的``key``冲突，于是我们使用系统调用``ftok``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420210744.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数是自定义路径名，第二个参数是自定义项目id，下面是它的返回值：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420210828.png" style="zoom:80%;" />

&emsp;&emsp;所以要``key_t``，要先用``ftok``得到一个``key_t``，返回的``key(key_t)``这个东西就是内核中标志共享内存的唯一标识符。

&emsp;&emsp;那么怎么保证不同的进程看到的是同一个共享内存呢？我们只要得到同样的``key``就行，只要我们形成``key``的算法+原始数据是一样的，就可以形成同一个``key``。

&emsp;&emsp;一个简易测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420212559.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420212534.png" style="zoom:80%;" />

&emsp;&emsp;但是再次运行却寄了，这是为啥呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420212629.png" style="zoom:80%;" />

&emsp;&emsp;这是因为我们在``shmget``中加了参数``IPC_CREAT | IPC_EXCL``，它只能创建全新的共享内存，如果该key值的共享内存已经存在，则返回错误，这说明我们创建的那份共享内存仍然存在。

&emsp;&emsp;那我们的进程已经挂了啊，为啥这个共享内存还在呢？这说明共享内存的声明周期不是随进程的，而是随内核的。

&emsp;&emsp;所以我们介绍一个``shell``命令：``ipcs``，来验证一下我们那份共享内存没有消失。

``ipcs``：查看``SystemV``标准的消息队列、贡献内存、信号量。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420212240.png" style="zoom:80%;" />

``icps -m``：查看当前存在的共享内存。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420212737.png" style="zoom:80%;" />

``ipcs -q``：查看当前存在的消息队列。

``ipcs -s``：查看当前存在的信号量。

&emsp;&emsp;所以下面我们介绍一下怎么释放共享内存。

## 4 释放共享内存

### I 命令行操作方式—ipcrm

&emsp;&emsp;我们申请的这块共享内存是属于内核的，它的生命周期是随内核的，需要程序员手动释放，不然只有OS关闭了，这块共享内存才消失。

&emsp;&emsp;而堆资源是随进程的，进程退出内存也就归还了。

&emsp;&emsp;``ipcrm -m``：根据``shmid``删除这个共享内存。

&emsp;&emsp;``ipcrm -q``:删除消息队列。

&emsp;&emsp;``ipcrm -s``:删除信号量。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420213027.png" style="zoom:80%;" />

&emsp;&emsp;为什么我们通过命令操作共享内存时，不用共享内存的``key``而用``shmid``呢？

&emsp;&emsp;``key vs shmid``:

- ``key``只是用在系统内核层面唯一的标识共享内存的，不能用来管理``shm``，类比``struct file``的地址。
- ``shmid``是OS给用户返回的id，用来在用户层进行``shm``管理，类比文件描述符``fd``。

### II 系统调用-shmctl

&emsp;&emsp;``ctl``的意思是``control``，控制。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420213548.png" style="zoom:80%;" />

&emsp;&emsp;返回值：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422160014.png" style="zoom:80%;" />

&emsp;&emsp;对``cmd``参数，目前我们只用``IPC_RMID``，它的含义是根据shmid删除共享内存。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420213812.png" style="zoom:80%;" />

&emsp;&emsp;那后面那个参数是什么呢？

&emsp;&emsp;看man里头有一个``shmid_ds``，它就是用户层描述``shm``的结构：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220420213608.png" style="zoom:80%;" />

&emsp;&emsp;所以这些``cmd``的参数其实也比较好理解了：

``IPC_STAT``:从内核拷贝共享内存的状态到这个输出型参数中。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422160509.png)

``IPC_SET``:根据你的``shmid_ds``来设置这个共享内存的状态。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422160606.png" style="zoom:80%;" />

&emsp;&emsp;不过我们并不打算设置共享内存的属性，因为这是一件成本比较高的事情，我们不如内核懂共享内存的属性。

&emsp;&emsp;所以删除时，``cmd``设为``IPC_RMID``，属性不关心设为``NULL``即可，下面做一个小``lab``。

&emsp;&emsp;注意，我们在``cmd``后面增加的``0666``表示我们设置的该共享内存的权限。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422162219.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422161116.png" style="zoom:80%;" />

&emsp;&emsp;下面我们通过一个命令行脚本来一直监视``shm``,配合来看看``shmid``：

```shell
while :; do ipcs -m; sleep 1; echo "#################################"; done
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422161539.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422161552.png" style="zoom:80%;" />

&emsp;&emsp;注意返回的``shmid``，我们一次一次运行从0变1，从1变2，从2变3，很像数组下标，可能内核组织共享内存资源时，也是通过数组织的。

&emsp;&emsp;所以共享内存也是可以通过文件权限来约束的，这又体现了一切皆文件。

## 5 挂接共享内存—shmat

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422165430.png" style="zoom:80%;" />

&emsp;&emsp;第一个参数是``shmid``，用来唯一标识去找到那个共享内存，第二个参数表示你的共享内存想要挂接到什么地址范围，我们这点上肯定没有操作系统懂，所以直接设置为``NULL``,最后一个参数我们也暂时不管，设置为0。

&emsp;&emsp;man手册根据字段找信息在哪：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422165735.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422165809.png" style="zoom:80%;" />

&emsp;&emsp;如果挂接成功，则返回挂接的共享内存的起始地址，我们这个地址是给进程用的，所以显然这个地址一定是虚拟地址。

&emsp;&emsp;而我们在C语言中学习过``malloc``，它返回的是申请的堆空间的其实地址，显然这个地址也一定是虚拟地址，这里shmat挂接的共享内存的地址和malloc的返回值含义是一样的，具体大小就是那块共享内存的大小。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422170437.png" style="zoom:80%;" />

## 6 取消挂接共享内存—shmdt

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422170516.png" style="zoom:80%;" />

&emsp;&emsp;它并不是释放共享内存，而是取消当前进程与这个共享内存的关系。

&emsp;&emsp;关注第二个函数，其参数就是那个虚拟内存，这里不必传给它大小，因为共享内存的大小在``shmid_ds``里头存着，不需要我们去告知。

&emsp;&emsp;其返回值返回0表示取消挂接成功，返回-1表示失败。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422171134.png" style="zoom:80%;" />

&emsp;&emsp;进行一个简单的实验看看，依然是运行``sever``然后挂看共享内存的shell脚本：

创建共享内存并挂接共享内存：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422171346.png" style="zoom:80%;" />

去挂接共享内存：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422171416.png" style="zoom:80%;" />

释放共享内存：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422171505.png" style="zoom:80%;" />

## 7 利用共享内存的serve-client简易模型

``sever.c``

```cpp
#include "comm.h"

int main()
{
    // 获取key值
    key_t key = ftok(PATH_NAME, PRO_ID);
    if (key < 0) {
        perror("ftok fail\n");
        exit(1);
    }
    // 创建共享内存
    // int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL);
    // 创建全新的共享内存 权限是666
    int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0) {
        perror("shmget error\n");
        exit(2);
    }
    printf("key : 0x%x shmid : %d\n", key, shmid);

    // sleep(10);
    // 挂接到这个共享内存上
    char* shmp = (char*)shmat(shmid, NULL, 0);
    printf("sever process attaches success\n");
    sleep(15);

    // 在这里就可以进行IPC了

    // 去挂接
    shmdt(shmp);
    sleep(5);
    printf("sever process detaches success\n");

    shmctl(shmid, IPC_RMID, NULL);

    printf("key : 0x%x shmid : %d -> delete success\n", key, shmid);
    sleep(10);
    return 0;
}

```

``client.c``

```cpp
#include "comm.h"

int main()
{
    // 获取key值
    key_t key = ftok(PATH_NAME, PRO_ID);
    if (key < 0) {
        perror("ftok fail");
        exit(1);
    } 
    // 获取shmid client不用新建共享内存 client只需要获取即可
    int shmid = shmget(key, SIZE, IPC_CREAT);

    if (shmid < 0) {
        perror("shmget fail");
        return 2;
    }
    
    // 挂接
    char* shmp = (char*)shmat(shmid, NULL, 0);
    sleep(5);
    printf("client process attaches success\n");

    // 通信区域
    

    // 去挂接
    shmdt(shmp);
    sleep(5);
    printf("client process detaches success\n");



    return 0;
}
```

&emsp;&emsp;这里把``sever.c``的挂接时间弄长一点，我们希望看到``sever``和``client``同时挂接上。

``sever``挂接这块内存：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422173917.png" style="zoom:80%;" />

``client``挂接上这块共享内存

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422173707.png)

``client``去挂接

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422173729.png" style="zoom:80%;" />

``sever``去挂接

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422173758.png" style="zoom:80%;" />

&emsp;&emsp;我们写一个简单的通信进程：

``sever.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422175003.png" style="zoom:80%;" />

``client.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422175106.png" style="zoom:80%;" />

虽然cilent没写入，但是sever还是在读取：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422175459.png" style="zoom:80%;" />

client开始写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422175542.png" style="zoom:80%;" />

&emsp;&emsp;我们这里都没有调用read和write这些系统调用接口来让我们的sever和client去读，共享内存一旦创建并挂接后，就如同``malloc``出来的空间一样，不需要任何系统调用接口来操作。

&emsp;&emsp;而管道需要read和write的本质是要把数据从内核拷贝到用户或从用户拷贝到内核。

&emsp;&emsp;当``client``没有启动的时候，``sever``也在直接读取共享内存，并没有等待``client``写入。

&emsp;&emsp;总结出共享内存的三个特征：

- 共享内存的生命周期随内核，需要程序原自己申请、释放。
- 共享内存是所有的进程间通信中速度几乎最快的；
- 共享内存不提供任何同步或互斥机制，需要程序员自行保证数据的安全，在多线程通信中不太安全。

## 8 共享内存size的问题

&emsp;&emsp;共享内存的大小建议是4096的整数倍，但是我们很调皮的把size设置为了4097，但是好像没有出现什么问题。

&emsp;&emsp;那么为啥呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422180832.png" style="zoom:80%;" />

&emsp;&emsp;共享内存在内核中申请的基本单位是**页**，是**内存页**（``4kb``）。

&emsp;&emsp;如果我申请4097个字节，那么内核其实给我了``4096 * 2byte``，实际就给了``8kb``的共享内存。

&emsp;&emsp;但是我们查的时候，共享内存就是``4097kb``啊？怎么回事呢？

&emsp;&emsp;我要4097个字节，操作系统实际给了我们4096字节，这肯定是错误的，因为这可能导致我们越界访问；

&emsp;&emsp;我要4097个字节，操作系统实际给了我们``4096 * 2``个字节，这并没有错误啊，但是如果我们有需求，越界4097时希望抛异常，然而如果直接显示给你了4096 * 2个字节，你就可能去怪os为啥不给我正好4097个字节呢，所以它还是要显示成给了4097个字节，防止你归因操作系统。

&emsp;&emsp;所以为了避免这种奇怪的错误，创建共享内存的大小建议是4kb的整数倍。

## 9 struct shmid_ds的一些细节

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422181723.png" style="zoom:80%;" />

&emsp;&emsp;第一个元素是一个结构体，是``ipc``的属性，里头有创建者的id，所属者的id，里头也有``__key``，就是我们那个key值。

&emsp;&emsp;看看消息队列的接口：``man msgget``:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422182030.png" style="zoom:80%;" />

&emsp;&emsp;它也需要key值，``msgflg``和我们共享内存的``shmflg``也比较像。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422182203.png" style="zoom:80%;" />

&emsp;&emsp;发现它的两个结构体和共享内存十分类似，信号量``sem``也是一样的，

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422182309.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422182332.png" style="zoom:80%;" />

&emsp;&emsp;发现这三者创建接口类似，并且描述他们的数据结构的第一个类型是完全一样的。

&emsp;&emsp;在内核中，所有的ipc资源都是通过数组组织起来的，所有的System V标准的IPC资源，``XXXid_ds``结构体的第一个成员都是``ipc_perm``(一样的)。

&emsp;&emsp;它们在内核中的组织存储方式如图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422194701.png" style="zoom:80%;" />

&emsp;&emsp;所以为什么我们看到的shmid是0,1,2,3...呢，这就是它的原因。

# 七、消息队列

&emsp;&emsp;操作系统给上层用户提供的一种队列结构，由于比较过时，我们就不做讲解了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422194957.png" style="zoom:80%;" />

# 八、认识信号量

&emsp;&emsp;首先介绍五个概念，什么是临界资源？什么是临界区？什么是原子性？什么是互斥？什么是同步？

信号量的申请：``semget``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422195438.png" style="zoom:80%;" />

信号量的控制：``semctl``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422195506.png" style="zoom:80%;" />

信号量的PV操作：``semop``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422195607.png" style="zoom:80%;" />

&emsp;&emsp;那么什么是信号量呢？

&emsp;&emsp;管道（匿名或命名）、共享内存、消息队列都是以传送数据为主要目的的，但是信号量不是以传输数据为目的的，通过共享“资源”的方式，来达到多个进程的同步和互斥的目的。

&emsp;&emsp;信号量的本质是一个计数器，类似``int count;``，衡量临界资源中的资源数目。

- 什么是临界资源？

&emsp;&emsp;凡是被多个执行流同时能够访问的资源，就是临界资源！比如我们同时向显示器打印，这个显示器就是邻接资源。再比如我们进程间通信时，管道、共享内存、消息队列等都是临界资源。

&emsp;&emsp;凡是进程间通信，必须引入能被多个进程都能看到的资源，同时这也就引入了一个新的问题：**临界资源问题**。

- 什么是临界区？

&emsp;&emsp;进程的代码中用来访问临界资源的代码就称为临界区，如访问共享内存、访问管道等的代码。

&emsp;&emsp;假设我们去电影院看电影，理想情况下，并不是我坐到那个座位上以后，那个座位才属于我，而是在我买票的时候，就已经把那个座位预订好了。

&emsp;&emsp;所以买票行为就是临界资源的预订机制，假设电影院生意很好，一个电影院最怕的就是座位挤兑情况，即不能只有100个座位却卖出110张票，在临界资源的管理中，约束它的东西就是信号量，信号量是一个**计数器**。

```cpp
int count = 100;
if (count > 0) count--;//一个人买了张票
else //等待
    
// 看电影

count++;//从电影院离开
```

- 什么叫做原子性呢？

&emsp;&emsp;一件事情要么不做，要么做完，没有中间态，就叫做原子性。

&emsp;&emsp;当做某件事时，认为可以存在中间状态，就称为非原子性。

```cpp
int count = 100;// 假设父子进程都能看到这个count

// 多进程 父进程和子进程都进行下面的操作
count--;
```

&emsp;&emsp;这个``count--``是具有原子性的吗？

&emsp;&emsp;这个``count--``的行为不是原子性的，原因如下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422203720.png" style="zoom:80%;" />

&emsp;&emsp;所以前面的买票过程是非原子的，任何时刻都可能被中断，从而导致买票多买了的问题。

&emsp;&emsp;所以每个人想进入电影院，就要先对``count--``,前提是每个人都看的到``count``。

&emsp;&emsp;所以``count``本身也是临界资源！

&emsp;&emsp;发现了一个奇葩的事情，信号量是用来保护临界资源的，然而信号量本身就是临界资源，它怎么保护它自己呢？

&emsp;&emsp;所以信号量的--和++必须是安全的，即信号量的--和++操作是原子性的，这就是它的P操作和V操作。

&emsp;&emsp;P操作：对计数器进行--；V操作：对计数器进行++；这两个操作在OS内部被设置为了原子性的。

&emsp;&emsp;所以信号量大概是这样的逻辑：你要访问临界资源，就一定要先申请信号量，相当于一个预订机制，而不同的进程都要申请信号量，所以信号量是临界资源，为了使信号量自己得以被保护，所以信号量的P操作和V操作都是原子的。

- 什么是互斥？

&emsp;&emsp;在任意一个时刻，只能允许一个执行流进入临界资源，执行他自己的临界区，就好像自习室中挂了个牌子，里头有人在自己，你就没法进去了。

&emsp;&emsp;考虑下面图中场景，蓝色为第一个执行流，红色为第二个执行流。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220422210252.png" style="zoom:80%;" />

&emsp;&emsp;这种一般被称为二元信号量，又称互斥锁。

