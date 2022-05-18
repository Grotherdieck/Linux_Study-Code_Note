# 一、C语言文件操作回顾

## 1 利用fputs写文件内容

&emsp;&emsp;C语言利用文件指针与``fputs``完成文件写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171053.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171027.png" style="zoom:80%;" />

## 2 利用fgets读文件内容

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171608.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171636.png" style="zoom:80%;" />

## 3 fopen打开方式"a"：追加

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171952.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327171928.png" style="zoom:80%;" />

# 二、系统文件IO调用接口的引入

## 1 stdin stdout stderr

&emsp;&emsp;C语言会默认为我们打开三个输入输出流：``stdin``、``stdout``、``stderr``，从类型是``FILE*``的角度来看，C语言好像把他们当文件来处理。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172109.png" style="zoom:80%;" />

&emsp;&emsp;``stdin``对应的叫做键盘，``stdout``对应的硬件是显示器，``stderr``对应的硬件是显示器，我们最终一定是向硬件写入的。

&emsp;&emsp;利用``fputs``向标准输出写入：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172450.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172435.png" style="zoom:80%;" />

&emsp;&emsp;我们也可以利用输出重定向把标准输出的内容从标准输出写到文件中：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172625.png" style="zoom:80%;" />

&emsp;&emsp;这里引入输出重定向的原因想说明``stderr``虽然绑定的也是显示器，但是它无法被输出重定向。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172849.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327172924.png" style="zoom:80%;" />

&emsp;&emsp;所以输出重定向的含义是把``stdout``的内容重定向到别的地方，``stderr``的内容无法被这样输出重定向，如果要重定向``stderr``和``stdout``，命令行操作为`` &>``和``&>>``.

## 2 cout cin cerr

&emsp;&emsp;C++中也有三个对象表示这三个输入输出流：``cout``、``cin``、``cerr``。

&emsp;&emsp;一般大部分语言都会提供这三个输出输出流。

&emsp;&emsp;所以这说明了``fputs``向一般文件或硬件设备都能写入，磁盘明明是硬件，但是也可以写入，这体现了Linux的一切皆文件。

## 3 OS与文件读写的关系引入

&emsp;&emsp;所有的读写，最终访问的都是硬件：显示器、键盘、磁盘（普通文件），而OS是硬件的管理者，**所以所有的语言对“文件”的操作，都必须贯穿OS**，而操作系统不相信任何人，访问操作系统必须通过系统调用。

&emsp;&emsp;所以结论就是几乎所有语言封装的文件的读写操作，如``fopen``、``fclose``、``fread``、``fwrite``等等函数的**底层一定需要使用OS提供的系统调用**。

# 三、文件IO的系统调用接口

## 1 open和close以及它们的测试

&emsp;&emsp;先用``man 2 open``看一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327174356.png" style="zoom:80%;" />

```cpp
int open(const char* pathname, int flags, mode_t mode);
```



&emsp;&emsp;第一个参数``pathname``表示文件的路径与名字，第二个参数``flags``表示状态（只读、只写、创建），第三个参数``mode``表示创建文件的权限信息。

&emsp;&emsp;``flag``的参数宏：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327174550.png" style="zoom:80%;" />

&emsp;&emsp;返回值的情况：返回一个文件描述符，出错则返回-1.

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327174721.png" style="zoom:80%;" />

&emsp;&emsp;接着看看``close``，参数是一个文件描述符。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327174934.png" style="zoom:80%;" />

**权限的讨论**

&emsp;&emsp;为了看看权限的作用，我们进行一个简单的测试：利用open和close如果文件存在就只写打开 不存在就创建：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327175539.png" style="zoom:80%;" />

&emsp;&emsp;发现创建出了一个权限奇形怪状的文件，每次创建后权限还都不一样：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327175730.png" style="zoom:80%;" />

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327175919.png)

&emsp;&emsp;这说明使用``open``系统调用创建文件时，如果你的文件没有告知权限，权限就会是混乱的，我们必须提前告知其权限。

&emsp;&emsp;以权限的八进制方案补上：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327180221.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327180339.png" style="zoom:80%;" />

&emsp;&emsp;所以我们使用``fopen("./log.txt", "w")``创建并写文件时，文件就是正常权限，从来也没关心过创建文件的权限，这就是C语言对系统调用进行了一层封装。

**flag的参数**

&emsp;&emsp;这种宏的方式是给操作系统传多个标志位的常见方法：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327180640.png" style="zoom:80%;" />

&emsp;&emsp;``flags``是一个``int``类型，32位，它是以一个bit表示一个标志位，这样同时可以传多个标志位，且使用位运算控制，速度比较快。

&emsp;&emsp;所以我们的``|``就可以把对应的位置成1，然后操作系统内部再通过``&``来判断对应的标志位是否启用。

&emsp;&emsp;所以这些宏``O_WRONLY``、``O_CREAT``、``O_RDONLY``都是只有一个比特位为1的数据，它们不会重复。

&emsp;&emsp;所以要一次向系统调用传递两个标志位，只要把它们按位或起来就行。

&emsp;&emsp;我们去``/usr/include/bits/fcntl-linux.h``中找系统中的宏来验证一下我们的说法：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327181528.png" style="zoom:80%;" />

## 2 返回值—文件描述符

&emsp;&emsp;我们把前面的程序的返回值打印出来看看：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327181747.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327181807.png" style="zoom:80%;" />

&emsp;&emsp;为啥是3呢，我们再创建一批临时文件看看它们的文件描述符：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327182038.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327182131.png" style="zoom:80%;" />

&emsp;&emsp;这个文件描述符是连续的，那为啥从3开始打，0 1 2去哪了呢？

&emsp;&emsp;实际上，文件描述符0 1 2对应的分别是标准输入、标准输出、标准错误。

