- 本人的LeetCode账号：[魔术师的徒弟](https://leetcode-cn.com/u/mo-zhu-shi-de-tu-di/)，欢迎关注获取每日一题题解，快来一起刷题呀~
- 本人Gitee账号：[路由器](https://gitee.com/router123456)，欢迎关注获取博客内容源码。
@[toc]
# 一、冯诺依曼体系结构

![](https://img-blog.csdnimg.cn/img_convert/a81c4685392e61c37ec2b004db948bde.png)

1. 输入设备：键盘，磁盘，网卡，显卡，话筒，摄像头等。
2. 输出设备：显示器，磁盘，网卡，显卡，音响等。
3. 存储区：内存。
4. 运算器和控制器：CPU。

&emsp;&emsp;离CPU越近的设备，存储和读写的效率越高，但是单价成本也会更贵：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220226163715.png" style="zoom:80%;" />

&emsp;&emsp;比如几百个G的磁盘，是几百块钱，然而16个G的内存的价格就是几百块钱。

&emsp;&emsp;同样的，CPU的效率也显著的高于其他部分，CPU的效率是纳秒级别的，输入输出设备的速度是秒或毫秒级别的。

&emsp;&emsp;如果直接使CPU和外设进行交互，根据木桶效应，那么计算的速度只能取决于外设的速度，会特别慢，幸好有了存储器这一设备，也就是内存，CPU可以只和内存打交道，内存和其他外设进行交互，这样计算机计算的速度仅取决于内存的速度，效率相当不错。

&emsp;&emsp;我们把输入设备的内容读取到内存的过程称为``input``,把内存中的数据写到输出设备的过程称为``output``,两个过程加起来就称为``IO``.

&emsp;&emsp;网络也可以把数据写到内存中，也可以接收内存的数据，也属于一种``IO``设备。

&emsp;&emsp;如打开网络聊天软件交互，发一条消息，一定是外设(键盘)到内存，内存到外设(网卡)，外设再到另一台机器的内存，内存再到输出设备(显示器)

&emsp;&emsp;任何外设，在数据层面，都优先和内存交互;CPU在数据层面也优先和内存打交道。

&emsp;&emsp;电脑启动的本质就是把软件对应的代码加载到内存中。

# 二、操作系统的概念

## 1 OS是什么？

&emsp;&emsp;操作系统是一款软件，是一款专门针对软硬件进行管理工作的软件。

## 2 OS设计的目的是什么？

&emsp;&emsp;对下即硬件软件层面，管理好软硬件资源。对上即用户层面，**给用户程序提供提供稳定的，高效的，安全的运行环境**。

&emsp;&emsp;前者是**方式**，后者是**目的**。

## 3 OS怎样实现自己的目的呢？

&emsp;&emsp;它的方式是“管理”，根据我微不足道的管理科学知识，所谓的管理主要分为“决策”和“执行”。管理就要分为管理者和被管理者，如校园中，校长等人是管理者，作为学生的我们一般是被管理者。

&emsp;&emsp;管理者和被管理者一般不需要直接打交道，那管理者是怎么管理被管理者的呢？

&emsp;&emsp;管理的本质就是对被管理者做各种决策，那怎么做决策呢？也不需要直接交互，直接有对应的数据就可以做管理了。

&emsp;&emsp;但是管理者和被管理者不直接打交道，你的数据是怎么被管理者知道的呢？

&emsp;&emsp;科学管理的观念下，是执行者获得了你的**数据**，执行者是在执行管理者的数据决策。

&emsp;&emsp;那么基于操作系统就是管理者的情况下，被管理者是各种硬件，那么**操作系统获得各种硬件的数据的东西就是驱动程序**。

&emsp;&emsp;为了管理方便，在操作系统的视角下，应该设计一个结构体来把被管理者的数据给放到一起。

&emsp;&emsp;但是如果直接开一个数组来存结构体，在删除数组中某元素的时候，其实比较麻烦，所以Linux下存储这些结构体的数据结构就是双链表。

&emsp;&emsp;所以对操作系统的管理就是存储被管理者们的各种信息的结构体组成的数据结构进行增删查改的过程！

&emsp;&emsp;增加或删除后让对应的执行者去操作即可，如要删除一个被管理者，则先让执行者去删除对应被管理者，并且让数据结构删除这个对应的结构体。

&emsp;&emsp;总而言之，对被管理对象，我们的做法都是**先描述它的属性选取一些对应数据(选择一种特定的结构体)**，再**组织(选择一种特定的数据结构)**，即**先描述，再组织**的管理理念。

&emsp;&emsp;所以对于进程来说，描述进程也有一种结构体—PCB(进程控制块)。

&emsp;&emsp;所以前面的一系列的逻辑描述都是为了描述为什么要有PCB.



## 4 什么是驱动程序

&emsp;&emsp;那么操作系统是怎么获得底层硬件的具体数据呢？操作系统是通过和驱动程序和对应底层硬件合作的，驱动程序会去和底层硬件直接打交道。

## 5 系统调用是什么

&emsp;&emsp;对于普通用户，操作系统仅提供一个稳定的运行环境即可，普通用户在这个层面上使用各种软件即可；对于软件开发者，操作系统应该提供一系列更多的基础功能——**系统调用**。

&emsp;&emsp;逻辑很简单，操作系统在管理硬件，如硬盘和显示器，当你要写一些函数时，实现的功能是要使硬件改变状态，如要读写磁盘，那么你的操作就一定不能绕开操作系统，一定是操作系统给你提供接口让你去通过接口去访问硬件。

&emsp;&emsp;操作系统不信任任何用户，这一点和银行非常类似，银行的保安和防弹玻璃是来防危险客户的。

&emsp;&emsp;银行通过各种柜台来完成你的服务请求，操作系统也是类似的，操作系统提供**系统调用**接口去完成某些需要使用操作系统管理的软硬件资源的请求。

&emsp;&emsp;这个系统调用接口，直白点说就是函数，而Linux是C语言编写的OS，提供的就是一些C函数接口！

## 6 系统调用与普通的语言库函数的关系

&emsp;&emsp;通常系统调用接口通常使用起来会复杂一些，为了让开发者能以更低的成本调用系统资源，有一些开发者对系统调用的接口进行软件封装以第三方库的形式呈现出来，有些库就直接以语言的标准库呈现出来。

&emsp;&emsp;所以语言库函数(访问软硬件资源的操作)和系统调用的关系是上下层关系。



<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308183835.png" style="zoom:80%;" />

# 三、进程的概念

&emsp;&emsp;进程就是加载到内存中的可执行程序，由程序的代码和内核中的有关进程的代码组成。

## 1 为什么要有进程控制块

&emsp;&emsp;系统中是可能存在很多进程的，操作系统明确有义务来管理进程，管理的方式就是我们前面说的：先描述，再组织。

&emsp;&emsp;任何进程在形成之时，操作系统都要为进程创建进程控制块PCB，其中包含进程的所有属性。

## 2 进程控制块

&emsp;&emsp;在Linux系统中，进程控制块是一个结构体—>``struct task_struct``,其中包含了进程的所有属性。

## 3 进程的一个小测试

&emsp;&emsp;Windows中，可以打开任务管理器查看进程，Linux中，可以用``ps axj``指令来查看进程信息，我们补充上``grep``行过滤工具来查看：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301194654.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301194510.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301194558.png" style="zoom:80%;" />

## 4 进程与程序

&emsp;&emsp;程序本质其实就是一个文件，可执行程序的执行的过程就是把程序加载到内存中且为这个进程创建一个进程控制块，从这个角度上来看，**进程就是程序文件内容再加上操作系统创建的与进程相关的数据结构**，在``linux``中，也就是加上一个``task_struct``。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301200121.png" style="zoom:80%;" />

**CPU和进程交互**

&emsp;&emsp;一个运行队列，其中的元素都是``PCB``，CPU通过进程控制块就找到了进程的所有属性，然后它还会指向那个程序指令集和和数据，CPU通过这个进程控制块就可以运行对应的程序了。

&emsp;&emsp;总结：有了进程控制块，所有的进程管理任务与进程对应的程序毫无关系，而与内核创建的对应进程的PCB强相关。

&emsp;&emsp;总而言之，**进程=程序+操作系统维护的进程相关的数据结构**。

## 5 PCB的组成

- 标示符: 描述本进程的唯一标示符，用来区别其他进程，即pid
- 状态: 任务状态，退出代码，退出信号等。 
- 优先级: 相对于其他进程的优先级。 
- 程序计数器: 程序中即将被执行的下一条指令的地址。 
- 内存指针: 包括程序代码和进程相关数据的指针，还有和其他进程共享的内存块的指针 
- 上下文数据: 进程执行时处理器的寄存器中的数据
- I／O状态信息: 包括显示的I/O请求,分配给进程的I／O设备和被进程使用的文件列表。 
- 记账信息: 可能包括处理器时间总和，使用的时钟数总和，时间限制，记账号等。 
- 其他信息

### I 标识符pid

&emsp;&emsp;标识符是一个唯一标识一个进程的有符号数字。

&emsp;&emsp;下面实现一个能够查看``pid``的程序，先试用man查看一下``getpid``

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301201149.png" style="zoom:80%;" />

实现程序：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301202447.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301201621.png" style="zoom:80%;" />

``kill -9 pid``发送9号信号干掉``pid``对应进程

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301201706.png" style="zoom:80%;" />

``getppid``可以查看父进程的``pid``，在命令行上运行的程序其父进程一般都是``bash``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301202329.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301202200.png" style="zoom:80%;" />

### II 状态

&emsp;&emsp;状态一行中，退出码是被父进程获得的一个有符号数字，即C/C++程序主函数最后的``return``后面的那个数字，可以使用``echo $?``获得最近一次进程结束的退出码。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301202753.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301202720.png" style="zoom:80%;" />

&emsp;&emsp;任务状态：进程所处的一些状态，后续我们会通过PCB所处的位置来进一步理解任务状态。

### III 优先级

&emsp;&emsp;当进程数量远大于CPU数量时，一些进程同时需要访问CPU等其他硬件资源时，那么如何安排谁先使用CPU呢？这就需要一个优先级来确定谁先谁后。

&emsp;&emsp;权限和优先级的区别：权限决定的是能与不能的问题，优先级的前提是已经“能”了，然后操作的先后顺序。

### IV 程序计数器

&emsp;&emsp;程序中即将被执行的下一条指令的地址，也就是正在执行的执行的指令的下一条指令的地址，被称为``pc``指针，有了它才能完成程序的对应逻辑运行。

### V 内存指针

&emsp;&emsp;通过内存指针可以找到进程对应程序的代码和数据。

### VI IO状态

&emsp;&emsp;当前进程是否在使用输入输出设备的信息。

### VII 记账信息

&emsp;&emsp;操作系统中有一个调度模块，其要求为较为均衡的调度每个进程，意思就是每个进程相对平等的获得CPU资源，让进程能正常执行。

&emsp;&emsp;所以较为均衡的调度每个进程呢，可以把当前进程已经使用CPU的时间给表示出来，即为记账信息，然后调度模块通过记账信息来均衡的调度CPU资源。

## 6 如何理解上下文

![](https://img-blog.csdnimg.cn/img_convert/d36806094656d8bf0d7c5ab3889d6caf.png)

&emsp;&emsp;因此进程是需要切换的，但是进程在运行的时候可能有很多临时数据，它们可能存在寄存器里，可是在切换的时候我要把寄存器的内容弄成别人的内容，弄到我的时候如果我本来的临时数据丢了，可能导致无法正常运行进程。

&emsp;&emsp;所以需要把这些临时数据保存起来，这个过程叫做：**保护上下文**，当又轮到我这个进程运行的时候，我需要把临时数据拿出来，这个过程就是**恢复上下文**。

&emsp;&emsp;保护上下文和恢复上下文的目的是让你去做其他事情，当回来继续时，能够继续往下做这些事情。

&emsp;&emsp;所以当某一进程A运行到了时间片后，剥离进程A的同时，把本进程的临时数据（如寄存器中的数据）保存起来，就是保存上下文，然后把这个进程A剥离掉，然后下一个进程B进入CPU，CPU先获得B进程的临时数据，恢复B进程的上下文，这样就能继续上次的状态继续运行。

&emsp;&emsp;进程的切换的核心技术点就是上下文的保存和恢复。

# 四、查看进程的方案

&emsp;&emsp;第一种方案：利用``ps``指令。

&emsp;&emsp;``ps ajx | grep "进程的某些信息"``

&emsp;&emsp;小技巧：vim批量化注释:

&emsp;&emsp;命令模式下：``ctrl v``,然后用``h j k l``移动光标选中区域(l是右移动 j是下移动 k是上移动 h是做移动) ，然后按大写``I``，然后输入``//``,然后``Esc``，就完成了批量化注释。

&emsp;&emsp;批量化删除：``ctrl v``,然后同样选中区域，然后按小写``d``就批量化删除了。

&emsp;&emsp;第二种方案：

&emsp;&emsp;Linux中，有一个文件夹``/proc``，是Linux预留的给我们查看进程的目录，不过这个目录其实不是实时在磁盘读写的，其实目录都是计算机启动起来以后直接读取到内存里头的，目录结构都是提前读到内存中的。

&emsp;&emsp;这个目录允许我们以文件的方式查看进程。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301223128.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301223434.png" style="zoom:80%;" />

&emsp;&emsp;这些蓝色字体的数字目录就是一个进程的目录，数字就是其``pid``。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301223631.png" style="zoom:80%;" />

&emsp;&emsp;``ls /proc/pid -al``，以目录中的文件的形式详细查看查进程的有关信息。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301213940.png" style="zoom:80%;" />

&emsp;&emsp;``exe``表明我们的进程当前正在执行谁，即进程对应的可执行程序是谁。

&emsp;&emsp;``cwd``：``current work director``，含义是当前工作路径，它储存着进程当前运行所在的工作路径，如我们在程序中使用C语言的文件操作创建文件，如果使用相对路径，程序得知要创建文件的位置起始就是靠的当前运行路径的相对路径创建的。

&emsp;&emsp;当结束对应进程后，查询对应的进程目录发现这个目录也就没了。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301214137.png" style="zoom:80%;" />

# 五、Linux中创建进程的函数

## 1 初识fork

&emsp;&emsp;Linux中，使用fork函数（系统调用）可以创建子进程，头文件``<unistd.h>``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301224926.png" style="zoom:80%;" />

&emsp;&emsp;怎么验证它fork后有两个进程呢？

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301225131.png" style="zoom:80%;" />

&emsp;&emsp;如果只有一个执行流，那么这个函数if和else应该只能进一个，从而只能打印一个指令，但是实际上：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301225232.png" style="zoom:80%;" />

&emsp;&emsp;说明现在有两个运行流在程序里，一个运行了上面那个，另一个运行了下面那个，不然没法解释为啥if和else都进入了。

&emsp;&emsp;下面这段程序：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301225718.png" style="zoom:80%;" />

&emsp;&emsp;运行结果每次都会打印两行东西出来：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220301225830.png" style="zoom:80%;" />

&emsp;&emsp;说明当前有两个执行流在执行。

## 2 调用fork创建子进程

&emsp;&emsp;不退出vim执行命令：底行模式下使用``! + 命令``。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308192531.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308192728.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308192823.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308192053.png" style="zoom:80%;" />

&emsp;&emsp;如何理解fork创建子进程？

&emsp;&emsp;我们目前创建进程的方式：./可执行程序，命令，fork，这些操作在操作系统角度都是一样的。

&emsp;&emsp;fork本质是创建进程，最终导致的结果一定是系统中多了一个进程，也就是与进程相关的**内核数据结构**和**进程的代码和数据**在系统中多了一份，即多了一份``task_struct``.

&emsp;&emsp;这里就有一个疑惑了，我们只是调用了fork，创建了一个子进程，**那么子进程的代码和数据在哪呢**？

&emsp;&emsp;默认情况下，**子进程会"继承"父进程的代码和数据**。

&emsp;&emsp;内核中的**子进程的数据结构**(也就是那个双链表结点中的值)也**会以父进程为模板来初始化**自己的``task_struct``.

&emsp;&emsp;我们现在关注一个点：fork之后子进程和父进程的代码是共享的，子进程把父进程的代码都继承了，前面也有，但是程序计数器也继承了，所以子进程和父进程都从fork往下运行，如果我们可以修改子进程的``pc``指针的值，是可以让自进程重新从头开始执行的。

&emsp;&emsp;代码在常量区，是不可修改、只读的，因此出于方便考虑，父子代码只有一份。

&emsp;&emsp;那么父进程的数据呢？在默认情况下，子进程和父进程的数据也是“共享”的，不过要考虑需要修改数据的情况，

&emsp;&emsp;简单讨论一下，进程是有独立性的，即便我们是父子进程，你挂了也不会影响我。

&emsp;&emsp;数据通过写时拷贝来完成了父子进程之间的独立性，也就是在咱俩都不修改数据时，这份数据只有一份，当其中一者打算修改数据时，操作系统会干涉一过程，操作系统会先重新把数据拷贝一份，然后让你改拷贝后的数据。

&emsp;&emsp;因此代码共享并不会影响进程的独立性，因为运行时代码不可以被修改，是只读的。

&emsp;&emsp;进行写时拷贝是为了节约空间，提高效率。

## 3 fork的返回值

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308194358.png" style="zoom:80%;" />

&emsp;&emsp;``pid_t``可以理解成一个整数。

&emsp;&emsp;它的作用是让不同的执行流（父进程和子进程）执行不同的事情，因为如果不能区别子进程和父进程执行同样的程序那么有什么必要创建子进程呢？所以``fork``的返回值就是来干这个事情的。

&emsp;&emsp;fork失败返回<0的负数；fork成功给父进程返回子进程的``pid``，给子进程执行流返回0.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308194622.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308194703.png" style="zoom:80%;" />

1、如何理解有两个返回值？

```cpp
pid_t fork()
{
    // 创建子进程的逻辑
    return XXX;
    // 走到这里子进程都创建完了 那父进程和子进程返回两个数据也不奇怪了
}
```

&emsp;&emsp;接受``fork``返回值的``ret``是数据，两个执行流都会返回，这两个返回值是不同的，就发生了写时拷贝，所以两个执行流一人一份。

2、如何理解这两个返回值的设置？

&emsp;&emsp;任何一个进程，其父进程是唯一的，但是父进程的子进程可以有多个，因此父进程如果要找父进程需要子进程的``pid``，而子进程不需要特地持有父进程的``pid``来找父进程，因为子进程只有一个父进程，可以直接``getppid()``获得。

## 4 多进程代码初识

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308200429.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308200348.png" style="zoom:80%;" />

&emsp;&emsp;fork之后，父子进程谁先执行？答案是不确定的，和CPU平台有关。

# 六、进程执行状态

## 1 各种状态的概念

&emsp;&emsp;很多OS的书对这块的状态讲的很玄乎，因为它们需要做到各个操作系统都适配，所以就很抽象。

&emsp;&emsp;所以我们现在就具体的看看Linux的内核的进程执行状态。

&emsp;&emsp;内核源码中是这么写的：

```c
/*
* The task state array is a strange "bitmap" of
* reasons to sleep. Thus "running" is zero, and
* you can test for combinations of others with
* simple bit tests.
*/
static const char * const task_state_array[] = {
"R (running)", /* 0 */
"S (sleeping)", /* 1 */
"D (disk sleep)", /* 2 */
"T (stopped)", /* 4 */
"t (tracing stop)", /* 8 */
"X (dead)", /* 16 */
"Z (zombie)", /* 32 */
};
```

&emsp;&emsp;首先每个进程的状态信息具体在``task_struct``中。

&emsp;&emsp;进程状态的意义：方便操作系统快速判断进程，完成特定功能，比如调度，本质是完成一种分类。

&emsp;&emsp;具体的状态：

&emsp;&emsp;**R(Running)：运行状态**。处于运行状态的进程一定正在占有CPU吗？不一定，它可能处于运行队列中正在排队。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308201403.png" style="zoom:80%;" />

&emsp;&emsp;**S睡眠状态 (sleeping)和D磁盘休眠状态 (disk sleep)**：当我们想完成某种任务，但是任务条件不具备，需要进程进行某种等待时，就处于S或D状态。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308202210.png" style="zoom:80%;" />

&emsp;&emsp;所有的进程，在运行的时候，可能由于运行需要，可以放在不同的队列中，在不同的队列中，所处状态不同。

&emsp;&emsp;所以程序卡住了可能就是资源不到位（比如网络下载不到数据），这个程序处于等待队列中，CPU不跑它。

&emsp;&emsp;我们把处于运行状态的``task_struct``（即在``run_queue``中）放到等待队列中，这被称为**挂起等待（阻塞）**。

&emsp;&emsp;从等待队列被放到运行队列中，被CPU调度称为**唤醒进程**。

S状态进程示例：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308221119.png" style="zoom:80%;" />

&emsp;&emsp;这个进程就是处于经典的等待状态，不断的等待``sleep``结束。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308221241.png" style="zoom:80%;" />

&emsp;&emsp;处于S状态的进程是可以直接被``ctrl + c``中断的，是一种可中断睡眠，是一种浅度睡眠。

D状态示例：

&emsp;&emsp;D状态是一种不可中断睡眠，即所谓的深度睡眠，下面是它的一个例子：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308205933.png" style="zoom:67%;" />

&emsp;&emsp;显然这个事情的根源在于操作系统干掉了等待磁盘回复的进程。

&emsp;&emsp;因此操作系统设置了一个D状态，**如果进程处于D状态，则此进程不可被杀掉（操作系统都不能杀）**。

&emsp;&emsp;因此当此进程被唤醒时，磁盘的回复来了，就一切正常了。

&emsp;&emsp;D状态深度睡眠，不可被杀掉。

&emsp;&emsp;D状态不太好模拟，容易把服务器挂掉，一般做运维和数据库的经常会见到D状态。

&emsp;&emsp;所以互联网公司的服务器挂掉了可能就是存储磁盘出了问题，大量进程处于D状态，个个都是爷，不能被干掉，越来越卡，就寄了。

**T (stopped)：暂停状态**

&emsp;&emsp;T状态是一种彻底的暂停，不会有任何数据更新，不同于S状态，S状态还会更新自己睡眠了多久的这种数据。

**t (tracing stop)：追踪暂停状态**

&emsp;&emsp;典型的t状态：程序的断点调试。

**X (dead)：死亡状态**

&emsp;&emsp;进程死亡了需要回收它的资源，即进程相关的内核数据结构（PCB）和进程的代码和数据。

&emsp;&emsp;死亡状态很难查到，因为释放资源很快。

**Z (zombie)：僵尸状态**

&emsp;&emsp;为什么要有僵尸状态？为了辨别进程的退出死亡原因，是被操作系统干掉了？还是代码有问题？还是正常退出了？

&emsp;&emsp;所以我们要保存进程退出的信息，这个信息显然也在``task_struct``中保存。

&emsp;&emsp;所以进程退出后不会直接进入死亡状态，而会进入僵尸状态，供父进程和操作系统识别退出原因。

&emsp;&emsp;僵尸进程不可以被kill杀死。

与操作系统书的状态对比：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308230453.png" style="zoom:80%;" />

## 2 状态的验证

**R状态**：

&emsp;&emsp;死循环就是典型的R状态。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308213307.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308212736.png" style="zoom:80%;" />

**S状态**:

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308212429.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308212324.png" style="zoom:80%;" />

&emsp;&emsp;当我们的程序有IO时，但是因为外设的速度太慢了，很多时候IO都在等待外设就绪，所以大部分状态都处于S状态。

**T：暂停状态**

&emsp;&emsp;基本信号操作表：``kill -l``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308212901.png" style="zoom:80%;" />

``kill -19 pid``：向进程发送暂停信号。

``kill -18 pid``:向进程发送继续信号。

&emsp;&emsp;暂停，注意此时的T状态没有``+``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308221641.png" style="zoom:80%;" />

&emsp;&emsp;恢复，因为在输出，所以大部分时间还是处于S状态，此时没有``'+'``，表示它变为**后台程序**了。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308221749.png" style="zoom:80%;" />

&emsp;&emsp;``kill -9 pid``杀掉这个后台程序。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308221842.png" style="zoom:80%;" />

&emsp;&emsp;状态为S+的程序是在前台运行的进程处于睡眠状态。

&emsp;&emsp;直接运行程序一般就是在前台运行，其状态就会加一个+号，此时在前台程序上不能运行其他命令。

&emsp;&emsp;发送继续信号后``+``号会不见，这时变成了后台进程，此时允许我们继续输其他命令，但是不能``ctrl + c``杀掉，此时可以用``kill -9``杀掉。

&emsp;&emsp;运行程序时后面加一个``&``也可以进入后台进程运行模式。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308222708.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308222803.png" style="zoom:80%;" />

**z：僵尸进程**

&emsp;&emsp;如果没有人检测或回收进程（一般是父进程承担此责任），则该进程进入Z状态。

&emsp;&emsp;如何看到这样的进程呢？

&emsp;&emsp;思路就是利用fork()创建子进程，然后子进程一直打印消息，父进程休眠啥也不干，然后``kill``命令杀掉子进程，此时父进程在休眠，不会回收子进程，就会出现子进程是僵尸进程了。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308223149.png" style="zoom:80%;" />

&emsp;&emsp;一个不断检查进程的命令行脚本：

```bash
while :; do ps axj | head -1 &&  ps axj | grep Ztest | grep -v grep; sleep 1; echo "##############################"; done
```

&emsp;&emsp;不加``grep -v grep``的时候：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308223516.png" style="zoom:80%;" />

&emsp;&emsp;加了``grep -v grep``可以不显示那个``grep``本身。

&emsp;&emsp;总体框架如下：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308224255.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308224411.png" style="zoom:80%;" />

&emsp;&emsp;父进程休眠时杀掉子进程，子进程变成僵尸进程，这里出现的``<defunct>``表示无用的，不再存在的，死者，死人。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308224531.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308224820.png" style="zoom:80%;" />

&emsp;&emsp;父进程退出后，子进程被回收。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308224601.png" style="zoom:80%;" />

&emsp;&emsp;父进程正常退出时，操作系统会回收僵尸子进程，但是若父进程一直不退出，但是又不回收子进程，僵尸进程就会造成资源浪费，所以在Linux中我们要想办法及时回收僵尸进程。

&emsp;&emsp;父进程先挂了怎么办，如下代码：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308225638.png" style="zoom:80%;" />

&emsp;&emsp;此子进程会被1号进程（操作系统）领养，此时这个子进程称为孤儿进程，保证父进程离开后的子进程能被正常回收。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220308230143.png" style="zoom:80%;" />

# 七、进程优先级

## 1 基本概念

&emsp;&emsp;CPU资源分配的前后顺序，就是进程的优先级。

&emsp;&emsp;为什么会有优先级？

&emsp;&emsp;因为资源相比进程数量太少，优先级本质是分配资源的一种方式。

&emsp;&emsp;PCB中有个字段就表示的进程优先级。

## 2 查看进程优先级的方案

``ps -al``:查看用户创建的进程的优先级。

Linux中的优先级值越小，优先级越高。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312093714.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312124605.png" style="zoom:80%;" />

&emsp;&emsp;``NI``（NICE）是优先级的修正数据，``NI``可以用来表示优先级变化了多少。

&emsp;&emsp;``PRI``就是优先级，越小优先级越高。

&emsp;&emsp;Linux中修改进程优先级就是修改``NI``的值。

## 3 调整优先级

&emsp;&emsp;有很多调整优先级的方案，如一些调整优先级的接口：``sched_get_priority_max``接口，还有一些命令：``NICE``命令、运行时的``RENICE``命令，但是不经常用，因为我们不比操作系统懂进程的优先级分配。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312124908.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312094305.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312125118.png" style="zoom:80%;" />

实验：``top``命令设置优先级。

&emsp;&emsp;按``top``进入一个界面，然后按``r``，然后输入进程的``pid``:

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312125816.png" style="zoom:80%;" />

&emsp;&emsp;然后回车，新出现的界面中设置``NICE``值即可。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312125909.png" style="zoom:80%;" />

&emsp;&emsp;然后q退出，来看看现在的优先级：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312130032.png" style="zoom:80%;" />

&emsp;&emsp;发现变成了90，那么我们如果再修改一个NICE值为5呢？（如果再次设置出现permission denied，则使用``sudo``）

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312130443.png" style="zoom:80%;" />

&emsp;&emsp;虽然``PRI(NEW) = PRI(OLD) + NICE``，但是``PRI(OLD)``一般默认是80，我们下面修改NICE为-10，再次验证：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312131200.png" style="zoom:80%;" />

**NICE取值范围实验**：

&emsp;&emsp;直接设置``NICE = -100``，发现NICE最多变成``-20``:

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312131456.png" style="zoom:80%;" />

&emsp;&emsp;设置``NICE = 100``，发现NICE最多变成19：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312131628.png" style="zoom:80%;" />

**NICE为何是一个先对比较小的范围呢？**

&emsp;&emsp;优先级再怎么设置，也只能是一种相对优先级，不能出现绝对优先级，就好像排队插队可能导致后面的人长时间无法排队成功，对进程也是一样的，如果能把优先级设置的非常高影响调度器，那么就可能会因此使得部分进程得不到CPU资源，出现“饥饿问题”。

&emsp;&emsp;调度器的目的是为了让每个进程较为均衡的享受到资源。

## 4 进程有关的其他概念

- **竞争性**: 系统进程数目众多，而CPU资源只有少量，甚至1个，所以进程之间是具有竞争属性的。为了高 效完成任务，更合理竞争相关资源，便具有了优先级。
- **独立性**: 多进程运行，需要独享各种资源，多进程运行期间互不干扰 
- **并行**: 多个进程在多个CPU下分别，同一时刻同时运行，这称之为并行，只有单个的CPU的情况并不存在并行。
- **并发**: 多个进程在一个CPU下采用**进程切换**的方式，**在一段时间之内**，让多个进程都得以推进，称之为并发。

# 八、环境变量

## 1 设置第一个环境变量

&emsp;&emsp;首先我们编一个程序，代码如下：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312133132.png" style="zoom:80%;" />

&emsp;&emsp;运行起来：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312133200.png" style="zoom:80%;" />

&emsp;&emsp;我们自己写的程序、命令、工具本质都是一个可以执行的文件，所以我们的``myproc``也是命令。

&emsp;&emsp;那为啥运行我们的命令需要``./myproc``呢，我们知道前面的``./``是路径，那我们的系统命令为啥就不需要路径呢？

&emsp;&emsp;因为系统的命令都是通过环境变量``PATH``找到的。

&emsp;&emsp;``PATH``是一个类似C语言中变量的东西，如果要查看``PATH``的内容，直接``echo $PATH``即可。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312134411.png" style="zoom:80%;" />

&emsp;&emsp;文件夹以``:``分割。

&emsp;&emsp;系统找命令就是从这几个文件夹依次找，找到了就停下跑对应命令，否则停下。

&emsp;&emsp;直接把你的命令变成可以直接执行有两个方法：一个是直接拷贝可执行文件到某个文件夹中，另一个是添加到``PATH``当中。

&emsp;&emsp;非常不推荐第一种设置方法，因为这会污染命名池，第二种方法是内存级的修改，就算改了重启一下又恢复默认了。

格式：``export PATH=路径名``。

&emsp;&emsp;所以不要直接``export PATH=你的路径``，这样直接就把``PATH``设置为只有你的路径了，什么东西都找不到了，不过你可以退出``xshell``重新链接即可。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312135539.png" style="zoom:80%;" />

&emsp;&emsp;重连就好了：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312135618.png" style="zoom:80%;" />

&emsp;&emsp;正确的格式应该是：``export PATH=$PATH:你自己要包含可执行程序的路径``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312135850.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312135910.png" style="zoom:80%;" />

&emsp;&emsp;具体修改登陆后的环境变量还有一个方法：``vim ~/.bash_profile``在这里头改每次登陆的环境变量。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312140218.png" style="zoom:80%;" />

## 2 Windows下的环境变量

&emsp;&emsp;Windows也有环境变量，右击此电脑，打开属性，找到高级属性设置：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312171428.png" style="zoom:80%;" />

&emsp;&emsp;找到``PATH``:

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312171717.png" style="zoom:80%;" />

&emsp;&emsp;双击打开：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312171846.png" style="zoom:80%;" />

&emsp;&emsp;这就是我们的``Windows``下的``PATH``环境变量。

&emsp;&emsp;环境变量的作用很多，比如链接时需要找动态库静态库在哪里，我们一般会通过环境变量来找。

&emsp;&emsp;环境变量具有全局属性，在哪里都可以访问。

&emsp;&emsp;常见的环境变量：

- PATH : 指定命令的搜索路径 
- HOME : 指定用户的主工作目录(即用户登陆到Linux系统中时,默认的目录) 
- SHELL : 当前Shell,它的值通常是/bin/bash。

## 3 HOME环境变量

&emsp;&emsp;存储的是当期用户的默认工作目录。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312173026.png" style="zoom:80%;" />

&emsp;&emsp;不同用户登陆的工作目录不同就是因为每一个用户的``HOME``环境变量不同。

4 SHELL环境变量

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312181633.png" style="zoom:80%;" />

&emsp;&emsp;查看我们的``shell``的版本。

## 4 env命令—看系统中的环境变量数据

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312181853.png" style="zoom:80%;" />

主机名：``HOSTNAME``.

&emsp;&emsp;环境变量的本质就是操作系统在内存/磁盘文件中开辟的空间，用来保存系统相关的数据。

&emsp;&emsp;环境变量中也有变量名和数据，比如说HOME中存的就是当前用户默认工作路径。

## 5 与环境变量有关的命令

- ``echo``:查看环境变量的值，``echo $环境变量名``.
- ``env``:显示所有**环境变量**

&emsp;&emsp;系统上还存在一种变量，是与本次登陆（session）有关的变量，只在本次登陆有效，被称为本地变量。

&emsp;&emsp;如我们可以直接在命令行定义变量：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312183318.png" style="zoom:80%;" />

&emsp;&emsp;重新登陆后就没有了：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312183448.png" style="zoom:80%;" />

&emsp;&emsp;所以写命令行脚本其实就是定义本地变量。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312183739.png" style="zoom:80%;" />

- ``set``：显示**本地变量和环境变量**。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312183904.png" style="zoom:80%;" />

- ``export``：把本地变量加入环境变量（内存级）。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312191213.png" style="zoom:80%;" />

- ``unset``：取消本地或环境变量。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312191012.png" style="zoom:80%;" />

## 6 命令行参数与C程序获取环境变量

&emsp;&emsp;学C语言时我们就知道，``main``函数是可以携带参数的。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312191632.png" style="zoom:80%;" />

&emsp;&emsp;``argv``是个``char*``类型的指针数组，数组元素有``argc``个。

&emsp;&emsp;一段代码打印一下数组内容：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312191937.png" style="zoom:80%;" />

&emsp;&emsp;直接运行的结果：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312192107.png" style="zoom:80%;" />

&emsp;&emsp;携带``-a``、``-b``、``-c``的参数时：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312192215.png" style="zoom:80%;" />

&emsp;&emsp;我们每个空格两端都视为一个字符串，程序获得的参数是``"./myproc" "-a" "-b" "-c" "-d"``.

&emsp;&emsp;``argc``决定有几个有效字符串，这里数组会多一个元素，最后一个元素指向NULL。

&emsp;&emsp;``argc``可以用来控制用户的命令行参数个数。

&emsp;&emsp;为什么需要有命令行参数呢？观察``ls``，带上``-a -l -n``都会有不同的效果，我们的程序如果想要这种效果，命令行参数是不错的实现方法。

&emsp;&emsp;一个简单的程序示例：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312193632.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312193729.png" style="zoom:80%;" />

&emsp;&emsp;选项的底层使用的就是命令行参数，这便是命令行参数的意义。

&emsp;&emsp;实际上，main函数的参数可以有3个，最后一个就是环境变量。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312194416.png" style="zoom:80%;" />

&emsp;&emsp;每个程序都会收到一张环境表，环境表是一个字符指针数组，每个指针指向一个以’\0’结尾的环境字符串，内容就是其父进程的环境变量。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312195738.png" style="zoom:80%;" />

&emsp;&emsp;一个示例程序：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312195227.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312195344.png" style="zoom:80%;" />

## 7 C程序获取环境变量的方法

**I main函数的参数**

&emsp;&emsp;一种就是和上面一样，通过main函数的第三个参数。

```cpp
int main(int argc, char* argv[], char* env[])
```

II extern char** environ 

&emsp;&emsp;另一种获取环境变量的方法是通过系统系统的二级指针``environ``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312195846.png" style="zoom:80%;" />

&emsp;&emsp;这个二级指针其实就是指向那个指针数组的开头元素，所以环境变量的组织结构就是这样的：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312195946.png" style="zoom:80%;" />

&emsp;&emsp;所以原本的程序可以改成这样：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312200316.png" style="zoom:80%;" />

&emsp;&emsp;那如果main函数参数都没有，可以传参给main函数吗？

&emsp;&emsp;答案是可以的，这个参数也会压栈，需要用可变参数列表来获得这个参数。

**III getenv**

&emsp;&emsp;不过获取环境变量的最常见方法还是``getenv``.

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312200842.png" style="zoom:80%;" />

&emsp;&emsp;使用示例程序：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312201238.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312201203.png" style="zoom:80%;" />

&emsp;&emsp;自己设计一个``getenv``也很简单，遍历那个表通过``strstr``找对应字符串的子串，找到了返回即可。

## 8 环境变量的全局属性的原因

&emsp;&emsp;命令行上启动的进程，其父进程都是``bash``，其实调用的就是``fork``.

&emsp;&emsp;所以我们的进程的环境变量是从bash获得的，bash的环境变量是从系统的配置文件中获得的。

&emsp;&emsp;**所以环境变量有全局属性的原因是环境变量会被子进程继承**，本地变量则不能被子进程继承，我们验证如下：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312202708.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312202909.png" style="zoom:80%;" />

&emsp;&emsp;从bash开始，bash设置的环境变量都会被子进程全部继承，所以环境变量可以帮助我们的进程做一些查找功能等。

&emsp;&emsp;我们的环境变量信息存储在``/etc/bashrc``、``~/.bashrc``、``~/.bash_profile``中，后两者是登陆启动信息，第一个是系统默认的环境变量的操作。

&emsp;&emsp;其实``gcc g++ gdb``找文件，找库就是在环境变量的指导下找的。

# 九、程序地址空间

## 1 程序地址空间是物理地址吗

&emsp;&emsp;在学习C/C++时，我们认为C程序的内存空间大致是这样分布的：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312205459.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312211625.png" style="zoom:80%;" />

&emsp;&emsp;一点验证：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312212048.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312212027.png" style="zoom:80%;" />

&emsp;&emsp;堆区和栈区之间地址大量镂空，说明它们之间有大量的空间。

&emsp;&emsp;看下面一个程序：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312213208.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220312213316.png" style="zoom:80%;" />

&emsp;&emsp;这里的原因当然是因为写时拷贝，但是竟然这**俩拷贝了的地址居然没变**。

&emsp;&emsp;如果C/C++打印出来的是物理内存的地址，那么写时拷贝不可能还是同一块地址。

&emsp;&emsp;所以，我们使用**在C/C++的地址绝对不是物理地址，而是虚拟地址**，所以这个空间应该是进程虚拟地址空间。

## 2 进程地址空间是什么

&emsp;&emsp;每个进程都有个进程地址空间，**都认为自己独占了物理内存**，这种OS提供给每个进程的虚拟的“饼”就是进程地址空间。

&emsp;&emsp;而在``Linux``中，描述进程地址空间本质是内存中的一种数据类型，使用的是一个结构体``mm_struct``，每个进程在创建时同时也会定义一个这样的结构体对象，而每个进程的``task_struct``会指向它的``mm_struct``.

&emsp;&emsp;32位下，进程地址空间的大小是4GB。

&emsp;&emsp;那么操作系统为每个进程画的饼：自己独占了物理内存，都可以通过数据的方式做到。

&emsp;&emsp;实际上，``mm_struct``中，我们不必具体的设置一个大数组，只需要设置每个区域的开始位置和结束位置即可。

```cpp
struct mm_struct
{
    unsigned int code_start;
    unsigned int code_end;
    
    unsigned int init_data_start;
    unsigned int init_data_end;
    
    unsigned int uninit_data_start;
    unsigned int uninit_data_end;
    
    unsigned int heap_start;
    unsigned int heap_end;
    
    ...
        
    unsigned int stack_start;
    unsigned int stack_end;
}
```

&emsp;&emsp;虽然这里只有``start``和``end``，但是每个进程都可以认为``mm_struct``代表整个内存，且所有的地址为：``0x00000000 - 0xFFFFFFFF``。

&emsp;&emsp;每个进程都认为地址空间的划分是按4GB的空间划分的。

&emsp;&emsp;地址空间上进行区域划分时，对应的线性位置就是虚拟地址，Linux中也叫作线性地址，即一个``char Buf[4 * 1024 * 1024 * 1024]``。

&emsp;&emsp;操作系统通过**页表**和**MMU (Memory Magment Unit)**（一种硬件，可以查页表）。

&emsp;&emsp;页表是操作系统给每个进程维护的一张表，左侧是虚拟地址，右侧是物理地址，页表的作用是把虚拟地址转化为物理地址。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314194837.png" style="zoom:80%;" />

## 3 为什么需要地址空间

&emsp;&emsp;加了一个中间层是更加适合管理的，如果直接让进程访问物理内存可能出很多问题。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314195334.png" style="zoom:80%;" />

&emsp;&emsp;直接访问物理内存的话进程直接修改其他进程的代码和数据怎么办呢？这就非常难管理。

&emsp;&emsp;如果加上一层中间层：虚拟地址，操作系统来帮你转换虚拟地址，可以预防出现这些访问其他进程区域的数据。

&emsp;&emsp;所以字符常量区不可写的原因就可以认为是操作系统给你的权限只有读，这个权限的管理和页表是有关系的。

&emsp;&emsp;总结一下，需要虚拟内存的第一的原因就是：

- 通过添加一层软件层来完成对进程操作物理内存进行权限管理，本质目的是为了保护物理内存以及各个进程的数据安全。

&emsp;&emsp;一个疑问：我们申请了1000字节，我们立马就能使用1000个字节吗？

&emsp;&emsp;不一定，编程者可能会存在暂时不会全部使用，甚至暂时不使用。站在操作系统角度，如果空间立马给你，是不是意味着整个系统会有一部分空间本来可以给别人立马用的，现在却被你闲置了呢。

&emsp;&emsp;操作系统怎么证明你用了这块空间与否呢？如果有了这块空间，但是你从来不进行读写，那你就是暂时没用这块空间。

&emsp;&emsp;可能你在堆区上申请一个空间，假如我要申请100字节，因为我们没立马读写，它只是进行了``heap_end += 100``,等真正读写时，才给你映射一块对应的物理内存，这就是一种**基于缺页中断进行的物理内存申请**。

&emsp;&emsp;总结一下，第二个理由就是：

- 将内存申请和内存使用的概念在时间上划分清楚，通过虚拟地址空间，来**屏蔽底层申请内存**的过程，达到 **进程读写内存** 和 **OS进行内存管理操作** 做到软件上的分离，比如，操作系统可以把很久不读写的物理内存中的内容先暂时转移到磁盘里头，以保证内存100%时仍然可以申请内存。

&emsp;&emsp;还有第三个理由，比如如果不同进程都从不同的地址作为程序入口，CPU会很恼火，所以CPU可以固定自己读取代码的入口，如是``0x1234``，然后操作系统在加载器把进程代码加载到内存后返回其入口的物理地址（如``0x4321``)，然后给次进程的页表建立一个映射，然后CPU执行页表的这个地址，使用映射获得真实物理地址即可，这样CPU工作量就会变小很多了。

- 站在CPU和应用层的角度，进程可以看做统一使用4GB空间而且每个空间区域的相对位置是比较确定的，这样程序的代码和数据都可以被加载到物理内存的任意位置，反正有页表的映射。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314205715.png" style="zoom:80%;" />

&emsp;&emsp;OS最终这样设计的目的，达到一个目标：每个进程都认为自己是独占系统资源的。

&emsp;&emsp;所以之前的代码得到同样的地址就是因为：子进程创建时，同样创建和父进程类似的PCB MM_STRUCT都类似，所以一开始父进程和子进程都指向同一个数字，所以值也相同，虚拟地址也相同；当子进程写时，发生写时拷贝，然后重新建立页表映射，把那个子进程的虚拟地址映射到新的拷贝后的变量的物理地址。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314210909.png" style="zoom:80%;" />

&emsp;&emsp;在虚拟内存和页表的视角下，代码区共享不过就是两个虚拟地址都映射向同一块物理内存地址罢了。

&emsp;&emsp;所以所有的只读的数据，一般只有一份，这样系统只需要维护一份，成本最低，所以就让两个虚拟地址都相同，这样映射到的物理地址自然也是同一个。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314211215.png" style="zoom:80%;" />

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314211406.png" style="zoom:80%;" />

&emsp;&emsp;命令行参数和环境变量的地址：

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314212244.png" style="zoom:80%;" />

&emsp;&emsp;发现命令行参数在栈上面，环境变量更在命令行参数的上方。

<img src="https://gitee.com/router123456/drawing-bed/raw/master/img/20220314212351.png" style="zoom:80%;" />

&emsp;&emsp;Linux中，有40个运行队列（和-20~19个数对应），这样就可以按下标作为优先级去遍历队列了。


















