# 一、Linux是什么？

&emsp;&emsp;准确来说，``Linux``指的是``Linux``操作系统内核程序，是操作系统程序的核心部分。``Linux``并非直接的操作系统发行版，它是在90年代芬兰的一位大学生``Linus Benedict Torvalds``,研发的一款仿``UNIX``操作系统内核。

&emsp;&emsp;与``UNIX``不同的是，Linux内核的代码是开源的，并且Linux内核的代码是免费使用的，而非像UNIX一样需要收费使用。由于这个特性，很多的工程师和``hacker``参与进了``Linux``的研发并且为``Linux``内核的迭代提供了很多帮助，使得``Linux``由最早的可能只是``Linus``的一个学习``Minix``制作的操作系统内核"玩具"渐渐变成了拥有强大功能，能为用户提供稳定功能的操作系统内核程序。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122112413.png" style="zoom:80%;" />

&emsp;&emsp;不同于``Windows``操作系统在运行过程中很可能出现蓝屏、死机等情况（开机时间一长这几乎是必然发生的），``Linux``操作系统的运行非常稳定，很少出现死机、宕机的情况，因此``Linux``无疑是十分适合作为需要长时间提供线上服务的互联网公司服务器的操作系统。

&emsp;&emsp;有了操作系统内核后，有些公司在操作内核上再增加一些包管理等应用层程序，为``Linux``提供了很多发型版本，方便大型企业直接部署自己的线上服务，这些版本就是Linux的发型版本。

&emsp;&emsp;下面简单介绍一下``Linux``的几个发型版本.以下参考了文章：[十款最常见的Linux发行版及目标用户](https://www.cnblogs.com/liangxiaofeng/p/3658145.html)

**1. Debian**

&emsp;&emsp;``Debian``运行起来极其稳定，这使得它非常适合用于服务器。

&emsp;&emsp;另外，``Debian``的设计为其他发行版本提供了灵感。``Debian``平时维护三套正式的软件库和一套非免费软件库，这给另外几款发行版（比如``Ubuntu``和``Kali``等）带来了灵感。``Debian``这款操作系统派生出了多个 ``Linux``发行版。它有37500多个软件包，这方面唯一胜过``Debian``的其他发行版只有``Gentoo``。``Debian``使用``apt``或``aptitude``来安 装和更新软件。

&emsp;&emsp;但``Debian``这款操作系统相对其他发行版本设计相对复杂一些，并不适合新手用户，而是适合系统管理员和高级用户。

**2.Ubuntu**

&emsp;&emsp;``Ubuntu``是``Debian``的衍生版本，与``Debian``一样，使用``apt``软件管理工具来安装和更新软件，且它相对更加易用，适合学校研究学习，个人使用。

**3. 红帽企业级Linux**

&emsp;&emsp;这是第一款面向商业市场的Linux发行版。它有服务器版本，支持众多处理器架构，包括x86和x86_64。红帽公司通过课程红帽认证系统管理员/红帽认证工程师（RHCSA/RHCE），对系统管理员进行培训和认证。就全球市场而言，总利润中80%来自支持，另外20%来自培训和认证.

&emsp;&emsp;补充说明：通常认为，开发了该发行版的Marc Ewin将该产品命名为红帽，因为他丢失了似乎很心爱的那顶红色帽子，帽子是他爷爷在他过生日时送的礼物。

**4.CentOS**

&emsp;&emsp;``CentOS``是 一款企业级Linux发行版，它使用红帽企业级``Linux``中的免费源代码重新构建而成。这款重构版完全去掉了注册商标以及Binary程序包方面一个非常细微的变化。有些人不想支付一大笔钱，又能领略红帽企业级``Linux``；对他们来说，``CentOS``值得一试。此外，``CentOS``的外观和行为似乎与母发行版 红帽企业级Linux如出一辙。 ``CentOS``使用``YUM来``管理软件包。

&emsp;&emsp;``Centos``拥有非常稳定的程序包；谁要是想在桌面端测试一下服务器的运作原理，都应该试试这款操作系统。

&emsp;&emsp;出于学习目的考量，本文介绍的发行版本是``Centos 7.6``.

# 二、购买云服务器搭建Linux环境

&emsp;&emsp;想要获得``Linux``环境，一种方法是在``Windows``上装虚拟机，不过这样占用磁盘空间较大，且可能运行时较为卡顿；另一种方法是卸载Windows操作系统，安装一款适配当前架构的``Linux``发行版，但是``Windows``其实是我们比较熟悉的环境，且``Windows``的``GUI``做的还不错，对于日常使用来说十分便捷，在不熟悉``Linux``系统的情况下贸然卸载``Windows``风险很大,并且大部分游戏也是只兼容``Windows``系统，如下图Linus也曾为``win7``盛情点赞：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122141729.png" style="zoom:80%;" />

&emsp;&emsp;还有一个解决方案就是购买云服务器，

&emsp;&emsp;作为学生，我们可以以较低成本获得腾讯云或阿里云或华为云的轻量级，对于学习目的来说1核1G完全足够，我们选择其中最低成本的即可，这里我选择的是阿里云。

&emsp;&emsp;首先需要一些认证才能购买云服务器，这里就不介绍了，按照步骤认证即可。

&emsp;&emsp;选择系统为``CentOS 7.6``,购买后，打开阿里云官网，点击右上角控制台 

&emsp;&emsp;点击轻量应用服务器：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122142507.png" style="zoom:80%;" />

&emsp;&emsp;进入如下界面：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122142632.png" style="zoom:80%;" />

&emsp;&emsp;点击这个购买的服务器，进入下面界面：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122142844.png)

