

# 1 Linux软件包管理器yum

&emsp;&emsp;Linux下安装卸载软件的三种解决方案：

- 源码安装
- 由于源码安装可能存在不同的环境的兼容问题，所以有人在不同的环境下都已经编译好了源码放到了包里，这种方式称为**rpm**安装
- 因为Linux的环境主要用于开发，所以Linux下可能存在大量的软件之间的依赖关系，安装一个库可能需要别的库，写一个代码可能依赖于某个库，前两种方案安装起来就非常麻烦，我要一步一步的根据报错信息把第三方库都安装好了，才能装这个程序，非常麻烦。可以使用**yum**，**yum本身会考虑软件的依赖关系**，而不需要我们自己处理。

&emsp;&emsp;yum就好像手机上的应用市场(``Appstore``)，对一个软件，我们还没下载是，这个软件在哪呢？在应用市场的服务器或在软件厂商的服务器中，yum就像应用市场一样，是一个客户端程序。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115002.png)

&emsp;&emsp;``yum``三板斧:``yum list、yum install、yum remove``

&emsp;&emsp;yum中使用``sudo list``列出所有可以安装的软件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115151.png)

&emsp;&emsp;如果要关键字搜索，可以``sudo yum list | grep 'sl'``

&emsp;&emsp;考虑到架构，可以这样关键字搜索``sudo yum list | grep 'sl.x86_64'``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115429.png)

```shell
sl.x86_64                                5.02-1.el7                     @epel
名称.架构                                 版本    属于红帽的centos7        开发公司
```

&emsp;&emsp;之前装过了，就会这样显示：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115731.png)

&emsp;&emsp;``sudo yum remove sl.x86_64``卸载这个程序，加上``-y``就不会有什么是否要卸载的提示。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115906.png)

&emsp;&emsp;卸了以后又能重新安装了：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125115948.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125120056.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125120157.png)

&emsp;&emsp;可以搜索一下Linux Centos7必装的软件/好玩的命令。

&emsp;&emsp;``yum``源：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125120837.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125121019.png)

&emsp;&emsp;这个``CentOS-Base.repo``是官方的，稳定的，是一些比较老的稳定的文件，非官方比较新的软件，放在扩展源里：``sudo yum install -y epel-release``.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125121421.png)

&emsp;&emsp;注意：

- yum要工作，必须联网。
- centos里面，只能有一个yum在运行，否则会报错。

# 2 CentOS 7.6云服务器软件举例

## I lrzsz——本地文件上传下载软件

&emsp;&emsp;``lrzsz``,安装后可以使windows和linux云服务器之间文件拖拽上传，安装命令：``sudo yum install -y lrzsz``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125123346.png)

&emsp;&emsp;它还可以使用指令上传下载文件。

&emsp;&emsp;``rz``：本地``windows``文件上传``Linux``服务器

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125123708.png)

&emsp;&emsp;``sz 文件名``：服务器文件上传``windows``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125123558.png)

## II htop——交互式进程查看器

&emsp;&emsp;``htop``是``Linux``下的一款交互式进程浏览器，可以用来替换Linux中的``top``指令

&emsp;&emsp;如果直接安装失败，首先更新安装包：``sudo yum -y install epel-release.noarch``

&emsp;&emsp;然后安装``sudo yum -y install htop``.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125132957.png)

## III GNOME+VNC——Linux服务器的远程桌面系统

&emsp;&emsp;如果实在是想要一个GUI桌面，Linux也是有提供开源的桌面的。

&emsp;&emsp;首先，运行：``sudo yum groupinstall "GNOME Desktop" -y``,然后等待出现``complete``就可以了。

&emsp;&emsp;如果要卸载，运行``sudo yum groupremove "GNOME Desktop" -y``就可以了

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125150712.png)

&emsp;&emsp;然后我们其实就已经安装好了图形化界面，接下来需要一个工具远程链接我们的服务器以显示图形化界面，我们安装vnc：

```shell
sudo yum -y install tigervnc-server
卸载 sudo yum -y remove tigervnc-server
```