&emsp;&emsp;而C语言中默认也会打开三个输入输出流：``stdin``、``stdout``、``stderr``，语言和系统之间一定是有某种对应关系的。

&emsp;&emsp;从0开始的一组数，会联想到数组下标！

## 3 操作系统组织管理加载到内存中的文件的引出

&emsp;&emsp;所有的文件操作，表现上都是进程执行相应的函数，也就是说文件操作就是进程对文件的操作，而要操作文件必须先打开文件，**本质就是将文件相关的属性信息，加载到内存**。

&emsp;&emsp;而操作系统中有大量的进程，每个进程都可以打开很多文件，进程与文件之间的关系应该是1对n的。

&emsp;&emsp;那么系统中就可能存在很多的打开的文件。

&emsp;&emsp;那么OS一定需要把打开的文件在内存中管理起来！如何管理？和管理进程一样，先描述再组织。

&emsp;&emsp;而我们学习的是Linux系统，在内核中有一个数据结构``struct file``，里头维护着打开文件的相关属性与组织这些数据结构的链接属性。

## 4 write—向文件中写内容的系统调用接口

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402185932.png" style="zoom:80%;" />

&emsp;&emsp;返回值表示我向文件中成功写入了多少字节。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402190424.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402190500.png" style="zoom:80%;" />

## 5 read 读文件内容的系统调用

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402190617.png" style="zoom:80%;" />

&emsp;&emsp;一个简单的测试：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402204007.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402203937.png" style="zoom:80%;" />

# 四、文件描述符

## 1 文件描述符的本质

&emsp;&emsp;我们打开这么文件，为啥文件描述符是从3开始的呢？0 1 2哪去了呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402191217.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402191243.png" style="zoom:80%;" />

&emsp;&emsp;当我们的程序运行起来后，变成了进程，默认情况下，OS会帮助我们的进程打开三个输入输出流：

- 0：标准输入，设备是键盘；
- 1：标准输出，设备是显示器；
- 2：标准错误，设备是显示器；

&emsp;&emsp;这和C/C++会默认帮我们打开标准输入流``stdin``、标准输出流``stdout``、标准错误流``stderr``很像。

&emsp;&emsp;0 1 2 ...，这个返回值很像数组，而open的返回值是OS给我们的，那这个数组如果存在一定是在操作系统那里，而我们知道进程打开文件都是一个进程可以打开多个文件，而打开文件是操作系统帮我们打开的，所以操作系统一定会打开更多的文件，所以操作系统一定要对打开的文件做管理！

&emsp;&emsp;一个文件尚未被打开时，它就在磁盘上静静地放着，那如果我们创建一个空文件，它是否要在磁盘上占空间呢？

&emsp;&emsp;文件除了它的内容，还有各种属性，如权限、文件名、修改时间等等，这些**属性都是数据**。

&emsp;&emsp;所以一个空文件指的是内容是空的，但是其属性一样会占空间。

&emsp;&emsp;所以一个磁盘文件，其内容就包括文件内容+文件属性。

&emsp;&emsp;所以对文件的操作就只有两种：对内容的修改(``fput``等)和对文件属性的修改(``chmod``等)。

&emsp;&emsp;回到主题，``os``来描述文件，肯定也要先描述后组织，我们先来关心描述：

&emsp;&emsp;也就是一个``struct file``，回想进程中``task_struct``放的东西，不就是一些进程的属性吗，所以``struct file``中放的一定也是文件相关的属性信息。

&emsp;&emsp;那么这些文件的结构也按双链表组织给``os``管理，那么哪些是我们进程的呢？

&emsp;&emsp;所以OS还给每个进程的``task_struct``中给了一个``struct file* ps``，它会指向一个数组``struct file* fd_array[]``，这个数组会指向一个个文件的``task_struct``，这样进程和文件就关联起来了。

&emsp;&emsp;而数组不是有一个一个的下标吗，所以对每个进程来说，这个下标就是**文件描述符**，所以进程通过一个一个文件描述符找到对应的文件。

&emsp;&emsp;所以系统调用read和write通过文件描述符，先通过``task_struct``中的``struct files* ps``找到数组，然后利用文件描述符找到数组中对应的``struct file*``，进而找到那个文件进行操作。

&emsp;&emsp;所以文件描述符本质是进程和文件关联用的数组``struct file* fd_array[]``数组的下标，调用过程如图：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402205345.png" style="zoom:80%;" />

## 2 进一步理解一切皆文件

&emsp;&emsp;网卡、键盘、显示器、磁盘等硬件，他们一定都要提供``read``和``write``方法，它们一定要和内存进行``IO``。

&emsp;&emsp;有的设备不能写，有的设备不能读，那么把对应的接口设为空就可以了，不管怎么说，不同的设备在底层来讲，读写方法一定是不一样的。

&emsp;&emsp;那为啥说一切皆文件呢，大家这些外设对应的读写方法不是不一样吗？

&emsp;&emsp;这是因为Linux中有虚拟文件系统``vfs``，不管你是什么样的外设，我都为你创建一个结构体，然后通过双链表组织这些结构。

&emsp;&emsp;然后利用**多态**的思想，在``write``和``read``中定义上一些函数指针，以保证它们能够实现对应不同硬件的读写方法（类似C++的虚函数表）。

&emsp;&emsp;但是从顶层来看，看这些``struct file``都是一样的，我要读就调用``read``方法，要写就调用``write``方法，我并不关心其到底是什么文件，也就是说，**从顶层来看，它们不过都是有读写方法的文件**。

&emsp;&emsp;我们可以从内核原码中验证，它其中的文件操作就是有一堆函数指针。

