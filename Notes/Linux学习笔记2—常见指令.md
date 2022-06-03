# pwd指令

&emsp;&emsp;显示用户当前所在的目录。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122164111.png)

&emsp;&emsp;这里的``/``是路径分割符，``Windows``下使用的是反斜杠：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122120726.png)

# ls指令

**语法**：``ls[选项][目录或文件]``

**功能**：对于目录，该命令累出该目录下所有的子目录与文件。对于文件，将列出文件名与其他信息。

## 1 ls

&emsp;&emsp;直接使用``ls``会查看当前目录下有那些文件（只显示文件名）：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122164323.png)

## 2 ls -l

&emsp;&emsp;``ls - l``会以列表形式显示文件的更多属性，空文件里也有空间，因为要标记这个文件的文件名、创建时间、文件大小等等，

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122164449.png)

&emsp;&emsp;从``Router``起分别是文件所属用户、文件大小、最后更改时间。

## 3 ls -a

&emsp;&emsp;``ls - a``会追加显示隐藏文件：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122164701.png)

&emsp;&emsp;``ls -a``和``ls -l``可以合并使用，显示隐藏文件和普通文件的更多属性：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122165223.png)

&emsp;&emsp;这里``d``开头的是目录文件(Linux下一切都是文件),``-``开头的是普通文件。

&emsp;&emsp;下面简单介绍Linux下的隐藏文件，Linux下，``.``开头的文件就是隐藏文件，``ls -a``其实和``windows``下的这个功能类似：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122122140.png)

&emsp;&emsp;这里的``.``是当前路径的目录文件，``..``是上一级的目录文件，证明如下：

&emsp;&emsp;补充一下，``cd 路径名``可以进入对应路径，我们配合``pwd``来看看:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122165447.png)

&emsp;&emsp;所以上篇博客里的``./a.out``就是告诉系统运行的``a.out``是当前路径下的``a.out``

&emsp;&emsp;根目录，我们一直``cd ..``回退到上一级目录，发现到了一个``/``就回退不动了：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122123007.png)

&emsp;&emsp;这个``/``称为Linux下的根目录，大部分的操作系统组织文件的数据结构是一颗多叉树，自然也有根的概念。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220122123154.png)

&emsp;&emsp;如果想要按照数装结构展示文件，可以安装``tree``

## 4 ls -R

&emsp;&emsp;首先化身``root``,然后``yum install -y tree``，如果失败了，说命名要更新软件包，运行``yum install -y install epel-release``，然后再``yum install -y tree``,然后可以执行``tree``功能：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123203603.png)

&emsp;&emsp;默认是以当前目录以根节点然后展示树装文件结构。后面也可以跟绝对路径。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123203831.png)

&emsp;&emsp;还可以用``ls -R 目录名``递归的展示子目录及文件信息，作用与``tree``类似，只有图形部分不太一样：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123205202.png)

## 5 ls -d

&emsp;&emsp;默认情况下，``ls``指令作用于目录会显示目录里面的内容，而非目录的信息本身，验证如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123204401.png)

&emsp;&emsp;如果你想看的是这个目录的信息而非目录本身，就可以使用``ls -d``(一般还要追加上``-l``以显示详细信息)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123204801.png)

## 6 其他备注选项

&emsp;&emsp;其他选项备注如下：

- ``-i`` 输出文件的 i 节点的索引信息。 如 ls –ai 指定文件
- ``-k`` 以 k 字节的形式表示文件的大小。ls –alk 指定文件
- ``-n`` 用数字的 UID,GID 代替名称。 （介绍 UID， GID）
- ``-F`` 在每个文件名后附上一个字符以说明该文件的类型，“*”表示可执行的普通文件；“/”表示目录；“@”表 示符号链接；“|”表示FIFOs；“=”表示套接字(sockets)。（目录类型识别）
- ``-r`` 对目录反向排序。
- ``-t`` 以时间排序。
- ``-s`` 在l文件名后输出该文件的大小。
- ``-1`` 一行只输出一个文件。

# which和alias

&emsp;&emsp;使用``which``可以查看特定命令处于的路径：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123210942.png)