&emsp;&emsp;然后通过以下命令启动VNC服务：

```shell
vncserver :3
```

&emsp;&emsp;3的意思就是将VNC服务绑定到5903端口，2就是5902。第一次输入的时候被要求设置登录密码，后来就不需要了。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125152334.png)

&emsp;&emsp;如果要修改密码，使用

```shell
vncpasswd :3
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125151322.png)

&emsp;&emsp;如果要关掉``vnc``服务,使用（前面开的多少这里就写多少的数字）

```shell
vncserver -kill :3
```

&emsp;&emsp;然后我们去阿里云控制台防火墙中把新增的5903端口放开，这样就可以通过这个端口链接服务器了。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125151737.png)

&emsp;&emsp;然后输入端口号5903，点击确定。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125151810.png)

&emsp;&emsp;接下来需要在我们的电脑上安装VNC Viewer客户端，[VNC® Connect consists of VNC® Viewer and VNC® Server](https://www.realvnc.com/en/connect/download/viewer/)下载并安装即可。

&emsp;&emsp;然后在上面输入``你的服务器的ip地址:3``(之前写的3这里就输入3)。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125152016.png)

&emsp;&emsp;然后输入之前的密码就进来了：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220125152602.png)

&emsp;&emsp;关于这个GUI的使用可以上网搜GNOME对应操作怎么使用。



参考文档：

- [如何在阿里云轻量应用服务器Centos7.6上安装GNOME图形界面](https://blog.csdn.net/weixin_43461520/article/details/122262703)
- [怎么关闭VNC服务](https://zhidao.baidu.com/question/368928275709015884.html)
- [vnc软件怎么改密码（详解vnc软件改密教程）](http://www.changchenghao.cn/n/615390.html)

# 3 vim

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218140618.png" style="zoom:67%;" />

&emsp;&emsp;``IDE``:集成开发环境，把编写、编译、调试、发布代码、运行、代码关系的维护等开发功能都继承到一起的软件就成为IDE，如VS2019.

&emsp;&emsp;``Linux``中也可以直接安装集成开发环境，不过Linux下也可以把这些功能分开使用，其中文本编辑器即为vi、vim(vi是比较早的vim).

&emsp;&emsp;确认vim:直接输入vim,然后会打开一个界面，退出``shift + ;``(也就是冒号)，输入q回车即可退出。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218112229.png" style="zoom: 50%;" />

&emsp;&emsp;``vim + 文件名``：进入编辑界面。

&emsp;&emsp;``vim``是一种多模式编辑器，主要常见的有4种模式，这里介绍三种模式：命令模式、底行模式、插入模式。

&emsp;&emsp;默认打开vim时就是命令模式。

&emsp;&emsp;命令模式下按``i``进入插入(Insert)模式。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218112958.png)

&emsp;&emsp;然后按``Esc``从插入模式回退到命令模式，退出vim，从命令模式到底行模式``shift :``,底行模式输入``w``保存刚刚写入的内容，然后再退出。

&emsp;&emsp;有时候不让写，``w!``强制写；有时候不让退出，``q!``强制退出，强制保存并退出``wq!``,普通保存并退出:``wq``.

## I 命令模式

&emsp;&emsp;vim打开文件后，默认处于命令模式。

### 1 光标相关

&emsp;&emsp;可以按小键盘的上下左右移动或使用``h``(左移一个字符)，j(下移动一行)，k(上移一行)，l(右移一个字符)。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218114131.png)

&emsp;&emsp;原因是早些年的键盘没有上下左右，使用``hjsk``充当上下左右，记忆技巧：h在最左边，所以是←，l在最右边，所以是→，j是jump，跳下，所以是↓，k是king，高高在“上”,所以是↑。

&emsp;&emsp;光标位置锚点：``shift + ^``:快速定位到行首；``shilf + $``:快速定位到行尾。

&emsp;&emsp;``gg``：定位到起始行，``shift + g``(也就是大写G):定位到结束行，``n + shift + g``;跳转到第n行。

&emsp;&emsp;w/b：按照单词为单位进行前后移动。w是向后移动，b是向前回退。这里符号连在一起会被视为一个单词。

### 2 文本操作

&emsp;&emsp;**复制**：

&emsp;&emsp;``yy``复制当前行，``n + yy``复制当前行及其之后的n - 1行， ``p``在当前行下一行粘贴，``n + p``一次粘贴n行。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218120040.png" style="zoom:67%;" />

&emsp;&emsp;撤销：

&emsp;&emsp;u：撤销上一次操作。

&emsp;&emsp;``ctrl + r``：撤销最近一次的撤销。

&emsp;&emsp;粘贴：

&emsp;&emsp;``yy, 10000 + p``就能一瞬间粘贴一万次，这体现了vim的强大。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218120346.png)

&emsp;&emsp;删除：

&emsp;&emsp;``dd``:删除当前光标所在行，支持``n + dd``

&emsp;&emsp;剪切：

&emsp;&emsp;``dd``完了用``p``，就是剪切后粘贴，``100 p``剪切后粘贴100次，支持``n + p``.

&emsp;&emsp;字母大小写切换：

&emsp;&emsp;``shift + ~``快速大小写切换。

&emsp;&emsp;``x``:删除当前光标指向的一个字符，支持``nx``;``X或shift + x``：删除光标之前的一个字符，支持``nX``.

&emsp;&emsp;``r``：替换当前光标的字符，支持``nr``,同时替换当前光标及之后``n - 1``个 字符；``shift + r(或R)``:进入替换模式，之后输入的字符自动替换后续字符，``Esc``退出替换模式回到命令模式。

## II 插入模式

&emsp;&emsp;命令模式下使用i或a都可以进入插入模式，a进入插入模式光标会往后移动一个字符，命令模式下o也可以进入插入模式，会新起一行，如下：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218130107.png" style="zoom:67%;" />

&emsp;&emsp;使用↑ ↓ ← →移动光标进行对应位置插入删除，和普通文本编辑器一样，不过能用命令模式做的删除复制等还是用命令模式做比较好，因为vim不支持鼠标，你用插入模式会很慢。

## III 底行模式

&emsp;&emsp;命令模式下输入``ctrl ;``进入底行模式。

### 1 行号

&emsp;&emsp;``set nu``调出行号；``set nonu``关闭行号。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218124006.png" style="zoom:67%;" />

### 2 多个文件分屏

&emsp;&emsp;进入底行模式后输入``vs 文件名``即可。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218124415.png)

&emsp;&emsp;当然还可以继续分屏。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218124618.png)

&emsp;&emsp;底行模式``wq``保存并关闭当前分屏，光标在哪就关的哪个。

&emsp;&emsp;``ctrl ww``光标在不同分屏跳转。

&emsp;&emsp;这样就可以利用命令模式在不同文件之间剪切或复制粘贴：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218125245.png)

&emsp;&emsp;底行模式下输入``help vim-modes``查看更多关于vim模式的知识。

## IV vim的配置

&emsp;&emsp;vim的配置修改后，只会对自己用户的vim进行改变，root也不会例外。

&emsp;&emsp;首先在工作目录下创建一个``.vimrc``文件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218132020.png)

&emsp;自己普通用户vim的基础配置都会写在``.vimrc``文件中，往其中增加命令就可以配置vim，如增加行号``set nu``.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218132235.png)

&emsp;&emsp;打开后默认带了行号：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218132329.png)

&emsp;&emsp;百度一下vim常见配置选项就可以查到很多vim的配置选项。

&emsp;&emsp;root用户的vim配置在/root目录下的``vimrc``文件中。

&emsp;&emsp;由于本人的主力语言是C/C++，所以直接使用gitee上的``VimForCpp``[项目](https://gitee.com/HGtz2222/VimForCpp)配置vim，配置后打开如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218134035.png)

&emsp;&emsp;这个配置有缩进、有自动补全，非常好用。

&emsp;&emsp;vim学习参考资料：[Vim从入门到牛逼](https://github.com/wsdjeg/vim-galore-zh_cn)

&emsp;&emsp;使用vim配置信任组:切换root用户，然后使用``vim /etc/sudoers``,然后在100行左右的allow any ...那里yy复制root那一行，然后p，然后修改名字为你的用户名，然后切换底行模式``wq!``退出即可。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218140224.png)

## V vim的奇淫巧技

1 gg=G 瞬间全部正确缩进格式。 

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220308231316.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220308231339.png" style="zoom:80%;" />

2 vim批量注释

``ctrl + v``进入某模式，然后利用``hjkl``作为上下左右选中需要加``//``的区域，然后``I``输入``//``，然后ESC退出即可。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220308231527.png" style="zoom:80%;" />

批量取消注释：``ctrl + v``进入某模式，然后选中要删除区域，然后d即可。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220308231656.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220308231716.png" style="zoom:80%;" />

3 vim批量替换

&emsp;&emsp;``vim``底行模式下替换：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/20220327144641.png" style="zoom:80%;" />

# 4 Linux上的编译器—gcc/g++

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222190153.png" style="zoom:80%;" />

&emsp;&emsp;直接``gcc 文件名``,不加任何选项生成的程序默认名字为``a.out``.

## I -E

```shell
gcc -E 文件名
// 把“翻译”进行完预处理后停下
```

&emsp;&emsp;如果不指定生成什么文件 会把预处理后的文件直接打印到屏幕上。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222223401.png" style="zoom:80%;" />

&emsp;&emsp;加上``-o 文件名.i``就可以把预处理后的临时文件放到``文件名.i``中，预处理后的文件名字后缀一般是``.i``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222190613.png)

&emsp;&emsp;发现头文件展开了，并且完成了宏替换

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222190756.png" style="zoom:80%;" />

&emsp;&emsp;预处理的行为就是把注释去掉，头文件展开，宏替换，条件编译等等。

## II -S

```shell
gcc -S mytest.i -o mytest.s
// 开始执行程序编译 完成编译工作后停下把汇编语言代码存到后面的文件中
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222191144.png" style="zoom:80%;" />