```cpp
struct file {
　　union {
　　struct list_head fu_list; //文件对象链表指针linux/include/linux/list.h
　　struct rcu_head fu_rcuhead; //RCU(Read-Copy Update)是Linux 2.6内核中新的锁机制
　　} f_u;
　　struct path f_path; //包含dentry和mnt两个成员，用于确定文件路径
　　#define f_dentry f_path.dentry //f_path的成员之一，当统的挂载根目录
　　const struct file_operations //*f_op; 与该文件相关联的操作函数
　　atomic_t f_count; //文件的引用计数(有多少进程打开该文件)
　　unsigned int f_flags; //对应于open时指定的flag
　　mode_t f_mode; //读写模式：open的mod_t mode参数
　　off_t f_pos; //该文件在当前进程中的文件偏移量
　　struct fown_struct f_owner; //该结构的作用是通过信号进行I/O时间通知的数据。
　　unsigned int f_uid, f_gid; //文件所有者id，所有者组id
　　struct file_ra_state f_ra; //在linux/include/linux/fs.h中定义，文件预读相关
　　unsigned long f_version;
　　#ifdef CONFIG_SECURITY
　　void *f_security;
　　#endif
　　
　　void *private_data;
　　#ifdef CONFIG_EPOLL
　　
　　struct list_head f_ep_links;
　　spinlock_t f_ep_lock;
　　#endif
　　struct address_space *f_mapping;
　　};
```

&emsp;&emsp;看其中就有一个``file_operations``，它就指向以下这个结构体。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402195654.png" style="zoom:80%;" />

&emsp;&emsp;验证一下0 1 2确实是标准输入、标准输出、标准错误，我们用``write``和``read``与它们交互一下。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402200319.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402200409.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402200611.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402200635.png" style="zoom:80%;" />

## 3 文件描述符的分配规则

&emsp;&emsp;我们把0关掉，看看我们打开文件分配的文件描述符是多少：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201216.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402200956.png" style="zoom:80%;" />

&emsp;&emsp;再把2关掉看看。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201149.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201108.png" style="zoom:80%;" />

&emsp;&emsp;可以验证，**文件描述符的分配规则是每次给新打开的文件找一个最小的，没有被占用的文件描述符给你用**，这和分配时的查找算法有关系。

&emsp;&emsp;那如果我们把1（标准输出）关掉呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201344.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201443.png" style="zoom:80%;" />

&emsp;&emsp;这个``printf``打印没有打印到显示器中，被打印到了文件里``printf``。

&emsp;&emsp;我们多打印一些试试呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201707.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402201751.png" style="zoom:80%;" />

&emsp;&emsp;本来我们往显示器打印，居然往文件中打印了，无意之中我们完成了一次输出重定向。

&emsp;&emsp;分析一下，``task_struct``中的``files_struct*``指向一个数组，然后这个数组中0 1 2默认指向键盘、显示器、显示器，那么我们关掉1以后，那么这个1位置的指针被置空了，然后我们再打开一个磁盘文件，该文件被分配的描述符就是1，这时1指向了一个磁盘文件。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402202216.png" style="zoom:80%;" />

&emsp;&emsp;而其实``stdout``在系统层次不过也是一个``FILE*``，``FILE``这个结构体想操作硬件的IO，它不能绕开操作系统，必须要用操作系统给的接口，即文件描述符，它其中肯定封装了一个整数表示文件描述符。

&emsp;&emsp;所以在语言层，``stdin stdout stderr``中一定也封装了一个整数，**表示其要操作的硬件的文件描述符**。

&emsp;&emsp;你``printf``想往标准输出打印，是向硬件操作，必然绕不开操作系统，所以``printf``里头一定只能是通过文件描述符来找标准输出，即通过1这个文件描述符打印，现在1这个文件描述符本来指向标准输出的显示器，被我关啦，现在指向一个磁盘文件，自然就往文件里写了。

&emsp;&emsp;我们通过``fprintf``验证一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402203405.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402203340.png" style="zoom:80%;" />

&emsp;&emsp;这其实就是重定向的原理。

# 五、重定向的原理

&emsp;&emsp;重定向分为输出重定向、输入重定向、追加重定向，注意一下操作都不要``close``文件描述符。

## 1 输出重定向

```bash
echo "hello" > log.txt
```

&emsp;&emsp;原理就是把``echo``进程的文件描述符1关掉，然后打开``log.txt``，然后向文件描述符1打印就行。

代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232500.png" style="zoom:80%;" />

运行结果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232533.png" style="zoom:80%;" />

## 2 追加重定向

```bash
echo "hello" >> log.txt
```

&emsp;&emsp;类似C语言中的``"a"``接口，我们在``open``中增加一个``O_APPEND``接口即可，其他与写入重定向相同。

代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232644.png" style="zoom:80%;" />

运行结果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232712.png" style="zoom:80%;" />

## 3 写入重定向

```bash
cat < log.txt
```

&emsp;&emsp;首先把0关掉，然后``open``对应文件，然后把从标准输入中获取内从转换成从对应文件中读取即可。

代码：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232103.png" style="zoom:80%;" />

运行结果：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404232042.png" style="zoom:80%;" />

# 六、FILE结构的定义

&emsp;&emsp;FILE结构中有：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402210512.png" style="zoom:80%;" />

&emsp;&emsp;我们也可以把``stdin、stdout、stderr``中的``_fileno``打印出来验证一下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402210700.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402210731.png" style="zoom:80%;" />

# 六、重定向的系统调用

&emsp;&emsp;我们并不一定需要非得关标准IO流，开文件完成重定向，可以用系统调用``dup2``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402210913.png" style="zoom:80%;" />

&emsp;&emsp;我们一般``dup2``用的最多。

&emsp;&emsp;它的操作就是把文件描述符表中的内容进行拷贝，我们来看看具体的描述：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402211419.png" style="zoom:80%;" />

