@[toc]
# shell命令以及运行原理

&emsp;&emsp;打开``Xshell``出现的字符交互界面：

![](https://img-blog.csdnimg.cn/img_convert/588e6f7e6c8bea13deeff30e92c54cca.png)

&emsp;&emsp;称为命令行。

&emsp;&emsp;我们后面输入的字符代表的是命令+选项。

&emsp;&emsp;我们的命令就是一些**可执行程序**，验证如下：

![](https://img-blog.csdnimg.cn/img_convert/eab6aca72e408872e0c01b18a265cd89.png)

&emsp;&emsp;广义上，Linux发行版 = Linux内核 + 外壳程序，如下图：

![](https://img-blog.csdnimg.cn/img_convert/6422bfd83ef53fabea505e6fff50a6c4.png)

&emsp;&emsp;通常我们是不能直接与操作系统内核进行沟通的，内核的使用成本很高，需要了解操作系统内核的知识，还有权限约束的问题，还有可能这件操作不允许用户进行或者存在错误，直接与操作系统内核沟通就很危险。

&emsp;&emsp;所以我们要和外壳沟通，Linux下就是shell（命令行解释器），把用户的命令翻译给操作系统内核执行，操作系统内核执行后把结果翻译一下给用户看。另外，shell本身有一些缓存功能，可以对操作系统内核拒绝过的命令做出反应，降低操作系统内核的工作量。

![](https://img-blog.csdnimg.cn/img_convert/19f667a7c067d3305a3965b0377533c7.png)

&emsp;&emsp;对有风险可能会终止shell进程的命令，shell程序会创建子进程为我们执行命令

![](https://img-blog.csdnimg.cn/img_convert/e32df6f27251d229ee7e1bd99680daac.png)

&emsp;&emsp;可以看出shell外壳的作用：

- 传递请求指令，让OS执行命令
- 保护内核

&emsp;&emsp;那bash是什么呢？bash是一个具体的shell程序，shell是一类称为命令行解释器的程序，bash是一个具体的shell程序，``Centos 7.6``使用的shell就是bash。

&emsp;&emsp;如何验证bash是一个进程呢？用指令``ps axj | grep bash``;（ps是一种查看当前进程信息的命令）

![](https://img-blog.csdnimg.cn/img_convert/299328a9f874374fdfd318ea3a09e339.png)

&emsp;&emsp;我们再登陆一个用户，发现又创建了一个bash

![](https://img-blog.csdnimg.cn/img_convert/919a4199682a69a6b118aaf8de6cc89b.png)

&emsp;&emsp;使用杀死进程指令``kill -9 PID``就把另一个bash进程干掉了:



![](https://img-blog.csdnimg.cn/img_convert/0ee660e53b87fa99fb7fe1326e8e1695.png)

# 如何开启当前用户的sudo命令

&emsp;&emsp;有时候部分命令需要``root``权限，这时我们频繁切``root``是非常麻烦的，我们可以做如下配置，使得当前用户可以使用``sudo 命令``使后面的命令在``root``权限下运行，具体操作如下：

&emsp;&emsp;首先``su -``切换为``root``用户，然后``nano /etc/sudoers``,就会打开一个文本文件：

![](https://img-blog.csdnimg.cn/img_convert/2ceb283d5020beb40d147291209b80ab.png)

&emsp;&emsp;按下往下翻，就可以找到这样一行：

![](https://img-blog.csdnimg.cn/img_convert/53c50013c792645df5dcea7fb49027ae.png)

&emsp;&emsp;然后把``root``开头那一行复制一下，粘到空的那一行，然后把``root``修改为对应的用户名，然后把缩进和第一行调成一行的就好了。

![](https://img-blog.csdnimg.cn/img_convert/6cd711d5805194f43b3ee58f0de9fb82.png)

&emsp;&emsp;然后``ctrl x``退出并保存就ok了。

# Linux权限管理

## 1 Linux用户类别

&emsp;&emsp;Linux把用户分为两类，Linux中，默认有两类用户：

&emsp;&emsp;``root``：超级用户，具有非常高的权限，基本不受权限约束。

&emsp;&emsp;普通用户：具有一般权限，需要受权限约束 （只能由超级用户创建用户）。

&emsp;&emsp;``root``密码和普通用户的密码绝对不能相同，普通用户的密码可能经常使用，有暴露风险。

&emsp;&emsp;普通用户切换为``root``用户,使用``su -``

![](https://img-blog.csdnimg.cn/img_convert/69ea8dd0cd8017ad4abe256426c30144.png)

``exit``或``ctrl d``可以回到原本的用户，不建议使用``su -用户名``，因为系统这样又要重新创建bash进程，验证如下：

![](https://img-blog.csdnimg.cn/img_convert/4c00515a3ee95de813cba20d601b9354.png)

&emsp;&emsp;这里写错了，应该是下面的``kill -9``杀死的``su Router``创建的bash;

&emsp;&emsp;kill -9 (su Router)创建的bash后，直接回到了root，然后使用回退，发现su -创建的-bash也回收了。

![](https://img-blog.csdnimg.cn/img_convert/368f676f5e2151009533ade3c08fa15b.png)

&emsp;&emsp;root用户可以随意切换其他用户,执行``su -用户名``,那如果我需要``root``权限但是不想频繁切换用户怎么办呢？

&emsp;&emsp;``sudo``可以临时权限提升，对后面的命令以root的身份运行。

&emsp;&emsp;但是不配置是用不了``sudo``的，因为我们没有添加信任关系：

![](https://img-blog.csdnimg.cn/img_convert/35ba94d501229c4a37933309b5427577.png)

&emsp;&emsp;具体配置方法我们在前面已经讲述。

## 2 Linux的文件权限的理解

&emsp;&emsp;什么是权限：一件事情是否允许被特定的人做。

&emsp;&emsp;首先，权限约束的是人，文件本身具有天然的权限属性：r(读)+w(写)+x(执行)，所以对具体的用户，文件的这三个属性是否允许具体的用户执行，使得文件天然存在权限约束。

&emsp;&emsp;所以权限信息主要有文件属性（不同的人是否可读等）以及修改人和这个文件的关系。

&emsp;&emsp;先认识一下“人”的信息，看一下``ll``的具体信息:

![](https://img-blog.csdnimg.cn/img_convert/4e03ebe3a652d7b75ab8a4ecc3782e4e.png)

&emsp;&emsp;拥有者和其他都好理解，所属组是什么意思呢？

&emsp;&emsp;假如有这样一个需求：我写了一份代码，想让自己能看到，同工作小组的人也可以看，不想给其他人看到，但是公司的同一个部门又是都在同一台Linux服务器上做开发的。

&emsp;&emsp;这一份需求就引出了所属组，所属组存在的目的是为了组内协同。

&emsp;&emsp;``other``并不会展示给我们，因为你不是拥有者也不是所属组，那么全集减去拥有者和所属组剩下的就是``other``咯。

![](https://img-blog.csdnimg.cn/img_convert/c7425e592bfa1d4413a64422b6ca51a2.png)

&emsp;&emsp;补充一下前面的问题，前面不是说Linux下用户类别只有root用户和其他用户吗？拥有者，所属组，other是什么？

&emsp;&emsp;root 和普通用户指的是每一个具体的用户；拥有者，所属组，other指的是一种用户的角色身份；

&emsp;&emsp;第一列的含义：

&emsp;&emsp;第一个字符：Linux**系统**不以后缀区分文件，以属性的第一列的第一个字符作为标识符区分文件类型。

![](https://img-blog.csdnimg.cn/img_convert/3c9494132cb0f3f89c69d538eb3cc934.png)

- -:普通文件[文本，各种动静态库，可执行程序，源程序![](https://img-blog.csdnimg.cn/img_convert/720fa796b79c400d4bd97cc2725bfb0e.png)
- d：目录文件![](https://img-blog.csdnimg.cn/img_convert/90070952bdb41024d918a65108004458.png)

- c：字符设备文件，最典型的如键盘和显示器驱动程序。![](https://img-blog.csdnimg.cn/img_convert/a668eb16764158facbd07c7b2e2b058b.png)

- b：块设备文件，磁盘驱动。![](https://img-blog.csdnimg.cn/img_convert/412ba1976234e75d8786e5768a91cfe1.png)

  补充：v的意思是虚拟出来的，查看磁盘容量：``df -h``

  ![](https://img-blog.csdnimg.cn/img_convert/d9df36462ef1a0d14037516e65796987.png)

- s：套接字，网络中的``socket``

- p：管道文件,通信时经常出现

  ![](https://img-blog.csdnimg.cn/img_convert/c650942012277b184bc3865688ac06f1.png)

  ![](https://img-blog.csdnimg.cn/img_convert/61096f5fa8bd5538e462ecb903d6e5f6.png)

- l：链接文件，软链接。

  ![](https://img-blog.csdnimg.cn/img_convert/f111311ad6b8ac2bdf7a13de8ac80724.png)

&emsp;&emsp;怎么键盘显示器、磁盘这些都是文件啊，因为Linux的设计哲学就是Linux下一切皆文件。

&emsp;&emsp;不过``gcc``仍然以后缀识别文件，因为它不是系统文件，是应用程序。

&emsp;&emsp;了解一下第一列的后9个字符：

![](https://img-blog.csdnimg.cn/img_convert/d781b29dcd660f39185f8172899728a3.png)

&emsp;&emsp;根据文件对每个用户的权限，9个字符中的每一组（以3个为1组）这三个字符分别代表``r(是否具有读)w(是否具有写)x(是否具有写)``。

&emsp;&emsp;``r`` 具有读权限 ；``-`` 无读权限

&emsp;&emsp;``w`` 具有写权限 ；``-``没有写权限

&emsp;&emsp;``x``具有可执行权限 ；``-``没有执行权限

&emsp;&emsp;这里的属性 3个为一组，对应拥有者权限、所属组权限、其他人权限，每个字符对应这个人是否拥有这项权限，就完成了文件的权限管理。

&emsp;&emsp;使用``chmod u-r test.c``更改拥有者的读权限：

![](https://img-blog.csdnimg.cn/img_convert/4415f2998f155858bbb5b036b25eee89.png)

&emsp;&emsp;更改权限后就如果没有对应权限就会发生权限拒绝。

![](https://img-blog.csdnimg.cn/img_convert/145e33bb3c8f1ee0d232689912b56b5b.png)

## 3 Linux文件权限的修改

&emsp;&emsp;如何对应权限的修改？——``chmod  chown/chgrp``

### I chmod—修改拥有者、所属组、其他的rwx权限

&emsp;&emsp;拥有者``chmod u+(-)x file.txt``.

![](https://img-blog.csdnimg.cn/img_convert/91e3881d00137ef9fe3c86f95e3b5fb8.png)

&emsp;&emsp;是永久修改。

&emsp;&emsp;所属组：``chmod g+(-)x file.txt``

![](https://img-blog.csdnimg.cn/img_convert/457d7e6f850b4eb60f2d7395254011fc.png)

&emsp;&emsp;其他人(o)：``chmod o+(-) rwx``

![](https://img-blog.csdnimg.cn/img_convert/a5308eb9e7b5d27f90e54098b0e58f44.png)

&emsp;&emsp;一起改：``chmod u+rwx,g+rwx,o+rwx``或``chmod a+(-rwx)``,a是all的意思。

![](https://img-blog.csdnimg.cn/img_convert/32d0a0680f9e7f6b9eca17a1c533e2a6.png)

&emsp;&emsp;普通文件普通用户限制情况：

![](https://img-blog.csdnimg.cn/img_convert/1e28047b37e66d173822e41034279b64.png)

&emsp;&emsp;测试一下``root``会受限制吗？

![](https://img-blog.csdnimg.cn/img_convert/0401f383d7c7cf8ba8f3f0ab5a4d9a07.png)

&emsp;&emsp;我们把所有的``rwx``权限全部删掉，``root``仍可以读写文件：

![](https://img-blog.csdnimg.cn/img_convert/9f1d0af5246dcf9e5dca0656d9bc604a.png)

&emsp;&emsp;验证得``root``基本上不受权限限制。

&emsp;&emsp;文件没有实际的可执行权限时，``root``也无法执行文件，就算加上了``x``权限，但是一般还是不能执行，因为可能里头的命令找不到，而且可能有一些风险，一般是否为可执行是系统给的，你硬加可能也无法执行。	

![](https://img-blog.csdnimg.cn/img_convert/fe00a81a8dcc4c6cb7ae1baca7cd203b.png)

### II chown—更改文件的拥有者和所属组

&emsp;&emsp;更改文件的拥有者：

&emsp;&emsp;直接把文件给``root``，发现被拒绝。

&emsp;&emsp; ![](https://img-blog.csdnimg.cn/img_convert/de081e5af8038c6c8ade48fa848c67ee.png)

&emsp;&emsp;现实中给别人东西都要别人同意，你把文件给``root``当然也要``root``同意.

&emsp;&emsp;一般解决方案是``sudo``提升权限后给或者切``root``账号给,更改拥有者：``chown 用户名 文件名``

![](https://img-blog.csdnimg.cn/img_convert/ad13e106171956b1713cf13892186659.png)

&emsp;&emsp;更改文件的所属组：``chgrp 组名 文件名``，同样的，直接给的话需要``root``权限

![](https://img-blog.csdnimg.cn/img_convert/ef36396af0a19627e035340d3bdc68ed.png)

&emsp;&emsp;如果此时这个文件属于你，但是这个文件所属组不是你，你可以直接``chgrp``直接改回来

![](https://img-blog.csdnimg.cn/img_convert/a752bd2b5aedc16d6f6bf2b6168af91f.png)

&emsp;&emsp;``su``和``su -``,``su``切超级用户当前路径不变，``su -``会回到``root``的工作路径.

&emsp;&emsp;一次性改拥有者和所属组``chown 拥有者名:所属组名 文件名``

![](https://img-blog.csdnimg.cn/img_convert/048cadfa1b626d0c8cdfd2226d065068.png)

### III 权限rwx表示的八进制方案

&emsp;&emsp;前9列的权限符号表明权限位置是明确的且是**两态**的，可以用1表示有权限，0表示无无权限，如file.txt的权限``rw- --- --x``可以改为``110 000 001``，再转化为八进制数据：``6 0 1``.，这就是权限表述的**八进制方案**，修改权限也可以这样改。

&emsp;&emsp;语法:``chmod 八进制方案 文件名``

![](https://img-blog.csdnimg.cn/img_convert/edb5bfb5e589d3dd3d1dc25c856e279e.png)

&emsp;&emsp;文件的创建者最终不一定是文件的拥有者，可以用``chown``修改拥有者。

### IV 目录文件的rwx权限含义

&emsp;&emsp;对目录文件来说，``rwx``权限是什么意思呢？

&emsp;&emsp;``r``权限：查看目录中内容的权限：

![](https://img-blog.csdnimg.cn/img_convert/641690f6515d1bf1b0e159199879ac6c.png)

&emsp;&emsp;同样的``root``不受权限约束。

&emsp;&emsp;``w``权限：考虑目录是一个多叉树结构，在目录中添加文件，作为父节点的目录，需要添加子节点的地址，这就是写权限。

![](https://img-blog.csdnimg.cn/img_convert/36468db49302cae9d23202020ca70e83.png)

&emsp;&emsp;进入目录需要什么权限呢？进入目录其实就相当于更改用户当前路径，是一种执行操作，需要的就是``x``权限，目录文件的可执行权限就是进入目录的权限。

![](https://img-blog.csdnimg.cn/img_convert/8afb4109d692e81fccd578d0f3e24186.png)

&emsp;&emsp;按常理来说，要查看一个目录中的文件，你要进入这个目录才能查看。

&emsp;&emsp;但是下面的实验表明：我们去掉目录的``x``权限仍然可以看目录中的文件名，只是不能看详细信息了.

&emsp;&emsp;这可能说明目录中的文件名是记录在目录中的（就像孩子指针）,但是详细信息还是记录在每个文件中的，如果你进入不了这个目录那肯定也看不到文件的详细信息。

![](https://img-blog.csdnimg.cn/img_convert/6399fc1515a36096a1bbf1b32ce015f0.png)

## 4 粘滞位

&emsp;&emsp;下面的测试会表明，在一个文件夹``test``中，我和其他用户一起使用这个文件夹，我能看到别人的文件存在，但是看不到别人的文件内容（被其他的``r``权限限制），但是我居然可以删掉别人的文件：

![](https://img-blog.csdnimg.cn/img_convert/481c3c6d2f63e7ddc3d8bfc55e6e0fdc.png)

&emsp;&emsp;本质原因是因为我们对``test``目录有写权限，改掉就不行了。

![](https://img-blog.csdnimg.cn/img_convert/47493dab4213cf6e542dee78a776d88a.png)

&emsp;&emsp;如果目录对other有w权限，则other可以删掉任何目录中的文件；如果目录对other没有w权限，那么other不可以删掉目录中任何文件,但是同时other没有了w权限，它也不能在目录中增加自己的文件了：

![](https://img-blog.csdnimg.cn/img_convert/b6c8cedbfc86d5d6224ec855c3c63559.png)

&emsp;&emsp;需求：other可以在特定的目录下创建文件并对文件(目录具有w)有写入权限(文件有w)，但是不想让other有权利删掉这个目录中的其他文件（这就要求other不能拥有目录的写权限）。

&emsp;&emsp;Linux下能否做到这种限定性的约束呢？这就引出了粘滞位。

&emsp;&emsp;语法``chmod o+t test``,这个加的t权限会加在w位置上。

![](https://img-blog.csdnimg.cn/img_convert/9f0a22ac5d1d8b73216f85094c064107.png)

&emsp;&emsp;粘滞位只能对目录设置，一般都是给``other``设置的。同组一般不用设置，要么你别给同组的人放开``w``权限，要么直接组内就那么几个人能查到，但是对于一个工作集群来说，``other``身份人很多，一般给``other``设置一个粘滞位。

&emsp;&emsp;对文件设置了粘滞位后，该目录下，只能文件的拥有者和``root``才能删除文件，其他人无法删除文件。

![](https://img-blog.csdnimg.cn/img_convert/cdaa53360e240faf0408495443cbb56f.png)

在操作系统临时数据中的应用：

&emsp;&emsp;有时候，存在多个用户，或者系统中存在临时数据，系统会将所有的临时文件放在``/tmp``目录下，这个目录的拥有者和所属组毫无以为是``root``,又因为所有用户的临时数据都要放在这里，所以要把这里``tmp``目录对``o``的读、写、执行权限放开，才能让别人把自己的临时文件写到``tmp``目录下，并且可以删除。

&emsp;&emsp;但是这样全放开每个用户却不仅可以删除自己的文件，还可以删除别人的文件。

&emsp;&emsp;我们想收回一部分权限，让每个用户在``tmp``目录下只能删除自己拥有的文件，不可以删除别人拥有的文件，这就需要给``tmp``目录权限设置粘滞位。

![](https://img-blog.csdnimg.cn/img_convert/cff8d3b5b6055bb51111a14590dfeb03.png)

&emsp;&emsp;当一个目录设置粘滞位时，只有以下三个用户可以删除里面的文件：

- 文件的所有者（只能删除所拥有的文件）
- 目录的所有者（能删除所有的文件）
- 超级用户``root``（能删除所有的文件）

&emsp;&emsp;一般我们把需要粘滞位的目录都是用``root``创建的，这样也符合``root``超级用户的权限性质。

## 5 默认权限与权限掩码

&emsp;&emsp;为什么系统中的文件，创建出来的默认权限是下面我们看到的样子呢？

![](https://img-blog.csdnimg.cn/img_convert/de437aea28d8f252dcecd27b8ee858bd.png)

&emsp;&emsp;起始权限：

- 普通文件：起始权限 666
- 目录文件：起始权限 777

&emsp;&emsp;那为什么我们看到的创建目录和文件的权限不是起始权限呢？因为系统中有``umask``，权限掩码。

![](https://img-blog.csdnimg.cn/img_convert/b9953cda4ba3dfbcacd29ccf9d402e1d.png)

&emsp;&emsp;后三位，凡是在掩码中出现过的权限，默认创建出来的文件都不准有这个权限。（这个最前面的0也是有用的，以后再议）

&emsp;&emsp;``002 = 000 000 010``

&emsp;&emsp;掩码的目的就是过滤掉掩码中的某些内容。

&emsp;&emsp;然后我们看普通文件的起始权限：``110 110 110``,干掉掩码中是1的位后，变为``110 110 100``，即``664``

&emsp;&emsp;这个干掉掩码中是1的位的操作是：``default = default & (~umask)``.

&emsp;&emsp;某一个比特位``&``上1，可以保持原来的模样，某一个比特位``&``上0，,这一位必然变成0。

&emsp;&emsp;``111 111 111 & ~(000 000 010) = 111 111 101 = 7 7 5``.

&emsp;&emsp;怎么修改``umask``呢？``umask 权限掩码``

![](https://img-blog.csdnimg.cn/img_convert/0cb45ccdbf44c2e113b65383f060b223.png)

&emsp;&emsp;权限掩码的修改只在这次会话有效，所以不用担心忘了改回去，如果要修改全局的默认权限掩码，可以去修改权限文件。