&emsp;&emsp;这些指令本身其实也是程序，我们可以用ls指令查看如下:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123211241.png)

&emsp;&emsp;查看详细信息后，发现这个指令程序本身就是一个可执行文件：``Windows``下的``.exe``

&emsp;&emsp;``alias``的意思是别名，``which ls``回答我们的含义就是我们实际执行的指令是：``ls --color=auto``,意味输出带默认颜色。

&emsp;&emsp;我们可以使用``alias``指令自己定制别名：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123212025.png)

&emsp;&emsp;修改后的别名仅在这次会话有效，即仅在本次登陆有效。尽量不要永久定义别名，会污染别名池。

# cd指令—更改当前路径

**绝对路径与相对路径**：

&emsp;&emsp;在文件多叉树中，叶子结点通常都是空结点或者是普通文件，目录文件一般是非叶子结点。

&emsp;&emsp;几乎所有操作系统管理文件的方式都是采用多叉树的形式，目录这种目录里放目录或文件天然符合多叉树的特点。

&emsp;&emsp;为什么我们可以采用""路径“方案定位或指定特定的文件呢？``/usr/bin/cd``

&emsp;&emsp;路径回答的应该是指定的文件在哪里+文件是谁，如上面``/use/bin/``是在哪里，``cd``表示这个路径下的名字

&emsp;&emsp;为什么这种定位方式在多叉树有**唯一性**呢？

&emsp;&emsp;因为每一个结点都只有一个父节点，每一个结点都可以不断``cd ..``回到根目录。因为每一个结点只有一个父节点，所以回溯从叶子结点回溯到根目录具有唯一性，所以从根目录开始，一直访问到对应的叶子结点也是具有唯一性的。

&emsp;&emsp;这种从根目录开始到达叶子结点的路径我们称为**绝对路径**.

&emsp;&emsp;绝对路径具有唯一性，绝对不会出错，任何时候都是有效的。

&emsp;&emsp;下面来看看相对路径，下面是一个Linux下的典型相对路径：

```shell
[Router@iZ2vc7mqno8ww3xkmg27snZ 2022_1_23]$ ls ../2022_1_22/test.c
../2022_1_22/test.c
```

&emsp;&emsp;``..``表示上级目录``/home/Router``,然后再进入``/2022_1_22``找到``test.c``

&emsp;&emsp;不以根目录``/``开始的路径称为相对路径，意为相对当前我所处目录的路径。

```shell
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ pwd
/home/Router
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ cd 2022_1_23
[Router@iZ2vc7mqno8ww3xkmg27snZ 2022_1_23]$ pwd
/home/Router/2022_1_23
[Router@iZ2vc7mqno8ww3xkmg27snZ 2022_1_23]$ cd ../../
[Router@iZ2vc7mqno8ww3xkmg27snZ home]$ pwd
/home
```

&emsp;&emsp;由于出于不同目录下时，不同目录的上级目录很有可能不同，显然相对路径并不具有唯一性，显然从不同目录开始要找某个文件，有很多条不同的相对路径都能找到这个文件，所以相对路径并不能唯一的标识一个文件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123095737.png)

&emsp;&emsp;这里``./a.out``使用的就是一种简单的相对路径。

&emsp;&emsp;一般都使用这种相对路径来执行文件，绝对路径一般用来配置一些文件，因为它在任何路径下具有唯一性。

``cd``指令语法：

用法1 ：``cd 绝对路径``使用绝对路径进行路径切换。

用法2 ：``cd ../``或``cd ./``使用相对路径进行路径切换

用法3：直接``cd + 目录名``默认表示就在当前目录下找这个目录名。

用法4：``cd - ``,``-``会记录我最近从哪个路径来的，``cd -``进入我最近一次来的地方。 

```shell
[Router@iZ2vc7mqno8ww3xkmg27snZ home]$ cd /home/Router
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ 
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ pwd
/home/Router
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ cd /
[Router@iZ2vc7mqno8ww3xkmg27snZ /]$ pwd
/
[Router@iZ2vc7mqno8ww3xkmg27snZ /]$ cd -
/home/Router
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ pwd
/home/Router
```