&emsp;&emsp;``oldfd``的值拷到了``newfd``中，``newfd``和``oldfd``都变成了``oldfd``的值。

&emsp;&emsp;以输出重定向为例，我们想把3（磁盘文件描述符）的地址值拷贝到1（标准输出）去，即``dup2(3, 1)``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402212241.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402212501.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220402212439.png" style="zoom:80%;" />

&emsp;&emsp;利用``dup2``实现输入重定向。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404233256.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404233428.png" style="zoom:80%;" />

&emsp;&emsp;追加重定向只要在输出重定向后面增加一个``O_APPEND``就行了，就不赘述了。

&emsp;&emsp;补充一点，执行``exec``进行进程的程序替换时，原程序打开的文件都不会被影响，因为它们都在``task_struct``的文件属性中，不会被替换。

&emsp;&emsp;所以命令行上的重定向，如输出重定向，扫描到``>``后，fork创建子进程，然后``dup2``给子进行的1进行一个重定向，然后再进行exec做进程的程序替换执行对应输出命令，就重定向完成了。

&emsp;&emsp;子进程创建时，其文件描述符也会继承吗？

&emsp;&emsp;答案是会的，子进程以父进程为模板，会把父进程的很多数据都复制一份，这其中就包括父进程的``struct_files*``数组。

&emsp;&emsp;所以我们命令行上启动的进程都是``bash``的子进程，因为``bash``打开了``stdin stdout stderr``，它的所有子进程都继承了打开的这些文件。

&emsp;&emsp;所以文件的释放是通过**引用计数**操作的，当指向它的进程等于0时，它才释放自己。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220406143425.png" style="zoom:80%;" />

# 七、缓冲区

## 1 引入实验

&emsp;&emsp;引入实验：前面的标准输出的重定向，如果我们最后close了文件描述符，输出的东西就会不见了，不信我们看看：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404234045.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220404234128.png" style="zoom:80%;" />

&emsp;&emsp;发生什么事了？

&emsp;&emsp;先来看看标准输出和标准错误：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409091537.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409091501.png" style="zoom:80%;" />

&emsp;&emsp;进行输出重定向，发现标准错误还是会被打印到屏幕上，只有标准输出被打印到文件里了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409091659.png" style="zoom:80%;" />

&emsp;&emsp;可以用``./redir > log.txt 2>&1`` 来把标准错误也重定向到文件中，它的含义就是把文件描述符1的内容拷贝给2。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409092748.png" style="zoom:80%;" />

&emsp;&emsp;回到我们的引出实验，为什么``close``文件描述符该有的重定向的内容就没了呢？显然问题就在这个最后的``close``。

&emsp;&emsp;根本原因在于C语言本身也给我们提供了缓冲区。

## 2 用户级缓冲区—C语言缓冲区

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409215650.png" style="zoom:80%;" />

&emsp;&emsp;从C语言缓冲区刷新到OS的文件的内核缓冲区，肯定需要``fd``。

&emsp;&emsp;那么这个C语言的缓冲区在哪呢？在C语言的文件结构体``FILE``中。

```cpp
struct _IO_FILE {
 int _flags; /* High-order word is _IO_MAGIC; rest is flags. */
#define _IO_file_flags _flags
 //缓冲区相关
 /* The following pointers correspond to the C++ streambuf protocol. */
 /* Note: Tk uses the _IO_read_ptr and _IO_read_end fields directly. */
 char* _IO_read_ptr; /* Current read pointer */
 char* _IO_read_end; /* End of get area. */
 char* _IO_read_base; /* Start of putback+get area. */
 char* _IO_write_base; /* Start of put area. */
 char* _IO_write_ptr; /* Current put pointer. */
 char* _IO_write_end; /* End of put area. */
 char* _IO_buf_base; /* Start of reserve area. */
 char* _IO_buf_end; /* End of reserve area. */
 /* The following fields are used to support backing up and undo. */
 char *_IO_save_base; /* Pointer to start of non-current get area. */
 char *_IO_backup_base; /* Pointer to first valid character of backup area */
 char *_IO_save_end; /* Pointer to end of non-current get area. */
 struct _IO_marker *_markers;
 struct _IO_FILE *_chain;
 int _fileno; //封装的文件描述符
#if 0
 int _blksize;
#else
 int _flags2;
#endif
 _IO_off_t _old_offset; /* This used to be _offset but it's too small. */
#define __HAVE_COLUMN /* temporary */
 /* 1+column number of pbase(); 0 is unknown. */
 unsigned short _cur_column;
 signed char _vtable_offset;
 char _shortbuf[1];
 /* char* _save_gptr; char* _save_egptr; */
 _IO_lock_t *_lock;
#ifdef _IO_USE_OLD_IO_FILE
};
```

&emsp;&emsp;所以我们``printf``、``fprintf``这些操作本质是把内容先写到了``FILE``里的缓冲区中。

&emsp;&emsp;一般在合适的时机：遇到``'\n'``和进程退出时，就会把用户级缓冲区的内容刷新到``OS``内部。

&emsp;&emsp;用户->OS的刷新策略：

- 立即刷新（不缓冲）
- 行刷新（行缓冲\n）,比如向显示器打印数据
- 全缓冲，缓冲区满了才刷新，比如往磁盘上写入

&emsp;&emsp;OS->硬件这种策略也是适用的。

&emsp;&emsp;假如重定向显示器->log.txt，显示器是行刷新，log.txt是全缓冲，重定向后变成了全缓冲，

&emsp;&emsp;当我们没有close时，进程退出，C语言缓冲区刷新到了内核去，就能让我们在文件里看到；

