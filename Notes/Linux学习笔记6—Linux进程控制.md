# 一、进程创建

## 1 从系统的角度认识fork——fork的写时拷贝

&emsp;&emsp;父进程和子进程都还没有修改数据时，父子进程的虚拟内存、页表、物理内存大致如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319175239.png" style="zoom:80%;" />

&emsp;&emsp;父子进程的一方修改数据后，数据发生写时拷贝，即修改一方的数据会在物理内存中被操作系统重新分配一块地址，然后修改对应进程的页表，进而修改数据不会影响另一个进程，保证进程的独立性。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319175532.png" style="zoom:80%;" />

## 2 fork常规用法

&emsp;&emsp;一个父进程希望复制自己的数据，使得父子进程同时执行同一程序的不同的代码段，例如：父进程等待客户端请求，生成子进程来处理请求；

&emsp;&emsp;一个进程要执行一个不同的程序，例如子进程从``fork``返回后，调用``execl``函数，我们将在本章的进程的程序替换中讲解。

## 3 fork失败的原因

&emsp;&emsp;根据我们前面的学习，创建进程是需要时间和空间成本的。所以当系统中有太多的进程时，fork会失败；每个用户都有创建进程的上限数，当超过上限时，创建进程也不会被允许。

# 二、进程终止

## 1 进程退出场景

&emsp;&emsp;进程总共有三种退出可能：

- 代码运行完毕，结果正确
- 代码运行完毕，结果错误
- 代码异常终止

**代码运行完毕的退出**

&emsp;&emsp;为什么main函数总会``return 0``呢，意义在哪里呢？main函数的return值就是进程的退出码，是用来衡量进程的退出情况与执行结果的。

&emsp;&emsp;``echo $?``输出最近一次的进程退出时的退出码。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319091259.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319091700.png" style="zoom:80%;" />

&emsp;&emsp;意义是用退出码判断进程的退出情况，一般用0表示进程运行完毕正常退出，用非0表示有错误。

&emsp;&emsp;一般进程退出不正确时，有多重可能，所以可以用一堆具体的数字充当一种可能性。

&emsp;&emsp;C语言中就提供了很多和错误码有关接口，如``strerror``函数，可以获得每个数字对应的错误原因。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319092536.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319092851.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319092756.png" style="zoom:80%;" />

&emsp;&emsp;程序运行完，结果不正确的样例：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319093053.png" style="zoom:80%;" />

&emsp;&emsp;也可以自己搞错误码的对应含义，只要你搞一个全局的``char*``数组即可。

**代码异常终止的退出**

&emsp;&emsp;没跑完代码突然被终止的，可能是因为各种异常或收到了什么信号退出的，即程序崩溃(进程异常结束)的一个样例：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319093455.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319093357.png" style="zoom:80%;" />

&emsp;&emsp;一般程序如果崩溃了，那么它的退出码就没有意义了，验证如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319093544.png" style="zoom:80%;" />

## 2 进程退出的方式

- ``main``函数``return``代表进程退出，非``main``函数的``return``仅仅是普通的函数返回。
- ``exit``终止进程，任意位置退出进程，参数代表退出码

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319093927.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094125.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094102.png" style="zoom:80%;" />

&emsp;&emsp;exit内部有很多宏，可以用宏来表示一些退出码，如``EXIT_SUCCESS``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094603.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094646.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094701.png" style="zoom:80%;" />

&emsp;&emsp;代码分析：效果和正常退出一样，这说明``exit``和``return``一样，会要求系统进行进程的各种后续首尾工作，如进行缓冲区刷新。

- ``_exit``强制终止进程，但不进行进程的后续收尾工作，比如刷新缓冲区。

&emsp;&emsp;``_exit``函数：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094749.png" style="zoom:80%;" />

&emsp;&emsp;测试代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319094933.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319095035.png" style="zoom:80%;" />