用法5：``cd ~``，这里的``~``储存当前用户的工作目录，``cd ~``进入特定用户的工作目录

```shell
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ cd /
[Router@iZ2vc7mqno8ww3xkmg27snZ /]$ cd ~
[Router@iZ2vc7mqno8ww3xkmg27snZ ~]$ pwd
/home/Router
```

**工作目录**：

&emsp;&emsp;用户登陆的时候就会默认进入这个用户的工作目录，统一用``~``表示

&emsp;&emsp;普通用户的工作目录：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123101233.png)

&emsp;&emsp;root用户的工作目录：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123101528.png)



&emsp;&emsp;普通用户默认所处的工作路径一定是``/home/用户名``;

&emsp;&emsp;root用户对应的工作路径一定是``/root``;

&emsp;&emsp;windows下的用户工作路径：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123101644.png" style="zoom:80%;" />

&emsp;&emsp;里头有我们这个用户的桌面。

# touch指令—创建文件更新文件时间信息

&emsp;&emsp;``touch``指令可以创建普通文件，语法``touch 文件名``。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123101850.png)

&emsp;&emsp;touch还可以更改文件的最近修改时间。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123101959.png)

&emsp;&emsp;查看文件属性：``stat 文件名``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123104123.png" style="zoom:80%;" />

&emsp;&emsp;这里的Access Modify Change分别是:

- Access : 文件最近一次被访问的时间


- Modify: 文件内容最近一次被修改的时间


- Change: 文件属性最近一次被改变的时间

&emsp;&emsp;下面做一些验证：

&emsp;&emsp;使用``cat``指令访问内容，不会修改文件内容和文件属性，只有Access改变了。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123102212.png)

&emsp;&emsp;使用``date >> myfile.txt``(下面会介绍)直接把日期信息加入文件而不访问文件;

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123104407.png)

&emsp;&emsp;可以发现Access没变，但是Modify和Change变了

&emsp;&emsp;使用``mv``指令(下面会介绍)改名，修改``Change``属性，但是Access和Modify不会改变：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123104651.png)

&emsp;&emsp;``touch``一下观察更改情况：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124180428.png)

&emsp;&emsp;发现``touch``默认更改所有时间属性。

&emsp;&emsp;总结如下：``touch``命令可以直接修改文件的时间属性

- ``touch -a 文件名``,将access，change更改为系统时间
- ``touch -m 文件名`` 将modify，change更改为系统时间
- ``touch 文件名`` 将同时刷新access，modify，change

# mkdir—创建文件夹

&emsp;&emsp;默认语法：``mkdir 文件夹名``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123102846.png)

&emsp;&emsp;``mkdir``还可以创建多级目录（创建路径）：

&emsp;&emsp;``mkdir -p d1/d2/d3/d4``(其父目录不存在则先创建父目录)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123103044.png)

&emsp;&emsp;在文件树上，创建文件本质就是插入叶子结点的过程。

&emsp;&emsp;安装``tree``命令：``tree 目录名``是以树装结构查看目录内的信息，如下

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123103631.png)

&emsp;&emsp;先变身root，然后``yum install -y tree``，如果失败，说明要更新软件包，``yum install -y install epel-release``

&emsp;&emsp;``useradd``是一条系统命令，``adduser``是一个普洱脚本，所以尽量用``useradd``;

&emsp;&emsp;``/etc/passwd``里头没有密码，``/etc/shadow``保存着每个用户的密码信息，不过是加密的。

# rmdir/rm—删除文件

&emsp;&emsp;``rmdir 目录名``删除空目录，其实就是删除目录文件本身，如果目录非空会报错：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124170558.png)

&emsp;&emsp;``rm -r 目录名``，递归的删除这个目录以及这个目录内的文件，类似后续遍历删除二叉树，就可以删除非空目录。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123110006.png)

&emsp;&emsp;``rm 文件名``默认情况下，可以直接删除文件.