&emsp;&emsp;当我们用``close``时，进程退出以前我们已经把文件描述符关了，因为我们重定向为了一个文件流，所以它的刷新策略是全缓冲，遇到``\n``也不会刷新，你关了以后，内容就丢在用户级缓冲区了，没被刷新到内核态。

&emsp;&emsp;所以可以用``fflush``把用户级缓冲区强制刷到内核缓冲区中，然后再关掉文件描述符，就可以正常的在文件中看到重定向的内容了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409094925.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409095007.png" style="zoom:80%;" />

&emsp;&emsp;所以以下代码``printf``和``fprintf``为啥不会重定向到文件里？因为重定向后变为全缓冲策略，C语言缓冲区的内容还没来得及刷新就被关掉了，那么为啥``write``的内容能够重定向呢？因为``write``是系统调用，它直接往内核里写。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409095722.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409095755.png" style="zoom:80%;" />

&emsp;&emsp;一个新的实验：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409100037.png" style="zoom:80%;" />

&emsp;&emsp;往显示器上打印，显示四条消息，没问题：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409100216.png" style="zoom:80%;" />

&emsp;&emsp;如果重定向到文件里，则居然有7条？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409100324.png" style="zoom:80%;" />

&emsp;&emsp;发现重复出现的内容都是C语言的接口，系统调用的接口并没有出问题。

&emsp;&emsp;我们重定向到了文件里，本质上是C语言的缓冲区刷新策略变了，所以是全缓冲，fork时父进程缓冲区的内容还没被刷新到内核去呢，然后子进程会拷贝父进程的C语言级缓冲区，然后它们在进程退出时才刷新到内核中；本来未重定向是标准输出时，遇到``\n``就刷新了，父进程缓冲区早就没内容了，所以不会打印两份。

&emsp;&emsp;怎么阻止这一问题呢？我们可以在``fork``之前``fflush``呀。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409101018.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409100958.png" style="zoom:80%;" />

&emsp;&emsp;所以为啥``write``没有两份呢，因为它是一个系统调用接口，它不会往C语言级的缓存区写，所以也不会出现因为C语言级缓冲区未及时刷新而出现的问题。

&emsp;&emsp;所以``iostream``这些流对象本身也给我们提供了缓冲区，所以我们才需要``std::endl``刷新C++流对象的缓冲区并换行。

# 八、理解文件系统

## 1 机械磁盘的常识

&emsp;&emsp;我们以上讨论的内容都是已经打开的文件在操作系统的层面上如何和进程关联的时候。

&emsp;&emsp;那如果一个文件没有被打开呢？磁盘上的普通文件操作系统是如何管理的呢？

&emsp;&emsp;文件 = 文件内容 + 文件属性，没有打开时，文件的这些数据就会在磁盘中放着，磁盘是计算机中的一个机械设备（不考虑这些例外：SSD，FLASH卡、USB）。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409104506.png" style="zoom:80%;" />

&emsp;&emsp;一般机械硬盘虽然速度慢，但是容量大比较便宜，且读写限制小，企业中常用它来做存储业务。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409104618.png" style="zoom:80%;" />

&emsp;&emsp;固态硬盘主要优势在于速度快，但是读取有上限，读取到了次数上限会击穿。

&emsp;&emsp;我们讨论机械磁盘，它的盘面很像一个光盘，不过光盘VCD等是只读的，磁盘的盘面是可读可写的。

&emsp;&emsp;通过盘片转动磁头转动，可以定位到盘片的不同位置。

&emsp;&emsp;凡是在盘片的同心圆的一圈，上面一块一块的存储称为扇区，一个同心圆的一圈扇区称为一个磁道，一堆磁道形成一个柱面。我们磁盘读写的基本单位就是扇区。大小是512字节。

&emsp;&emsp;我们要找到扇区，首先要找到盘面，然后找到磁道，然后找到扇区。

&emsp;&emsp;一般一个扇区IO效率太低了，我们一般以8个扇区（4kb）为一组与内存进行交互。

&emsp;&emsp;以磁带为例，**我们可以把盘片想象成线性的结构**。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409110416.png" style="zoom:80%;" />

&emsp;&emsp;利用虚拟化的思想，OS视磁盘为一个线性的结构。

## 2 磁盘到inode

&emsp;&emsp;磁盘很大，那么管理成本肯定很高啊，所以我们一般也会把磁盘划分成一个一个区域：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409111850.png" style="zoom:80%;" />

&emsp;&emsp;其中一个分区主要组成内容的部分如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409220317.png" style="zoom:80%;" />

&emsp;&emsp;``Boot Block``：文件系统启动相关的内容。

&emsp;&emsp;然后把剩余空间继续分组：``Block group i``，因为它们都很类似，所以我们只要设计好``Block group 0``，就可以管理好这些剩余空间。

&emsp;&emsp;``Super Block``：描述整个分区（而不是一个组）的文件信息，整个分区有多少组，每个组是什么情况，有多少``inode``没用之类的，相当于备份。

&emsp;&emsp;``Group Descriptor Table``: 描述组相关的信息，如有多少inode没被使用，有多少被使用了，多少

&emsp;&emsp;``Block Bitmap``:表示``Block``的使用情况的位图。

&emsp;&emsp;``inode Bitmap``：表示``inode``使用情况的位图。

&emsp;&emsp;``inode Table``：管理文件属性，里面是一个一个小块，一个小块一般称为一个``inode``。

&emsp;&emsp;``Data block``：管理文件内容，里面是一个一个小块，一个小块一般称为一个``block``。

&emsp;&emsp;文件名在系统层面没有任何意义，是给用户识别使用的，在Linux中真正表示一个文件，是通过一个文件的inode编号，一般一个文件一个inode。

&emsp;&emsp;一个``inode``中有文件属性和一个``inode_number``表示编号，里头还有一个``blocks``来关联到文件内容对应的``block``。