&emsp;&emsp;这里的缓冲区是“用户级缓冲区".

## 3 进程退出在OS层面做了什么呢 ？

&emsp;&emsp;进程退出，相当于系统层面少了一个进程，那就应该``free`` ``pcb``，``mm_struct``，``free``页表和各种映射关系，代码+数据申请的空间都还给操作系统。

# 三、进程等待

## 1 进程等待是什么？

&emsp;&emsp;``fork``可以创建子进程，子进程的目的是为了帮助父进程完成某种任务。父进程``fork``后，需要通过``wait``和``waitpid``等待子进程退出，这就是进程等待。

&emsp;&emsp;那为什么要让父进程等待子进程退出呢呢？

- 通过获取**子进程退出的信息**，能够得知子进程执行的结果。
- 等待可以保证一个**时序**。子进程先退出，父进程后退出，这是符合正常时序的。
- 进程退出时，会先进入**僵尸状态**，会造成**内存泄漏**的问题，需要通过父进程``wait``来**释放该子进程占用的空间**。
- 进程一旦进入僵尸状态，进程已经死掉了，``kill -9``都已经对它无能为力，需要父进程来解决它。

## 2 怎样进行进程等待？

### I 系统调用—wait

&emsp;&emsp;它的作用是让父进程进行进程等待，等待子进程退出变成僵尸进程后回收其资源并获得它的退出信息。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319100721.png" style="zoom:80%;" />

&emsp;&emsp;返回值：如果等待成功，返回被终止的进程的``pid``，如果失败返回``-1``。

&emsp;&emsp;它的参数的作用与下面要讲解的``waitpid``类似，因此我们放在下面一起讲解。

&emsp;&emsp;wait能够回收子进程的僵尸状态的测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319102043.png" style="zoom:80%;" />

进程监控shell脚本：

```bash
while :; do ps axj | head -1 &&  ps axj | grep test | grep -v grep; sleep 1; echo "##############################"; done
```

&emsp;&emsp;下面的图片中可以看到父进程回收了子进程的僵尸状态。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319101914.png" style="zoom:80%;" />

### II 系统调用—waitpid

**初识waitpid**方法：