&emsp;&emsp;汇编语言计算机也不能直接执行，需要汇编器从汇编语言搞成二进制。

## III -c

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222191410.png)

&emsp;&emsp;``-c``的含义是开始翻译工作，完成汇编过程后停下，汇编语言会变成二进制。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222191500.png)

&emsp;&emsp;汇编形成的机器码文件不能直接执行，学术名称是**可重定向二进制文件**，等价于VS中的``.obj``文件.

&emsp;&emsp;下面的测试表明就算加上了可执行权限也无法执行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222191720.png)

&emsp;&emsp;这里不能执行是因为我们用到了``printf``函数，但是没有去链接``printf``具体函数实现所在的库，``#include <stdio.h>``中仅有函数定义，这里需要链接操作将代码中的**函数调用**，**外部数据**和库**链接**起来。

## IV 链接

&emsp;&emsp;这步链接通常不需要加上任何选项，直接写``gcc mytest.o -o mytest``链接上库，编译器会自己帮我们找需要链接的C标准库。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222192207.png" style="zoom:80%;" />

## V -o

&emsp;&emsp;如果不想一步一步的进行翻译的过程，可以``gcc -o mytest mytest.c``或``gcc mytest.c -o mytest``直接生成名为``mytest``的可执行文件。

&emsp;&emsp;记忆技巧``ESc``,预处理、编译、汇编，``iso``预处理后临时文件、编译后临时汇编代码文件、编译后临时重定向二进制文件。