&emsp;&emsp;请记住自己的公网IP和实例ID,且不要透露，以免网络危险，点击重置密码设置一下密码，这里设置的密码就是``root``用户的密码,与公网IP记录在一起，至此，我们就算已经暂时租赁了这台云服务器。

&emsp;&emsp;如果遗忘密码，可以随时回来修改``root``用户的密码。

&emsp;&emsp;下面介绍三个方案利用SSH协议远程链接自己的云服务器。

## 1 Xshell

&emsp;&emsp;可以去Xshell官网获得：[Xshell](https://xshell.en.softonic.com/)

&emsp;&emsp;安装好后打开大概是这个样子：

<img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20220122143858805.png" alt="image-20220122143858805" style="zoom:80%;" />

PS：我设置了背影颜色，在工具选项下的配色方案中可以修改,在A按钮后可以修改字体：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122145817.png)



![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122145908.png)

&emsp;&emsp;输入以下命令：

```shell
ssh root@(你的服务器的公网IP)
```

&emsp;&emsp;它会要求我们输入密码

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122144110.png" style="zoom:80%;" />

&emsp;&emsp;这时输入刚刚在官网设置好的密码就可以了。

&emsp;&emsp;然后我们就进入了与操作系统交互的命令行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122150012.png)

&emsp;&emsp;下面简单讲解一下Linux下的用户管理。

### 1.1 Linux下简单的用户管理

&emsp;&emsp;Linux是一个支持多用户共同操作的系统，其中我们登录的``root``用户是超级用户，具有创建用户与修改用户密码的权限，其他用户并不具有这个权限，对于每个被设置了密码的用户，我们都可以通过类似的SSH方法登陆。

&emsp;&emsp;下面介绍一下怎么创建一个可以利用SSH链接云服务器的用户。

root身份下，输入指令：(这里的test是用户名)

```shell
adduser test
```

使用以下指令可以查看当前都有什么用户：

```shell
ls /home
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122150604.png)

&emsp;&emsp;现在的test是没有密码的，无法通过外界SSH登陆，我们使用指令``passwd test``为``test``用户增加密码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122150751.png)

&emsp;&emsp;输入并确定密码即可。

&emsp;&emsp;另外，root用户修改其他用户密码也可以用这条指令``passwd test``

&emsp;&emsp;删除用户，使用指令``userdel -r test``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122150947.png)

### 1.2 Xshell的复制粘贴

复制:`` ctrl + insert`` (一般在``F12``处，有的笔记本的 ``insert ``需要配合`` Fn ``来按) 

粘贴: ``shift + insert``

 ``ctrl + c / ctrl + v`` 无法在``Xshell``上使用.

## 2 vscode+RemoteSSH插件

&emsp;&emsp;官网下载对应版本的vscode安装包：[Visual Studio Code](https://code.visualstudio.com/)

&emsp;&emsp;打开vscode的拓展，搜索Remote-SSH插件：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122151130.png)

&emsp;&emsp;安装后，左侧会有这个插件：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122151405.png" style="zoom:80%;" />

&emsp;&emsp;点击后点：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122151525.png)

然后点第一个：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122151557.png)

&emsp;&emsp;进入后按如下格式输入:

```cpp
Host 服务名，这个可以随便输入
    HostName 你的IP地址
    User 想登陆的用户名
```

&emsp;&emsp;然后点这个东西链接用户就行

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122151844.png)

&emsp;&emsp;输入对应用户的密码：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122152255.png)

&emsp;&emsp;点这个+新建一个终端：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122152339.png)

&emsp;&emsp;创建结果如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122152429.png)

&emsp;&emsp;然后正常点打开文件夹像普通的vscode一样操作就好了，下面的终端就是命令行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122152600.png)

&emsp;&emsp;这个方法的好处是你可以在云服务器上安装vscode的各种插件，并且很方便的使用vsode的调试功能，官方都有文档，对于C/C++，这里提供如下：[gcc on Linux](https://code.visualstudio.com/docs/cpp/config-linux)

&emsp;&emsp;如果想要记住密码，免密登陆可以去看这篇文章：[使用vscode进行远程炼丹](https://zhuanlan.zhihu.com/p/89662757)

## 3 MobaXterm

&emsp;&emsp;获取``MobaXterm``可以去官网：[MobaXterm](https://mobaxterm.mobatek.net/)

&emsp;&emsp;打开以后大概是这个样子：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122153633.png)

&emsp;&emsp;点``Session``,

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122153726.png" style="zoom:80%;" />

&emsp;&emsp;点``SSH``：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122154759.png" style="zoom:80%;" />

&emsp;&emsp;host输入公网IP地址 username输入要登录的用户名，然后点ok。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122155004.png" style="zoom:80%;" />

&emsp;&emsp;输入用户密码即可。

&emsp;&emsp;然后就ok了

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122154450.png)

&emsp;&emsp;这个的好处是可以直接像``windows``一样图形化的看目录和文件，并且上传文件从服务器下载文件都比较方便：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122155450.png)

&emsp;&emsp;缺点是全英文的，需要英文水平比较好。

# 三、命令行下第一个Hello Linux程序

&emsp;&emsp;在命令行中，``touch 文件名``创建一个文件，然后``nano 文件名``进入编写文件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122161736.png)

&emsp;&emsp;然后``ctrl + x``退出；

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122161812.png)

&emsp;&emsp;按Y保存。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122161853.png)

&emsp;&emsp;然后按回车退出。

&emsp;&emsp;然后输入``gcc hello.c``

&emsp;&emsp;然后输入``./a.out``即可运行

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122162026.png)