```cpp
pid_ t waitpid(pid_t pid, int *status, int options);
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319104022.png" style="zoom:80%;" />

&emsp;&emsp;``waitpid``也是一个让父进程进行进程等待的系统调用，先来看全缺省表现：第二个参数设置为NULL，第三个参数设置为0，得到的效果如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319102420.png" style="zoom:80%;" />

&emsp;&emsp;``pid``设置为-1，等待任意一个子进程，``pid``设置为具体的``pid``，等待具体的子进程，通过上面的测试可以发现效果与``wait``一样，可以回收子进程的僵尸进程状态，它的全缺省表现和``wait``函数类似。

&emsp;&emsp;我们现在还没处理子进程的退出信息，我们来看看``waitpid``的参数。

&emsp;&emsp;先看第二个参数，第二个参数``status``是一个输出型参数，子进程的退出信息就会放在这个``status``中。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319103155.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319103348.png" style="zoom:80%;" />

&emsp;&emsp;``status``是一个``int``，有32个比特位，我们只使用低16位比特位，高16位暂时不做讨论。

&emsp;&emsp;进程退出无非这三种情况：

- 代码运行完毕，结果正确
- 代码运行完毕，结果错误
- 代码异常终止。

&emsp;&emsp;代码异常终止时，本质是进程因为异常问题，收到了某种信号，Linux中为很好的获得子进程的退出的两部分原因，``status``的构成如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319174305.png" style="zoom:80%;" />

&emsp;&emsp;``status``的低7位表示收到的信号，如果是0则表示它没有收到信号，高8位表示退出状态，也就是**退出码**。

&emsp;&emsp;所以要先判断低7位有没有收到终止信号看看其是否为异常终止，如果没有收到信号，再看看高8位确定其退出状态。

&emsp;&emsp;接下来做个实验：我们利用位运算获得对应位组成的信号与数字即可。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319105637.png" style="zoom:80%;" />

代码运行完毕，结果不正确：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319105736.png" style="zoom:80%;" />

代码运行完毕结果正确：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319105844.png" style="zoom:80%;" />

异常退出信号验证：

我们使用``kill -2 pid``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319110051.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319110104.png" style="zoom:80%;" />

除0，验证浮点数错误的异常退出。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319110241.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319110314.png" style="zoom:80%;" />

&emsp;&emsp;对应发现是浮点错误。

&emsp;&emsp;所以bash一定也是通过``wait``或者``waitpid``得到子进程的退出码，放到``echo $?``中。

### III Linux提供的宏—WIFEXITED和WEXITSTATUS

&emsp;&emsp;用位运算获得退出信号和退出码未免太过麻烦，因此操作系统提供了两个函数宏，``WIFEXITED``和``WEXITSTATUS``。

&emsp;&emsp;``WIFEXITED(status) == true``表示子进程的退出没有收到任何其他信号，即非异常退出，``WEXITSTATUS(status)``表示获得退出码，因此我们就可以设计一个先看看程序是否没有接收到信号正常执行完代码结束，如果是则看看其退出码。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319110918.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319111904.png" style="zoom:80%;" />

## 3 怎样理解waitpid

&emsp;&emsp;``waitpid``是一个系统调用接口，其与操作系统的内核交互，其作用原理大概如下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319181933.png" style="zoom:80%;" />

## 4 waitpid的第三个参数

```cpp
pid_ t waitpid(pid_t pid, int *status, int options);
```

&emsp;&emsp;``options = 0``,代表阻塞等待，我父进程就在这什么都不干；

&emsp;&emsp;``options = WNOHANG``，代表非阻塞等待。

### I 阻塞等待与非阻塞等待

&emsp;&emsp;什么是阻塞，什么是非阻塞？

&emsp;&emsp;假如你找人办事，你给那个人打电话，时刻获得他的办事状态，但是电话不挂，你一直等在那里什么也不干，就干等，他不下来，你就不挂电话，不去做其他的事情，这叫做阻塞等待。

&emsp;&emsp;假如我不断地打电话问那个人事情办完没，没办完则我先把电话挂了去干别的事情，即没过一段时间就来检查一下它的事情处理状态，并不会因为在这里等待就卡住，这就称为非阻塞等待。

&emsp;&emsp;这里非阻塞等待的多次打电话就被称为**基于非阻塞等待的轮询方案**。

&emsp;&emsp;我们这里进程的等待是父进程在等待子进程退出。

&emsp;&emsp;阻塞等待意味着父进程的代码目前就不会被CPU调度，本质就是把父进程的PCB加到等待队列中，将父进程的状态由R状态改为S状态，子进程退出后再把父进程的PCB放回运行队列，并且把父进程的状态由S改回R。

&emsp;&emsp;看到某些应用或者os本身，卡住了长时间不动，我们称为程序``hang``住了，所以``WNOHANG``就是非阻塞。

&emsp;&emsp;我们通过``waitpid``非阻塞等待的返回值和while循环实现基于轮询的非阻塞等待：

&emsp;&emsp;等待成功，但是子进程还没退出时，且``waitpid``第三个参数是``WNOHANG``时，``waitpid``会返回0。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319104022.png" style="zoom:80%;" />

&emsp;&emsp;基于轮询的非阻塞等待方案，父进程部分代码设计(子进程就去先做自己的事情即可)：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319114610.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319114806.png" style="zoom:80%;" />

# 四、进程的程序替换

## 1 什么是进程替换

&emsp;&emsp;进程不变，**仅仅替换进程的代码和数据以及修改部分页表的映射**，这样就能让这个进程执行新的程序了，就被称为进程的程序替换。

&emsp;&emsp;进程的老的壳子没有被修改，比如``PCB``、``mm_struct``，进程的程序替换没有创建新的进程！

&emsp;&emsp;进程替换的原理图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319183108.png" style="zoom:80%;" />

## 2 怎样进行进程的程序替换？

&emsp;&emsp;Linux提供了``exce*``系列函数来进行进程的程序替换。

```bash
man execl
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319115726.png" style="zoom:80%;" />