## VI 优化选项

``-O0 -O1 -O2 -O3``:编译器的优化选项的4个级别，``-O0``表示没有优化,``-O1``为缺省值，``-O3``优化级别最高.

## VII 为什么C语言程序的“翻译”是这个过程

&emsp;&emsp;最早的时候，编程的只能直接写二进制机器码，写二进制代码的过程就是“打孔编程“

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222193004.png)

&emsp;&emsp;由于效率太低了，因此诞生了一门称为汇编语言的语言。

&emsp;&emsp;汇编语言的主要主体是助记符，因为人类不太好记忆二进制符号，但是记忆助记符相对容易一些，并且同时诞生了编译器，将汇编语言编译成二进制机器码。

&emsp;&emsp;后来发现汇编语言开发效率还是太低了，因此贝尔实验室开发了C语言。

&emsp;&emsp;那么开发过程中，是直接把C语言编译成机器语言还是先编译成汇编语言呢？

&emsp;&emsp;显然是应该先编译成汇编语言，首先比起直接编译成机器码，编译成汇编语言相对更加简单；另外，先编译成汇编可以兼容汇编的生态。

&emsp;&emsp;C语言中有很多需要被提前处理而不需要汇编去处理的过程，如注释等，因此有了预处理阶段。

&emsp;&emsp;为什么有链接过程呢？因为链接可以使得大家同时开发然后链接到一起共同形成程序，或直接链接前人的库，可以提高开发效率。