&emsp;&emsp;``rm -f 文件名``强制删除，不给我们问什么这个文件不是你的什么乱起八糟的，不过非``root``用户权限有限制的，有的文件删除不了。

&emsp;&emsp;``rm -rf 目录文件名``强制递归删除，删库跑路专用（狗头），使用时一定要知道自己在干什么，删除结果能否接受，不要删重要文件，尤其不要轻易在``root``用户上使用，也不是立马能删掉，有的文件可能正在使用。

&emsp;&emsp;``rm * -rf或rm -rf *``清空当前目录。

# man指令（帮助手册）

&emsp;&emsp;Linux的命令有很多参数，我们不可能全记住，我们可以通过查看联机手册获取帮助。访问Linux手册页的命令是 man ，也就是问问这个男人(man)。

&emsp;&emsp;语法: man [选项] 命令，然后按回车往下翻。

&emsp;&emsp;``man man``，查看man指令怎么使用。

&emsp;&emsp;如果某个指令man一下报错了，说明需要更新man，执行下面的指令``yum install -y man-pages``

&emsp;&emsp;``man``的选项这里我们介绍一些数字，``man number 查找的名字``就是在对应范围下查找，如``man 3 printf``查找库函数里头的``printf``使用手册。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123111123.png)

&emsp;&emsp;最常用的编号：1 命令 2 系统调用 3 库函数调用

# cp指令（复制粘贴）

&emsp;&emsp;语法：``cp src des``默认拷贝到当前路径下

&emsp;&emsp;普通拷贝只能拷贝普通文件,无法拷贝目录中全部文件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123112136.png)

&emsp;&emsp;``cp -r``递归拷贝，拷贝目录及目录中全部文件.

&emsp;&emsp;``cp -f``拷贝时不被提问是否真的要拷贝别人的文件。

&emsp;&emsp;一般用``cp -rf``直接拷贝目录

# mv命令（剪切命令+重命名）

&emsp;&emsp;语法: mv [选项] 源文件或目录 目标文件或目录 

&emsp;&emsp;功能:  

1. 视mv命令中第二个参数类型的不同（是目标文件还是目标目录），mv命令将**文件重命名**或**将其移至一个新的目录**中。

2. 当**第二个参数类型是文件**时，``mv``命令**完成文件重命名**，此时，源文件只能有一个（也可以是源目录名），它将所给的源文件或目录重命名为给定的目标文件名。 
3. 当第二个参数是已存在的目录名称时，源文件或目录参数可以有多个，mv命令将各参数指定的源文件均移至目标目录中。

&emsp;&emsp;常用选项：

- ``-f`` ：force 强制的意思，如果目标文件已经存在，不会询问而直接覆盖 
- ``-i`` ：若目标文件 (destination) 已经存在时，就会询问是否覆盖

&emsp;&emsp;剪切普通文件测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123112754.png)

&emsp;&emsp;剪切文件夹测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123112937.png)

&emsp;&emsp;普通文件重命名测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123112708.png)

&emsp;&emsp;目录文件重命名测试：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185643.png)

&emsp;&emsp;还可以利用mv创建回收站，原理就是将``rm``改造成``mv``

```shell
将rm改造成mv
vim ~/.bashrc #修改这个文件
mkdir -p ~/.trash
alias rm=trash
alias ur=undelfile
undelfile() {
 mv -i ~/.trash/\$@ ./
}
trash() {
 mv $@ ~/.trash/
}
```

# cat指令——文本内容输出

&emsp;&emsp;假如我们有一个利用命令行脚本制作测试用的大文件：

```shell
count=0; while [ $count -le 1000 ]; do echo "hello ${count}"; let count++; done > file.txt
```

&emsp;&emsp;可以``cat 文件名``把文件内容输出到标准输出上。

&emsp;&emsp;查看文件内容，而非有修改权限，``cat -n``可以带上行号。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123113739.png)

&emsp;&emsp;``tac``输出的和cat命令顺序相反，``tac``：逆序输出文本内容，cat：正序输出文本内容。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123113718.png)

&emsp;&emsp;发现问题，大文本内容太多了，直接``cat``会刷屏。