```cpp
struct inode
{
    // ...文件的所有属性
    int inode_number;// inode编号
    int blocks[32];// 数据块
}
```

&emsp;&emsp;所以读取一个文件就是先找到它的``inode``，然后获得了属性，然后通过``blocks``找到数据块，它后面可能有双链表来存储数据块。

&emsp;&emsp;那么为了找到``inode``，我们通过一个``inode Bitmap``和``Block Bitmap``。

&emsp;&emsp;它是一个位图，里头比特位的位置代表是第几个``inode``，内容为0或为1表示特定的``inode``是否被占用了，我们要创建文件时，可以通过``inode Bitmap``加位操作找到第一个可以写的位置，然后写。

&emsp;&emsp;申请数据块时也是同理，通过``Block Bitmap``找可以用的``Block``，通过位运算快速申请和释放。

&emsp;&emsp;有了``inode``的理解后，首先目录也是文件，所以在磁盘的角度，它也有它的``inode``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409114904.png" style="zoom:80%;" />

&emsp;&emsp;那么目录中有数据吗？肯定也得有啊，那么目录的数据块中放啥呢？

&emsp;&emsp;我们创建的任何文件，它肯定在某一个目录下，我们操作者需要文件名，系统需要``inode``编号，所以目录中就放了文件名和``inode``编号的映射。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409121245.png" style="zoom:80%;" />

&emsp;&emsp;以下操作在文件系统级别是怎么操作的呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409115757.png" style="zoom:80%;" />

&emsp;&emsp;创建文件，通过``inode Bitmap``找到能够创建的``inode``编号，然后把内容通过这个当前目录下的文件名映射到``inode``编号然后找到``inode``，然后找到它的block，写入；然后cat打印，就是把文件名通过目录内容映射到``inode``编号，然后找到``inode``，进而找到``block``编号，然后打印其内容。``ls -al``就是先通过文件名在目录汇总找到``inode``编号，然后找到其``inode``，你要啥打印啥。

&emsp;&emsp;删除文件对应操作就是：直接把``inode Bitmap``中对应的位由1置成0，然后把对应Block Bitmap的对应的位由1置0，不需要删除其属性和数据，即只是把其``inode``是否有效改了，等下次有新的文件占用这个``inode``后，就会把内容和属性覆盖掉。

&emsp;&emsp;所以Linux下恢复文件可以拿着``inode``编号把``Inode Bitmap``的比特位由0置1，然后把Block Bitmap中的比特位也由0置1，Linux下``debugfs``可以帮助我们恢复文件。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409120546.png" style="zoom:80%;" />

&emsp;&emsp;所以如果误删除了文件，最好的做法是现在什么都别做，防止被删除的文件的inode和block占了，找别人恢复或者自己如果会的自己来想办法恢复。

## 3 软链接和硬链接

&emsp;&emsp;使用以下代码可以创建一种称为软链接的文件形态：

```shell
ln -s log.txt log_s
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409121527.png" style="zoom:80%;" />

&emsp;&emsp;删除软链接，这里不建议用``rm -rf``

```shell
unlink log_s
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409121559.png" style="zoom:80%;" />

&emsp;&emsp;它的作用和快捷方式类似，通过软链接文件可以在当前目录运行别的目录的文件。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409121920.png" style="zoom:80%;" />

&emsp;&emsp;硬链接：它不需要带``-s``选项。