&emsp;&emsp;后来有了面向对象的语言后，为了提高跨平台性，就有了很多不是上述类型的语言，语言运行在一个虚拟机软件上，跨平台性较好。

# 5 初识静态链接和动态链接

&emsp;&emsp;``ldd mytest``可以查看本可执行文件依赖的库。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222194246.png)

&emsp;&emsp;``libc.so.6``去掉``lib``和后缀，此库的名字就是``c``,即C标准库。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222194351.png)

&emsp;&emsp;Linux中的静态库``.a``，动态库``.so``.

&emsp;&emsp;总结：不管是静态库还是动态库，都和程序成功运行有关，链接的作用就是把自己写的C程序和语言或第三方提供的库关联起来。

&emsp;&emsp;那么什么是静态链接，什么是动态链接呢？

&emsp;&emsp;所谓的动态链接的含义就是程序获得链接的第三方库的**位置**后，然后在我们运行到对应库函数后，会直接去第三方库里找这个函数的实现，执行对应函数后再回到我们本来写的程序继续执行，这就是动态链接的直观理解。

&emsp;&emsp;所谓的**静态链接就是直接把库中的有关代码拷贝到我们的可执行程序**中，这就是一种静态链接。

&emsp;&emsp;所以静态链接后程序就不再需要对应的库了，因为库中的代码已经被拷贝在可执行程序中了。

&emsp;&emsp;``gcc``默认采取的链接方式是动态链接，可以通过``ldd``和``file``来看:

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222195719.png" style="zoom:80%;" />

&emsp;&emsp;如果需要静态链接，需要加上``-static``选项。

&emsp;&emsp;出现了这个报错则说明云服务器未安装C标准库的静态库：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220227205839.png)

&emsp;&emsp;安装C标准库的静态库的方法如下：

```shell
sudo yum install glibc-static
```

&emsp;&emsp;然后就能静态链接成功了！链接成功后，发现确实大小有区别。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222200604.png)

&emsp;&emsp;查看对应的静态库：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222200910.png)

&emsp;&emsp;安装C++标准库的静态库的方法如下：