&emsp;&emsp;第一个进程的程序替换测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319120029.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319120342.png" style="zoom:80%;" />

&emsp;&emsp;替换了去执行另外一个程序去了（即我们的ls指令），没有打印``hahaha``。

## 3 进程程序替换的讨论点

&emsp;&emsp;进程程序替换的本质就是把指定程序的代码和数据加载到特定进程的上下文中。

&emsp;&emsp;C/C++程序要运行，必须先加载到内存中，如何加载，加载器，这不也是一种程序替换嘛，所以在类Unix系统中就是用的exec*系列系统调用，它就是加载器。

- fork创建的父子进程与exec*系列函数

&emsp;&emsp;父进程和子进程，若子进程进行进程程序替换，由于父子进程共享代码，父进程受收影响吗？

&emsp;&emsp;不会，因为**进程具有独立性**，子进程进行进程程序替换会替换代码，为了保证父进程的独立性，会发生**写时拷贝**。

&emsp;&emsp;所以我们如果想**让进程去执行另外的一个程序**，就可以用进程的程序替换，这就是进程的程序替换的意义。

&emsp;&emsp;下面是一个把程序替换的逻辑放到父进程中。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319122116.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319122028.png" style="zoom:80%;" />

- exec*系列函数的返回值讨论

&emsp;&emsp;由于``exec*``系列函数进行进程程序替换，如果替换成功后，就会去执行另一个程序的代码，而就不会执行后续代码，所以意味着``exec*``成功时，不需要返回值检测。也就是说，只要``exec*``返回了，未能去执行对应程序，那么就程序替换失败了，所以我们所有的进程替换的测试代码的框架可以这么搞：

```cpp
execl();
exit(-1);
```



## 4 各个exec*系列函数的使用

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327142420.png" style="zoom:80%;" />

### I execl

&emsp;&emsp;``l``表示参数采用列表方式，参数一个一个的罗列出来。

```cpp
int execl(const char *path, const char *arg, ...);
```

&emsp;&emsp;第一个参数``path``表示要替换的目标程序的全路径，即所在路径/文件名。

&emsp;&emsp;第二批参数叫做可变参数，表示要执行的目标程序在命令行程序上想怎么跑，参数就怎么传，且必须以NULL作为参数传递的结束。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319122842.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220319122923.png" style="zoom:80%;" />

&emsp;&emsp;验证程序替换后后续代码不会执行：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327141714.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327141752.png" style="zoom:80%;" />

&emsp;&emsp;也验证了只要``execl``返回了，运行就一定出错了。

### II execv

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220326093659.png)

```cpp
int execv(const char *path, char *const argv[]);
```

&emsp;&emsp;v的含义是以数组方式传程序执行的参数。

&emsp;&emsp;第一个参数是程序的路径，第二个参数是以指针的数组的方式来传程序在命令行上执行时的参数。

&emsp;&emsp;测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327143058.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327143134.png" style="zoom:80%;" />

### III execlp

&emsp;&emsp;``p``的含义是``path``，它的含义是你让我切换的程序只告诉我名字就行了，我会自动在环境变量``path``中找，它还带``l``，所以需要和``execl``一样传参数。

```cpp
int execlp(const char *file, const char *arg, ...);
```

&emsp;&emsp;测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327143533.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327143611.png" style="zoom:80%;" />

### IV execvp

&emsp;&emsp;传程序执行方式同``execv``，只不过传参上不用带路径，只需要传程序名，自动在环境变量中搜索。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327144051.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327144031.png" style="zoom:80%;" />