```shell
ln redir hard
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409122511.png" style="zoom:80%;" />

&emsp;&emsp;软链接和硬链接的区别？软链接有自己独立的``inode``，说明它是一个独立的文件，有自己的``inode``属性，也有自己的数据块（保存指向的文件的所在路径 + 文件名）。

&emsp;&emsp;而硬链接本质上就不是一个独立的文件，它没有自己的``inode``(编号都一样)，而是一个自己文件名和链接向的文件的``inode``编号的映射关系，它没有自己的属性和数据。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409122332.png" style="zoom:80%;" />

&emsp;&emsp;所以创建硬链接本质是在特定的路径下添加一对文件名（硬链接名）和``inode``的映射关系。

&emsp;&emsp;硬链接某种程度上来说算是把文件重命名了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409122728.png" style="zoom:80%;" />

&emsp;&emsp;所以ls查看时权限后面的数字就是该``inode``拥有的硬链接数。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409123018.png" style="zoom:80%;" />

&emsp;&emsp;而这个硬链接的计数也可以存在``inode``中，采用引用计数的机制，当硬链接数为0时，才删除这个文件。

&emsp;&emsp;那为啥创建一个目录，默认的硬链接数是2呢？一个是当前路径下你的路径的目录文件名这个映射，另一个是你的创建的目录中的``.``，它也会硬链接向这个目录。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409123439.png" style="zoom:80%;" />

&emsp;&emsp;我们创建了文件夹的路径的硬链接数是3呢？一个是当前文件名2022_4_9，另一个是进去后的``.``，另一个是test目录中的``..``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409123605.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220409123635.png" style="zoom:80%;" />

## 4 文件时间

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416091447.png" style="zoom:80%;" />

&emsp;&emsp;Access：文件最近被访问的时间。

&emsp;&emsp;Modify：最近一次修改**文件内容**的时间。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416091727.png" style="zoom:80%;" />

&emsp;&emsp;Change：最近一次修改**文件属性**的时间。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416091916.png" style="zoom:80%;" />

&emsp;&emsp;增加文件内容时，Modify和Change都变了，这是因为我们修改文件内容时，有可能修改文件的属性，比如修改文件内容时，可能会更改文件的大小属性。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416092011.png" style="zoom:80%;" />

&emsp;&emsp;那为啥我们一顿操作是Access时间都不怎么变呢？这是因为它在较新的Linux内核中，Access时间不会被立即刷新，而是以一定的时间间隔让操作系统进行自动更新时间，如果频繁更新，则频繁刷盘可能导致Linux系统速度很慢。

&emsp;&emsp;makefile和时间相关：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416092852.png" style="zoom:80%;" />

&emsp;&emsp;为啥我修改了以后才能重新make呢？也就是说gcc怎么判断源文件和编译出的文件谁更新谁更旧从而判断是否要重新编译呢？

&emsp;&emsp;当我们修改文件内容时，Modify时间一定会被修改，当我们随便修改一下代码内容，那么test.c的时间一定比test的时间晚，而我们make形成test之后，test的时间一定比test.c要早些，所以gcc也是通过这个时间去对比的。

&emsp;&emsp;所以我们可以通过``touch``命令来更新文件的三个时间，然后就可以make了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416093549.png" style="zoom:80%;" />

# 九、动静态库

## 1 动态库和静态库的概念

&emsp;&emsp;如何查看程序使用了那些库？``ldd 可执行程序``，它可以显示可执行程序依赖的动态库。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416093944.png" style="zoom:80%;" />

&emsp;&emsp;另外两个我们都不认识，但是中间那个我认识啊，就是C标准库哇。

&emsp;&emsp;看一下C语言的标准库：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416094147.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416094223.png" style="zoom:80%;" />

&emsp;&emsp;那么这是什么类型的库呢？

&emsp;&emsp;在任何平台中，库分两种，在Linux中，如果是动态库（库也是个文件），一般默认是以``.so``作为后缀的；如果是静态库，那么库文件是以``.a``作为后缀的，在windows中，动态库是以``.dll``为结尾的，静态库是以``.lib``为结尾的。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416094512.png" style="zoom:80%;" />

&emsp;&emsp;库文件的命名：一般必须是lib开头，``lib****.a或.so``。

&emsp;&emsp;库的真实名字：去掉lib前缀和.a或.so的后缀的名字，如上面的C库真正的名字就是``c-2.17``。

&emsp;&emsp;C++的源文件后缀：``.cc .cxx .cpp``，这仨都可以被编译器识别为C++源文件。

&emsp;&emsp;看看C++的依赖呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416095349.png" style="zoom:80%;" />

&emsp;&emsp;不仅用了一些C库，还用了一些C++库，看一下C++库：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416095456.png" style="zoom:80%;" />

&emsp;&emsp;之前讲gcc时，加入``-static``选项可以让程序静态编译。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416095644.png" style="zoom:80%;" />

&emsp;&emsp;改为静态链接呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416095758.png" style="zoom:80%;" />

&emsp;&emsp;发生静态链接后，可执行程序的文件大小比动态链接的文件大小大很多。

&emsp;&emsp;在初识动态链接和静态链接中，静态链接是把库中的程序直接拷贝进目标代码，而动态链接是执行库函数等是直接到动态库中取执行。

&emsp;&emsp;那么动态库怎么做到共享的呢？其实是把动态库映射到进程地址空间，然后去对应执行。

&emsp;&emsp;我们之前讲过的很多命令，它就是用C语言写的，我们可以用ldd来看下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416100707.png" style="zoom:80%;" />

&emsp;&emsp;看着好像大家都同时用一个C语言动态库，所以如果把这个C语言动态库干掉，直接很多程序都没法运行了。

&emsp;&emsp;那么动静态库是如何制作的呢？

## 2 静态库的制作

&emsp;&emsp;我们先看看库文件里头都是啥呢，其实想想也知道，里头肯定是一堆二进制乱码。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416101501.png" style="zoom:80%;" />

&emsp;&emsp;那么库本身是二进制文件，那么我们如何得知一个库给我们提供了哪些方法呢？

&emsp;&emsp;一套完整的库，由库文件本身、头文件、说明文档组成，其中头文件会告知我们这个库中有那些方法可以使用，及其使用方法。

&emsp;&emsp;所以头文件对使用库也非常重要，这也和我们写C/C++程序时，``.c/.cpp``文件放上实现，``.h``放上声明的习惯相似。

&emsp;&emsp;我们这样设计的原因是我们可能要制作库，.h中只有声明对制作库方便。

&emsp;&emsp;``.hpp``文件可以直接把声明和定义放一起，这样可以直接把源文件给别人去用。

&emsp;&emsp;但是直接给源文件会暴露实现细节，而库文件可以实现私密，对源文件进行封装保护。

&emsp;&emsp;看看我们打算制作的动静态库的框架：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416102729.png" style="zoom:80%;" />

&emsp;&emsp;我们用最简单的方法来搞一个用外部``.c``文件的程序，就是一个nortest.c中引``./test_lib/*.h``，然后链接时链接上``./test_lib/*.c``即可。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416104119.png" style="zoom:80%;" />

&emsp;&emsp;那么如果我们想把我们的程序打包成静态库呢？

&emsp;&emsp;C/C++程序的编译过程中，``.o(.obj)文件是未链接的二进制文件``，所以如果要制作库就首先要先把所有的源文件打包成.o，然后用``ar -rc 静态库名 依赖文件``命令把.o文件打包成静态库。

&emsp;&emsp;所谓静态库的思想就是把文件编译成.o文件(已经是二进制了)，然后可能.o文件太多，把它们打包起来就行。

&emsp;&emsp;下面是我们在add.c sub.c所在目录的makefile，主要就是利用``ar -rc``命令生成静态库，然后利用``gcc -c``生成``.o``文件，这里``%.o``意思是这个目录下所有的``.o``文件

&emsp;&emsp;这里``$<``的含义是把上面的所有``.c``文件都一个一个在这里展开，汇编为``.o``文件。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416104804.png" style="zoom:80%;" />

 &emsp;&emsp;查看静态库里头有啥，利用``ar -tv 静态库名``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416104848.png" style="zoom:80%;" />

&emsp;&emsp;静态库发布：思想就是创建一个``output``文件夹，然后把.h文件和``libmymath.a``文件拷贝到那个目录去。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416105629.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416105427.png" style="zoom:80%;" />

&emsp;&emsp;静态库的安装：所谓安装就是把静态库的``.h``加到系统的``/usr/include``文件夹中，把静态库文件拷贝到``/lib64``去。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220417221256.png" style="zoom:80%;" />

&emsp;&emsp;我们把这个静态库打包好后，复制给了我的朋友，他怎么用呢？

&emsp;&emsp;朋友当前的路径情况：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416114408.png" style="zoom:80%;" />

```shell
gcc test.c -I./lib
// 头文件在./lib中找
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416114925.png" style="zoom:80%;" />