&emsp;&emsp;为了进行大文件查看，我们引入下面的命令。

# more命令——常用于大文本文件查看

&emsp;&emsp;默认功能：对文本分批查看，按回车向下看，按q回去。

&emsp;&emsp;``more -n``带行号查看。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185625.png)

&emsp;&emsp;``more -5 文件名``先只看5行;

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185559.png)

&emsp;&emsp;``more``的查找功能：

&emsp;&emsp;在more中输入``/字符串``，就可以找到对应字符串附近的内容，不过只能向下查找：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185544.png)

&emsp;&emsp;但是more只能下翻,只能向下查找，如果看过了想回头没法回头，还是很不方便。

# less指令——更推荐的大文本文件查看

&emsp;&emsp;好处：可以按键盘上的上下来上下翻，而且可以向上搜索或者向下搜索，所以更推荐less

&emsp;&emsp;语法： less [参数] 文件 

&emsp;&emsp;功能： less与more类似，但使用less可以随意浏览文件，而more仅能向前移动，却不能向后移动，而且less在查看之前 不会加载整个文件。 

&emsp;&emsp;``less``的查找验证：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185523.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185458.png)

选项： 

- -i 忽略搜索时的大小写 
- -N 显示每行的行号 
- /字符串：向下搜索“字符串”的功能 
- ?字符串：向上搜索“字符串”的功能 
- n：重复前一个搜索（与 / 或 ? 有关） 
- N：反向重复前一个搜索（与 / 或 ? 有关）
- q:quit

&emsp;&emsp;这里的主要大文件查看需求用于查看日志和写好的大规模代码。

&emsp;&emsp;日志是什么呢？

&emsp;&emsp;系统中存在各种日志，日志就是保存一些软件运行过程中的运行数据，写程序一般要带日志，比如用``ls /var/log/``查看日志文件：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185427.png)

&emsp;&emsp;如看一下``yum.log``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124185816.png)

&emsp;&emsp;日志就是一些保存软件运行过程中对应的运行数据的文本文件，有时候会很长，使用``less``或``more``查看为好，一般的软件都带着日志信息。

# head和tail命令—文件的局部查看

&emsp;&emsp;``head 文件名``默认查看前10行，``head -数字 文件名``查看前数字行。

&emsp;&emsp;``tail 文件名``默认查看后10行，``tail -数字 文件名``查看后数字行。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123114935.png)

&emsp;&emsp;如何能查看中间区域呢？假设要查[100,120]行。

&emsp;&emsp;``echo``指令默认输出空行，``echo 字符串``可以把字符串打印到标准输出上。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123115220.png)

&emsp;&emsp;可以按上或者按下找历史命令。

&emsp;&emsp;我们把``echo``的输出不弄到标准输出去，弄到文件中，这类操作叫做**输出重定向**，如果目标文件不存在，会自动创建文件，如下：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123115334.png)

&emsp;&emsp;本来应该显示到标准输出的内容重定向写入了文件中。

&emsp;&emsp;这条指令会覆盖原本的文件内容.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123115631.png)

&emsp;&emsp;输出重定向会清空原始文件内容，然后重新写入。

&emsp;&emsp;如果不想覆盖原本的内容，可以把``>``改为``>>``,这类操作称为**追加重定向**，

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123115847.png)

&emsp;&emsp;``cat``不加参数时，默认从标准输入，即键盘读取内容：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124190816.png)

&emsp;&emsp;同样也有输入重定向的操作：``cat < log.txt``把文件的内容作为原始数据输入到``cat``去。

```shell
cat < log.txt
```

&emsp;&emsp;把``log.txt``输入到``cat``的默认参数标准输出。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123120151.png)

&emsp;&emsp;有了输入重定向，我们就可以查看局部文件了，原理就是先把结尾的行号加进去，``hear file.txt -120 > tmp.txt``

&emsp;&emsp;然后``tail -20 tmp.txt``就能得到到指定范围。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123120904.png)

&emsp;&emsp;如果不想要创建中间文件呢？可以利用管道，这是一种进程互相通信的工具。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123120809.png)