### V execle

```cpp
int execle(const char *path, const char *arg, ...,char *const envp[]);
```

&emsp;&emsp;``e``的意思是自己维护环境变量，用``envp``来传给它环境环境变量。

&emsp;&emsp;首先验证``execl``可以执行其他程序，首先我们设置两个程序：``myexe``和``myload``，一个负责加载，一个负责执行程序，这就需要我们的``makefile``一次形成两个可执行程序。

&emsp;&emsp;``vim``底行模式下替换：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327144641.png" style="zoom:80%;" />

&emsp;&emsp;``makefile``生成两个两个可执行文件：

&emsp;&emsp;``myexe.c``的内容：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327144841.png" style="zoom:80%;" />

&emsp;&emsp;利用``makefile``默认执行第一个目标，我们利用一个伪目标``all``让它依赖于``myload``和``myexe``就行了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327145308.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327145157.png" style="zoom:80%;" />

&emsp;&emsp;我们想用``myload``执行``myexe``，首先利用``execl``就可以做到：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327145614.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327145641.png" style="zoom:80%;" />

&emsp;&emsp;``execle``会向切换的程序导入环境变量的验证：

``myload.c``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327150138.png" style="zoom:80%;" />

``myexe.c``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327150346.png" style="zoom:80%;" />

&emsp;&emsp;运行结果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327150517.png" style="zoom:80%;" />

&emsp;&emsp;如果直接运行``myexe``，环境变量是系统给的那一堆：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327150710.png" style="zoom:80%;" />

&emsp;&emsp;所以``e``的含义就是允许我们自己给切换的程序给环境变量。

### VI execve

```cpp
int execve(const char *path, char *const argv[], char *const envp[]);
```

&emsp;&emsp;它和``execle``的区别就是第二个参数是以数组的方式传执行参数的方法，注意以``NULL``结尾，测试如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327151128.png" style="zoom:67%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327151227.png" style="zoom:80%;" />

&emsp;&emsp;有这么接口的目的是为了满足不同的使用场景。

## 5 exec系列用于执行其他脚本语言代码

&emsp;&emsp;假如我们有一段``py``脚本代码或``shell``脚本代码或者其他的代码，我们可以用``exec``系列函数让这个进程去执行那个脚本语言对应的解释器，让它去执行对应的代码，下面以一个``py``脚本为例：

``test.py``内容：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327151909.png" style="zoom:80%;" />

普通执行：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327151935.png" style="zoom:80%;" />

``myload.c``的内容：通过``execl``执行``python3``解释器:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327152150.png" style="zoom:80%;" />

``myload``运行结果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327152249.png" style="zoom:80%;" />

## 6 execve与其他接口的不同

&emsp;&emsp;所有的接口的作用是没什么区别的，只是参数有所不同。

&emsp;&emsp;那为什么会有这么多接口呢？是为了满足不同的应用场景。

&emsp;&emsp;但是我们在``man 3 exec``手册中，发现``execv``系列中没有``execve``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327152905.png" style="zoom:80%;" />

&emsp;&emsp;发现``execve``特地在``man 2 execve``中，也就是说，它并不是一个普通接口，根据我们之间讲man的知识，3的手册代表库，2的手册代表系统调用。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327152954.png)

&emsp;&emsp;``execve``是2手册中的，说明它是一个系统调用，其实，Linux只提供了这一个系统调用，其他的都是它封装出的函数罢了，它们的关系大概入下图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327153845.png" style="zoom:80%;" />

# 五 利用进程的程序替换完成一个minishell

&emsp;&emsp;我们已经可以使用进程的程序替换让一个进程去执行其他磁盘上的程序了，我们可不可以让这个程序一直跑，我们输命令，然后它帮我们执行对应程序呢？这是不是就很像一个``shell``呢？