&emsp;&emsp;发现仍有错误，这说明我们找不到库在哪。

&emsp;&emsp;我们为gcc再指定一下再哪里找静态库，利用``-L路径 -l静态库名``，注意静态库库名是不带lib和.a这些浅缀后缀名字的。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416115325.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220418090855.png" style="zoom:80%;" />

&emsp;&emsp;那么我们之前怎么就不用指明库在哪头文件在哪呢？因为系统的库文件在``/lib64 或/usr/bin``，头文件在``/usr/include``中，编译器有环境变量，直接默认在这些路径中搜索。

&emsp;&emsp;如果我们不想带这些选项，我们就可把我们写的库拷贝到这个路径下，但是可能会污染命名池，并不推荐。

&emsp;&emsp;把库和头文件拷贝到标准路径也就是Linux中在系统中安装一个库的过程。

&emsp;&emsp;我的朋友如果想写makefile可以这么写：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416120015.png" style="zoom:80%;" />

## 3 动态库的制作

&emsp;&emsp;第一步还是要生成.o，但是在编译时要加``-fPIC``，程序内部的地址方案是：与位置无关，它与逻辑地址有关，可以把库文件加载到内存的任意位置，而不影响与其他程序的关联性。

&emsp;&emsp;怎么理解与位置无关呢？它采用的就是相对地址，即偏移量的方法来寻址，这样就不会占用别人的位置。

&emsp;&emsp;具体操作的过程（makefile文件与底下的命令行）：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416121618.png" style="zoom:80%;" />

&emsp;&emsp;那么朋友的使用与运行呢？

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416122135.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416122111.png" style="zoom:80%;" />

&emsp;&emsp;这是因为gcc编译时，我们的选项只是告诉编译器我们的库和头文件在哪。

&emsp;&emsp;但是在运行时，程序在运行时找动态库需要的是**加载器**知道动态库在哪然后去找。

&emsp;&emsp;第一种方法是把动态库拷过来考到当前路径或共享库路径下``/usr/bin``，我们仅演示放到当前路径下，因为另外一种方式会污染系统命名池：

&emsp;&emsp;拷贝到当前路径后：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416125440.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416125330.png" style="zoom:80%;" />

&emsp;&emsp;第二种方法是依赖环境变量``LD_LIBRARY_PATH``，它是加载库的寻动态库路径。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416122604.png" style="zoom:80%;" />

&emsp;&emsp;利用``export``命令增加路径到当前登陆的环境变量即可：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416122913.png" style="zoom:80%;" />

&emsp;&emsp;这样就可以运行了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416123029.png" style="zoom:80%;" />

&emsp;&emsp;第三种方法是系统级别修改：进入路径``/etc/ld.so.conf.d/``,这里操作都需要root权限，sudo即可，进来以后新增一个配置文件，我这里是``Router.conf``，文件内容增加上你动态库的路径。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416124441.png" style="zoom:80%;" />

&emsp;&emsp;然后``ldconfig``更新动态库缓存，就能用了。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416124652.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416124631.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416124741.png" style="zoom:80%;" />

&emsp;&emsp;不过也不推荐这种方式，如果需要安装很多库可以这样考虑，最好还是用环境变量``LD_LIBRARY_PATH``，因为它成本低可恢复。

&emsp;&emsp;删除后，更新缓存，又找不到了：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416125143.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220416125109.png" style="zoom:80%;" />

## 4 总结

&emsp;&emsp;我们其实一直在直接或间接的使用第三方库，最常用的如C标准库，C++标准库。

&emsp;&emsp;那么我们怎么使用的呢？拿到库和头文件，然后用``gcc -I 头文件路径 -L 库文件路径 -l库文件名``编译即可，动态库还需要选择三个操作之一（把动态库拷到当前路径或共享路径``/usr/bin``、或者把``export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库路径``（本次登陆有效）、或修改系统文件``/etc/ld.so.conf.d/``增加``*.conf``来增加库的路径，来让我们的程序能够找到库。

&emsp;&emsp;如何制作呢？把``.c``或``.cpp``文件编译成``.o``文件(vs中是``.obj``文件)，然后把这些东西打包。

&emsp;&emsp;静态库：``ar -rc 库名 *.o``;

&emsp;&emsp;动态库：``gcc -shared -o 库名 *.o``。

&emsp;&emsp;如果我们只提供静态库，那么我们只能把我们的库静态链接到我们的程序中。

&emsp;&emsp;如果值提供动态库，那么我们是能给他静态链接的，就算加了``-static``也没用。

&emsp;&emsp;如果要既能动态链接，又能静态链接，那么需要既提供动态库，又提供静态库。

&emsp;&emsp;gcc g++的链接方式默认是优先动态链接。

&emsp;&emsp;我们的Linux系统中一定有C的动态库，因为系统中有很多的命令是C语言写的，而且它是动态链接的，所以Linux系统中一定有C的静态库 gcc g++默认动态链接也有道理了。