&emsp;&emsp;或者这样写：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123121007.png)

&emsp;&emsp;正如现实中的管道一样，管道是用来传输资源的，这里的资源指数据，管道是有入口和出口的，管道可以把一个进程输出的数据交给另一个进程作为输入数据。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123121409.png)

&emsp;&emsp;使用管道时隐式的发生了重定向，本来``head -120 file.txt``要写到标准输出去，然后写到了管道里，本来``tail -20``要从标准输入或文件读的，从管道里读了,使用管道时，默认隐式发生了重定向。

# date命令——显示时间相关的命令

&emsp;&emsp;``date``显示年月日时分秒，采用的是格林威治形式显示：![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124191357.png)

&emsp;&emsp;date 指定格式显示时间： ``date +%Y:%m:%d``,加后面是一个格式化字符串，特殊符号有以下含义：

- %H : 小时(00..23)  
- %M : 分钟(00..59)  
- %S : 秒(00..61)  
- %X : 相当于 %H:%M:%S  
- %d : 日 (01..31)  
- %m : 月份 (01..12)  
- %Y : 完整年份 (0000..9999)  
- %F : 相当于 %Y-%m-%d

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123122009.png)

&emsp;&emsp;时间戳：Unix时间戳（英文为Unix epoch, Unix time, POSIX time 或 Unix timestamp）是从1970年1月1日（UTC/GMT的 午夜）开始所经过的秒数，不考虑闰秒，一种计算机计时的方法。

&emsp;&emsp;把当前默认时间转化为时间戳：``date +%s``

&emsp;&emsp;时间戳转化为时间:``date +格式字符串 -d@时间戳``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123122408.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220123122608.png)

&emsp;&emsp;程序的日志其实就像人的日记，突然不写日志了，就可能就是服务器挂掉了。日志中也会记录自己出错的时候，为什么出错了。程序员可以通过查看日志来修服务器。

&emsp;&emsp;显然日志中需要涵盖时间信息，普通时间信息显然是给程序员看的。

&emsp;&emsp;但计算机保存时间信息是用时间戳的，这种日志是程序写的，时间戳是确定且唯一且递增的，所以计算机使用这个时间戳作为日志的时间信息，我们程序员就可用时间戳，加上两个时间转化为时间戳后，作为key值利用head、tail来查看所搜索范围的日志。

&emsp;&emsp;时间戳存在溢出风险，2000年曾经出现溢出风险，千年虫问题，如果时间乱了，是否有可能火车等出错。

# 其他常见命令补充

``uname -r``查看Linux内核版本信息

``vmstat``  报告关于内核线程、虚拟内存、磁盘、陷阱和 CPU 活动的统计信息

``sar`` 主要帮助我们掌握系统资源的使用情况，特别是内存和CPU的使用情况

``stat`` 用于显示文件的状态信息

具体都可以问``man``

``top``:查看cpu资源使用状态

``netstat``: 查看网络连接状态

``free``: 查看内存资源状态

``df``: 查看磁盘分区资源状态

# cal指令——日期或日历命令

&emsp;&emsp;默认情况下会显示本年当月的日历信息：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124133132.png)

&emsp;&emsp;``cal -y 年份``会把对应年份的日历显示出来(默认就是带``-y``的)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124133359.png)

&emsp;&emsp;``cal -3``会把当前月，前一个月，后一个月的日历显示出来。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124133456.png)

# find命令—查找文件

&emsp;&emsp;``which``本身就是一个查找可执行文件的命令。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124133921.png)

&emsp;&emsp;指令、程序、命令、工具这些东西的本质都是可执行程序，``windows``下的``.exe``,``Linux``下的文件属性为可执行文件的程序。

&emsp;&emsp;**最常用语法**：``find 路径 -name``，根据名称查找指定文件。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124134814.png)

&emsp;&emsp;第一次查找可能会很卡，因为这条命令是会真的访问一个一个磁盘目录查找的，后续查找可能存在缓存，所以查找会很快。

&emsp;&emsp;具体语法：``find   路径   选项   [   -print ]   [ -exec   -ok   command ]   {} \;``