&emsp;&emsp;所谓的``shell``解释器，不就是打印一个打印提示符，然后等待你字符串输入去执行对应的程序嘛，所以我们利用目前学习了的知识制作一个简易的``shell``。

## 1 打印提示符

&emsp;&emsp;打印提示符，利用``fflush(stdout)``把提示符刷新出来而不换行：

&emsp;&emsp;获取当前用户的用户名：

&emsp;&emsp;先用``getuid``获得用户id，头文件``<unistd.h>``和``<sys/types.h>``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327231047.png" style="zoom:80%;" />

&emsp;&emsp;然后用``getpwuid``，头文件``<sys/types.h>``和``<pwd.h>``，得到一个``struct passwd``结构体指针``pwd``，它的``pwd->pw_name``就是用户名，``pwd->pw_dir``就是用户的默认工作路径。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327231134.png" style="zoom:80%;" />

&emsp;&emsp;获取当前进程的工作路径，打印在提示符中，使用``getpwd``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327230333.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327231205.png" style="zoom:80%;" />

## 2 获取命令行字符串

&emsp;&emsp;利用``fgets``获取标准输入一行的字符串，先打印出来看看：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327155951.png" style="zoom: 80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327155912.png" style="zoom:80%;" />

&emsp;&emsp;发现多打印了个回车，这是因为我们最后是自己输入了一个回车表示命令结束，所以可以把这个``\n``改成``\0``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327160255.png" style="zoom: 80%;" />

## 3 解析命令行字符串

&emsp;&emsp;然后解析命令字符串，将字符串进行分割，利用``strtok``，参数是串和分割符，返回第一个子串。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327161220.png" style="zoom:80%;" />

&emsp;&emsp;给出一个利用``strtok``分割字符串的模板：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327162139.png" style="zoom:80%;" />

&emsp;&emsp;测试结果如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327162242.png" style="zoom:80%;" />

## 4 执行命令

&emsp;&emsp;执行命令肯定不能我自己这个进程去执行，因为如果这样我原本命令行的这些代码就会都没了，所以需要利用``fork``。

&emsp;&emsp;我们获取的命令一般都是直接就是名字，一般就是在环境变量里头找，所以用``execvp``去让子进程进行进程的程序替换比较合适。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327163101.png" style="zoom:80%;" />

&emsp;&emsp;这样一个基本的命令行解释器就完成了，测试如下，如果输错命令了，使用``ctrl + Backspace``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327163205.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327163435.png" style="zoom:80%;" />

&emsp;&emsp;由此可见，我们的shell确实是一个进程，``ps -axj | grep mini_shell``看一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327163829.png" style="zoom:80%;" />

&emsp;&emsp;所以shell执行的程序挂了不会影响shell，就是因为那不过是它的子进程罢了。

## 5 切换路径与内建命令

&emsp;&emsp;但是我们发现路径并不会怎么变化：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327163227.png" style="zoom:80%;" />

&emsp;&emsp;原因是子进程执行的``cd ..``，并非是``shell``回退路径，我们想要shell（父进程）执行``cd ..``这样才能路径切换。

&emsp;&emsp;所以需要用``fork``执行的命令都是第三方命令，像``cd``这样的命令，需要以内建命令的方式执行（不创建子进程，让父进程shell自己执行），相当于父进程自己调用函数。

&emsp;&emsp;Linux中切换进程的工作路径有一个接口：``chdir``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327164439.png" style="zoom:80%;" />

&emsp;&emsp;如果是C++可以用``unordered_set``进行映射，我们这里仅添加一个``cd``内建命令的解析，利用``strcmp``确定是内建命令后让父进程执行内建命令。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327164908.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327164741.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327165505.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327165550.png" style="zoom:80%;" />

## 6 获得退出码

&emsp;&emsp;利用前面学的进程等待获得退出码和退出信号即可。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327170325.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327170427.png" style="zoom:80%;" />