```shell
sudo yum -y install glibc-static libstdc++-static
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222225529.png)

&emsp;&emsp;发现C++的标准库大很多。

&emsp;&emsp;g++编译cpp,用法和gcc基本一样。

&emsp;&emsp;选项与gcc的选项都相同。

# 6 Linux上的调试器——gdb的使用

## I 启动gdb

&emsp;&emsp;``gdb 可执行文件名``进入``gbd``，``quit``退出。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222202216.png" style="zoom:80%;" />

&emsp;&emsp;进入gdb后，我们加上``l``应该能看到什么东西才对，但是却显示**没有信息被加载**。

&emsp;&emsp;这是因为如果一个程序是可以被调试的，那么该程序的二进制文件一定加入了一些debug信息，如果不需要调试，则该程序的二进制文件就不加debug信息。

&emsp;&emsp;这就对应debug版本和release版本。

&emsp;&emsp;在``Centos7``中，默认生成的程序是``release``版本，没有调试信息，不可被调试.

&emsp;&emsp;如果要生成可调式的``debug``版本，那么就要加入``-g``选项。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222202447.png)

&emsp;&emsp;首先观察到可执行文件变大了，通过``readelf -S mytestdebug | grep -i debug ``可以查看二进制文件中里头有关``debug``的行：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222202728.png)

&emsp;&emsp;``readelf``可以查看可执行文件的段信息。

&emsp;&emsp;进入``gdb``后，直接输入``run``就是直接运行起来，类似vs中的``ctrl + F5``;

## II l 行号 gdb中查看代码 

&emsp;&emsp;我们先学习``l ``,``l + 行号``,显示指定的行号后的代码,同时也会显示其周围的代码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222230256.png)

&emsp;&emsp;``l 函数名``：显示函数名附近的代码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222230651.png)

## III b 行号 加断点

&emsp;&emsp;``b 行号``在对应行号打断点，这里的b也可以写为``break``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222230807.png)

&emsp;&emsp;``b 函数名``在函数入口处打断点。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222230900.png)

&emsp;&emsp;``info b 或info breakpoints``:查看打过的所有断点信息。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222205423.png" style="zoom:80%;" />

## IV s(逐语句调试)和n(逐过程调试)

&emsp;&emsp;``run``：执行程序，一下子就会跑到断点那里，类似VS调试中的F5;

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222205452.png)

&emsp;&emsp;按vs的流程，接下来应该需要了解的是逐语句调试F11和逐过程调试F10.

&emsp;&emsp;逐语句调试：``s 就是 step``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222205702.png)

&emsp;&emsp;逐过程调试：``n 就是 next``

## V 监视有关

&emsp;&emsp;常显示变量：``display 变量名``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222210113.png)

&emsp;&emsp;``p 变量名``，仅显示一次变量值，``p 表达式``，可以把变量的值更改。

&emsp;&emsp;``undisplay 显示符号``：去掉常显示。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222210242.png)

## VI 程序调试起来后的直接运行finish continue until

&emsp;&emsp;``finish``直接运行完当前函数。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222210744.png)

&emsp;&emsp;``continue``直接运行到下一个断点。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222210618.png)

&emsp;&emsp;``until 行号``：直接运行到某一行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222212352.png)

&emsp;&emsp;如果一个程序出现了段错误，第一种方法是进入每个函数，然后用``finish``，如果没报错，那么这个函数就没错误，这样可以晒出哪个函数有错误，这个过程也可以给每个函数都打上端点，然后用``continue``，找到错误函数。

&emsp;&emsp;然后我们再用``until``在有错误的函数中不断找哪一行有错误即可。

## VII 删除断点和使断点无效

&emsp;&emsp;``d 断点编号``：删除编号对应断点：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222211248.png)

&emsp;&emsp;``disable 断点编号``：不删除断点但使断点失效：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222211627.png)

&emsp;&emsp;``enable 断点编号``：启动断点。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222231820.png" style="zoom:80%;" />

## VIII 其他

&emsp;&emsp;``bt`` 显示堆栈:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222211744.png)

&emsp;&emsp;``set var 变量名=值``，设定变量值，比较少用。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222211934.png)

&emsp;&emsp;``print p``:打印表达式p的值，这里的表达式也可以改变变量的值。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222232144.png)

# 7 Linux项目自动化构建工具—make和Makefile

## I 什么是make和Makefile

&emsp;&emsp;make是一条命令，Makefile是一个文件。

&emsp;&emsp;make和Makefile主要来处理文件之间的依赖关系。

&emsp;&emsp;在VS中我们VS会帮我们维护好文件之间的依赖关系，Linux下，我们需要写Makefile来确定文件之间的依赖关系。

&emsp;&emsp;Makefile中主要构建了**依赖关系**和**依赖方法**。

&emsp;&emsp;当项目的文件很多的时候，如果直接使用gcc编译会非常麻烦，如果我们写好Makefile文件，可以直接快速的编译。

&emsp;&emsp;什么叫依赖关系和依赖方法呢？给我妈打电话说：“妈，我是你儿子“，这个就是表述了一个依赖关系，你依赖于你妈妈，但是这个依赖关系并没有什么用，你妈妈并不知道你打这个电话表明依赖关系干嘛，如果说：”妈，我是你儿子，到月底了，打钱“，打钱就是一个依赖方法。

&emsp;&emsp;这也表明了依赖关系和依赖方法必须成对出现才有用。

## II 一个简单的Makefile

&emsp;&emsp;``mytest``由``mytest.c``形成，所以mytest依赖于mytest.c,第一行写上``mytest:mytest.c``.

&emsp;&emsp;仅表明依赖关系是没用的，我们还得交待依赖方法：

&emsp;&emsp;在第二行先以tab健开头，然后加上``gcc -std=c99 -o mytest mytest.c``.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222214148.png)

&emsp;&emsp;然后直接执行make命令即可，也可以``make 目标名``，直接``make``默认执行第一个目标。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222214116.png)

&emsp;&emsp;``make``的作用就好像VS中的自动生成解决方案，因为VS把文件之间的依赖关系都已经写好了，这里``Makefile``要求我们自己写好文件之间的依赖关系以生成解决方案。

&emsp;&emsp;VS中还有自动清理解决方案，在``Makefile``中，我们利用``.PHONY:符号名``，它的作用是修饰符号名，使得符号名变成一个伪目标，并且可以无限执行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222214954.png)

&emsp;&emsp;我们的普通目标，在生成后，如果检查到已经是最新的了，系统不会允许我们更新：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222215059.png)

&emsp;&emsp;但是``make clean``却可以一直执行。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222215222.png" style="zoom:80%;" />

## III 一个稍微复杂一点的Makefile

&emsp;&emsp;我们维护一个项目，通过一个源文件``hello.c``生成``hello.i``,通过``hello.i``生成``hello.s``,通过``hello.s``生成``hello.o``,通过``hello.o``生成``hello``可执行文件。

&emsp;&emsp;我们最终的目标是``hello``，它由``hello.o``生成，所以它依赖于``hello.o``，``hello.o``依赖于``hello.s``，``hello.s``依赖于``hello.i``,``hello.i``依赖于``hello.c``,因此，Makefile文件可以这样写：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222221532.png)

&emsp;&emsp;执行``make``：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222221953.png)

&emsp;&emsp;执行清理项目文件：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220222222247.png)

&emsp;&emsp;一个管理堆的oj的debug版本和release版本的代码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220223002501.png)

## IV make的特殊符号

``$@``和``$^``:``$@``的含义是替换为本条命令的目标，``$^``的含义是默认替换成**本目标所依赖的文件列表**。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226143934.png)

用``$@``和``$^``显示所有编译细节：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226144502.png)

# 8 Linux上的第一个小程序—进度条

## I 回车和换行与缓冲区

&emsp;&emsp;回车和换行其实是一个分开的概念：

&emsp;&emsp;回车的含义是回到本行的起始位置，换行的含义是列位置不变，新起一行。

&emsp;&emsp;下面一个代码，先打印一个字符串，然后等待5s：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226145251.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226145155.png)

&emsp;&emsp;但是如果我们的字符串里头没有'\n'就会变成先等待五秒然后输出这个字符串：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226145508.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226145429.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226145443.png)

&emsp;&emsp;我们知道根据c语言循环语句的特性，sleep一定是比printf运行晚的，也就是说printf肯定已经执行，但是数据没有立刻刷新到显示器上，所以结论就是没有'\n'，字符串会暂时保存起来，而不会直接刷新到显示器上。

&emsp;&emsp;这个保存起来的区域就是**用户C语言级别的缓冲区**，显示器设备的刷新策略就是行刷新，即缓冲区遇到\n才刷新。

&emsp;&emsp;那我们如果就是想直接刷新呢，我们可以用``fflush``接口。

&emsp;&emsp;C程序默认会打开三个输入输出流：``stdin``、``stdout``、``stderr``,分别对应标准输入(键盘)、标准输出(显示器)、错误.

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226150122.png" style="zoom:80%;" />

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226150034.png" style="zoom:80%;" />

&emsp;&emsp;因为我们写程序的目的就是读取数据进行计算与显示，所以C程序默认打开``stdin``和``stdout``，又因为写代码很可能会写出一些错误，所以需要打开``stderr``,可以用``perr``等来把错误信息打印出来。

所以加上``fflush``即可直接把字符串刷新在标准输出(显示器)上：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226150716.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226150633.png)

如果要对第一个位置的数不停的刷新覆盖，可以打印一个数，然后回车回到开头第一个位置而不换行，即我们不换行而只想回车，反复打印数即可，在C语言中，``\r``就是只回车不换行，据此，我们就实现了一个简易的倒计时：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226151252.png" style="zoom:80%;" />

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226151359.png)

如果想完成10到0的倒计时，由于10会占两个字符，所以我们需要给它预留空间：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226151650.png" style="zoom:80%;" />

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226151538.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226151600.png)

## II 进度条的实现

所以进度条也可以打印1个#，然后回车打印两个#，重复。。。如果想打印两个#，只要让h后一个字符也变成#，然后回车但不换行输出h即可

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226152406.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226152305.png)

那如果想形成以下模样呢？

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226152710.png)

首先考虑用一个[]来装载#,但是因为右边还要有一个方块来承载数字，因为右边同时还有一个方框来承载一个数字，所以我们需要给**100个#预留空间**，可以用C语言中的格式化字符串中的%-100来实现，至于右边的百分比，同样可以先是用%-3d向右预留出3个空位，打印数字加`%`

我们还可以实现一个光标来表明当前进度条正在加载，可以利用一个组成为``label = "|/-\\";``的字符串(这里\\是转义\),然后利用``i%4``不断输出变化的字符。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226153434.png" style="zoom:80%;" />

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226153413.png)

另外，我们还可以通过printf调整字体的配色方案，百度printf的配色方案即可，这里总结了[xiao zhou](https://blog.csdn.net/cdzg_zzk/article/details/120068641?spm=1001.2014.3001.5502)的博客：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226211314.png" style="zoom:80%;" />

我们修改后的输出样式如图：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226213848.png)

```cpp
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main()
{
#define NUM 100
    char bar[NUM + 1];
    const char* label = "|/-\\";
    memset(bar, 0, sizeof(bar));
    int i = 0;
    while (i <= 100)
    {
        printf("[\033[1;40;37m%-100s\033[0m][\033[1;44;32m%-3d%%\033[0m]\033[1;43;34m%c\033[0m\r", bar, i, label[i % 4]);
        fflush(stdout);
        bar[i] = '#';
        i++;
        usleep(50000);
    }
    printf("\n");
    return 0;
}
```



# 9 命令行git的使用

## I 下载git

使用``git --version``检查git版本，若未安装``git``，则``sudo yum -y install git``安装git。

## II 在gitee创建仓库

然后在``Gitee``新建仓库，注意事项：设置模板最好设置上，后续的两个东西对我们学习入门级别的git使用不需要设置，分治模型也不需要设置。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154040.png" style="zoom:80%;" />

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154255.png)

创建好后修改代码仓库为开源的

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154710.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154722.png)

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154645.png" style="zoom:80%;" />

## III 本地git操作

克隆到本地：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226154600.png)

[记得要把提交邮箱改为公开](https://gitee.com/profile/emails)，或者使用在这个网址中对应的加密邮箱地址。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226160554.png" style="zoom:80%;" />

然后一样是add commit push三板斧

``git add 文件名``:搞上你要提交的文件

``git commit -m "提交日志"``：提交改动到本地，

一般``git commit``之前需要设置你是谁，设置方式为如下，这样设置这会使得提交信息同步到日志之中：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226161510.png" style="zoom:80%;" />

``git push``:把你的提交同步到远端服务器上，然后需要输入你的gitee账号和密码。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226161404.png" style="zoom:80%;" />

``git log``可以查看git提交的日志：

 <img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220226162221.png" style="zoom:80%;" >





 