&emsp;&emsp;如查找根目录下的所有以.c为后缀的文件：

```shell
find / -name '*.c'
```



&emsp;&emsp;find 根据下列规则判断 path 和 expression，在命令列上第一个 - ( ) , ! 之前的部份为 path，之后的是 expression。如果 path 是空字串则使用目前路径，如果 expression 是空字串则使用 -print 为预设 expression。

&emsp;&emsp;``expression`` 中可使用的选项有二三十个之多，在此只列出最常用的部份。

- ``-mount, -xdev`` : 只检查和指定目录在同一个文件系统下的文件，避免列出其它文件系统中的文件
- ``-amin n`` : 在过去 n 分钟内被读取过
- ``-anewer file`` : 比文件 file 更晚被读取过的文件
- ``-atime`` n : 在过去n天内被读取过的文件
- ``-cmin n`` : 在过去 n 分钟内被修改过
- ``-cnewer file`` :比文件 file 更新的文件
- ``-ctime n`` : 在过去n天内被修改过的文件
- ``-empty`` : 空的文件
- ``-gid n or -group name`` : gid 是 n 或是 group 名称是 name
- ``-ipath p, -path p ``: 路径名称符合 p 的文件，ipath 会忽略大小写
- ``-name name, -iname name`` : 文件名称符合 name 的文件。iname 会忽略大小写
- ``-size n`` : 文件大小 是 n 单位，b 代表 512 位元组的区块，c 表示字元数，k 表示 kilo bytes，w 是二个位元组。
- ``-type c`` : 文件类型是 c 的文件。
  - d: 目录
  - c: 字型装置文件
  - b: 区块装置文件
  - p: 具名贮列
  - f: 一般文件
  - l: 符号连结
  - s: socket

- ``-pid`` n : process id 是 n 的文件

# grep命令—文本行过滤查找工具

&emsp;&emsp;基本语法：``grep 查找内容 文件名``

&emsp;&emsp;Linux下的shell文本查找三剑客：``echo sed grep``,下面介绍``grep``，它是一个行过滤工具。

&emsp;&emsp;``ctrl+r``可以进入一个检索软件，然后输入之间输入过的命令的部分就可以让软件帮我们找到历史命令。

&emsp;&emsp;然后把之前的1000行hello弄进来了，然后我们如果想把包含字符9的行都打印出来，怎么操作呢？

&emsp;&emsp;就可以这样``cat file.txt | grep '9'``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124141325.png)

&emsp;&emsp;这里grep的作用就是行过滤工具。

&emsp;&emsp;也可以不用管道，直接``grep '9' file.txt``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124141536.png)

&emsp;&emsp;``grep -v``反向匹配，``grep -v '9' file.txt``,寻找不包含9的行：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124141742.png)

&emsp;&emsp;``grep -i``:首先利用``nano``修改一下内容：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124142000.png)

&emsp;&emsp;然后分别测试``grep "Router" file.txt``和``grep -i "Router" file.txt``:

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124142110.png)

&emsp;&emsp;``-i``大小写忽略，``-n``带行号。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124142313.png)

&emsp;&emsp;``grep``是支持正则表达式的。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124142612.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124142836.png)

# zip/unzip—打包压缩

&emsp;&emsp;语法:``zip 目标文件 src`` 压缩；``unzip 压缩文件名``解压

&emsp;&emsp;实验：压缩``2022_1_24``.

&emsp;&emsp;首先在命令行输入``zip``,如果没有出现下面的信息则运行``yum install -y zip``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124143546.png)

&emsp;&emsp;运行``zip 2022_1_24.zip 2022_1_24``:

![image-20220124143806682](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20220124143806682.png)

&emsp;&emsp;把他放到test里取：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124143858.png)

&emsp;&emsp;使用``unzip``解压,发现目录里什么都没有。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124144128.png)

&emsp;&emsp;因为默认情况下zip只会对默认单个文件进行压缩。

&emsp;&emsp;如果想把目录里的所有文件都压缩上，``zip -r 目标压缩文件名 文件夹`` 对一个目录里面的所有文件进行递归压缩.

```shell
zip -r ziptest.zip 2022_1_24
```

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124145355.png" style="zoom:80%;" />

&emsp;&emsp;解压到指定路径：``unzip 文件名 -d 路径``。

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124145940.png" style="zoom:80%;" />

&emsp;&emsp;Windows中zip结尾的文件上传到Linux中也是支持的，因为它们采用的相同的压缩算法。

# tar—打包压缩

&emsp;&emsp;压缩语法：``tar 选项 目标压缩文件名 文件名``,文件名后缀一般叫``.tgz``

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124150531.png" style="zoom:80%;" />

&emsp;&emsp;``tar``可以只打包不压缩，只要不带压缩算法就行：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124150907.png)

&emsp;&emsp;解压语法：``tar -xzvf 压缩文件名``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124151603.png)



&emsp;&emsp;在不同环境可能增加选项有的要带-有的不用带。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124095515.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124095544.png)

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124095558.png)

&emsp;&emsp;``-z``即采用zip算法压缩。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124095632.png)

&emsp;&emsp;``tar 选项 文件名 -C 路径名``解压文件到指定路径。

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124152038.png)

&emsp;&emsp;打包和压缩的区别：打包仅是把东西放一起，压缩是一种算法把装起来的东西打包的小一点。

# bc指令

&emsp;&emsp;可以进行浮点运算，可以用管道把需要运算的表达式给它：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124100929.png)

# Uname指令

&emsp;&emsp;查看计算机信息与内核版本：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124101140.png)

&emsp;&emsp;``uname -r``查看内核版本。

&emsp;&emsp;-a或–all 详细输出所有信息，依次为内核名称，主机名，内核版本号，内核版本，硬件名，处理器类 型，硬件平台类型，操作系统名称

&emsp;&emsp;内核版本 ``3.10.0***``，3表示主版本，``10``表示次版本,``**``是偶数是表示稳定发行版，次版本号是奇数表示测试版，el7表示它属于``Centos7``,x86_64表示计算机的体系结构，使用的CPU平台是x86_64.

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124101041.png)

&emsp;&emsp;如果要查看发型版本：``cat /etc/redhat-release``

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124101439.png)

&emsp;&emsp;服务器上的操作系统需要给用户提供稳定安全的服务，操作系统版本更新引入新特性很可能带来新问题，所以服务器选择操作系统内核版本选择合适（一般比较低）当前业务且经典的。

# 重要热键介绍

``tab``：如果有指令想不起来了，只记得几个字母，可以输入这几个字母再按tab，tab一下不行就tab两下。就会把以这些字母开头的命令战术出来：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124102910.png)

``ctrl r``:按``ctrl r``后进入下面界面：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124153020.png)

输入指令的前几个字母，它会把你用过的历史指令显示出来：

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124153116.png)

然后随便按一下左右键就会回到命令行

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124153233.png)

``ctrl c``:终止正在运行的程序。

``ctrl d``:退出``Xshell``

补充命令：

``history``可以查看使用过的命令：

<img src="https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124152436.png" style="zoom:80%;" />

``file``查看文件具体信息

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220124153526.png)

# 关机重启

&emsp;&emsp;``halt``关机 ``reboot``重启 ，``shutdown -s``关机

&emsp;&emsp;``-h`` ： 将系统的服务停掉后，立即关机。 

&emsp;&emsp;``-r`` ： 在将系统的服务停掉之后就重新启动 

&emsp;&emsp;``-t`` 秒数 ： -t 后面加秒数，亦即『过几秒后关机』的意思

&emsp;&emsp;因为服务器设计出来就是要一直开着通电使用的，所以云服务器尽量永远不要关机，直接用就行。

&emsp;&emsp;``shutdown -s -t sec``，几秒后关机，``shutdown -a``取消关机计划。

# file指令

功能：辨识文件类型

```shell
-c:详细展示指令执行过程，便于排错或分析程序执行的情形
-z:尝试解读压缩文件的内容
```

![](https://router-picture-bed.oss-cn-chengdu.aliyuncs.com/img/img/20220218110512.png)



