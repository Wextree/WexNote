# Linux

## 根目录

- **/bin**：
  bin是Binary的缩写, 这个目录存放着最经常使用的命令。

- **/boot：**
  这里存放的是启动Linux时使用的一些核心文件，包括一些连接文件以及镜像文件。

- **/dev ：**
  dev是Device(设备)的缩写, 该目录下存放的是Linux的外部设备，在Linux中访问设备的方式和访问文件的方式是相同的。

- **/etc：**
  这个目录用来存放所有的系统管理所需要的配置文件和子目录。

- **/home**：
  用户的主目录，在Linux中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的。

- **/lib**：
  这个目录里存放着系统最基本的动态连接共享库，其作用类似于Windows里的DLL文件。几乎所有的应用程序都需要用到这些共享库。

- **/lost+found**：
  这个目录一般情况下是空的，当系统非法关机后，这里就存放了一些文件。

- **/media**：
  linux系统会自动识别一些设备，例如U盘、光驱等等，当识别后，linux会把识别的设备挂载到这个目录下。

- **/mnt**：
  系统提供该目录是为了让用户临时挂载别的文件系统的，我们可以将光驱挂载在/mnt/上，然后进入该目录就可以查看光驱里的内容了。

- **/opt**：
   这是给主机额外安装软件所摆放的目录。比如你安装一个ORACLE数据库则就可以放到这个目录下。默认是空的。

- **/proc**：
  这个目录是一个虚拟的目录，它是系统内存的映射，我们可以通过直接访问这个目录来获取系统信息。
  这个目录的内容不在硬盘上而是在内存里，我们也可以直接修改里面的某些文件，比如可以通过下面的命令来屏蔽主机的ping命令，使别人无法ping你的机器：

  ```
  echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all
  ```

- **/root**：
  该目录为系统管理员，也称作超级权限者的用户主目录。

- **/sbin**：
  s就是Super User的意思，这里存放的是系统管理员使用的系统管理程序。

- **/selinux**：
   这个目录是Redhat/CentOS所特有的目录，Selinux是一个安全机制，类似于windows的防火墙，但是这套机制比较复杂，这个目录就是存放selinux相关的文件的。

- **/srv**：
   该目录存放一些服务启动之后需要提取的数据。

- **/sys**：

   这是linux2.6内核的一个很大的变化。该目录下安装了2.6内核中新出现的一个文件系统 sysfs 。

  sysfs文件系统集成了下面3种文件系统的信息：针对进程信息的proc文件系统、针对设备的devfs文件系统以及针对伪终端的devpts文件系统。

  

  该文件系统是内核设备树的一个直观反映。

  当一个内核对象被创建的时候，对应的文件和目录也在内核对象子系统中被创建。

- **/tmp**：
  这个目录是用来存放一些临时文件的。

- **/usr**：
   这是一个非常重要的目录，用户的很多应用程序和文件都放在这个目录下，类似于windows下的program files目录。

- **/usr/bin：**
  系统用户使用的应用程序。

- **/usr/sbin：**
  超级用户使用的比较高级的管理程序和系统守护程序。

- **/usr/src：**
  内核源代码默认的放置目录。

- **/var**：
  这个目录中存放着在不断扩充着的东西，我们习惯将那些经常被修改的目录放在这个目录下。包括各种日志文件。

- **/run**：
  是一个临时文件系统，存储系统启动以来的信息。当系统重启时，这个目录下的文件应该被删掉或清除。如果你的系统上有 /var/run 目录，应该让它指向 run。



##常用命令

###pwd (显示目前所在的目录)

> pwd 是 **Print Working Directory** 的缩写，也就是显示目前所在目录的命令。

- **pwd**    		显示目前所在	的目录
- **pwd -P**       显示出确实的路径，而非使用连结 (link) 路径



### cd（切换目录）

>命令格式：cd <路径>
>
>意义：cd是change directory 的缩写；cd命令后面跟一个路径，用于切换当前用户所在的路径，其中路径可以是绝对路径也可以是相对路径。

- **cd /system/bin** 	表示切换到/system/bin路径下。

- **cd logs**                    表示切换到logs路径下。

- **cd /**                          表示切换到根目录。

- **cd ../**                        表示切换到上一层路径。



### ls（列出信息）

>命令格式：ls <参数> <路径>
>
>意义：ls是list的缩写；ls命令后面可以跟一个路径或参数，也可以不跟，表示列出路径或当前目录下的所有文件信息。最常用的的参数是“-l"，也就是“ls -l”命令。
>
>- -a ：全部的文件，连同隐藏文件( 开头为 . 的文件) 一起列出来(常用)
>- -d ：仅列出目录本身，而不是列出目录内的文件数据(常用)
>- -l ：长数据串列出，包含文件的属性与权限等等数据；(常用)

- **ls /** 	 				显示根目录下的所有文件及文件夹。

- **ls -l /data**          显示/data路径下的所有文件及文件夹的详细信息。

- **ls -l**                     显示当前路径下的所有文件及文件夹的详细信息

- **ls * |wc **             显示当前目录下面的文件数量。



### rm（删除）

>命令格式： rm <文件> 或 rm -r <文件夹>
>
>意义：rm是remove 的缩写。用于删除文件或文件夹，常用参数-r -f，-r表示删除目录，也可以用于删除文件，-f表示强制删除，不需要确认。同样的，删除文件前需保证当前用户对当前路径有修改的权限。

- **rm -rf path** 			删除path。

- **rm test.txt**             删除test.txt。



###mkdir(创建文件夹)

>命令格式： mkdir 文件夹
>
>意义：mkdir是make directory 的缩写。用于创建文件夹。创建文件夹前需保证当前用户对当前路径有修改的权限。

- **mkdir /data/path**  			在/data路径下创建path文件夹。

- **mkdir -p a/b/c**                    参数 -p用于创建多级文件夹，这句命令表示在当前路径下创建文件夹a， 而a文件   夹包含子文件夹b，b文件夹下又包含子文件夹c。
- **mkdir -m [权限] dir**             可以设置目录的权限 
- **rmdir dir**                               删除目录
- **rmdir -p  path**                      联级删除           



### cp(复制)

>命令格式： cp <文件><目标文件>或者cp -r<文件夹><目标文件夹>
>
>意义：cp是copy 的缩写。用于复制文件或文件夹。
>
>- **-a：**相当於 -pdr 的意思，至於 pdr 请参考下列说明；(常用)
>- **-f：**为强制(force)的意思，若目标文件已经存在且无法开启，则移除后再尝试一次；
>- **-i：**若目标档(destination)已经存在时，在覆盖时会先询问动作的进行(常用)
>- **-p：**连同文件的属性一起复制过去，而非使用默认属性(备份常用)；
>- **-r：**递归持续复制，用於目录的复制行为；(常用)

- **cp /data/logs   /data/local/tmp/logs**      		复制/data路径下的logs到/data/local/tmp路径下。

- **cp 1.sh   /sdcard/**                                               复制当前路径下的1.sh到/sdcard下。



### rm(移除)

>- -f ：就是 force 的意思，忽略不存在的文件，不会出现警告信息；
>- -i ：互动模式，在删除前会询问使用者是否动作
>- -r ：递归删除啊！最常用在目录的删除了！这是非常危险的选项！！！

- **rm [-fir]**  					文件或目录



###mv(移动文件与目录，或修改名称)

> - -f ：force 强制的意思，如果目标文件已经存在，不会询问而直接覆盖；
> - -i ：若目标文件 (destination) 已经存在时，就会询问是否覆盖！
> - -u ：若目标文件已经存在，且 source 比较新，才会升级 (update)

- **mv mvtest mvtest2**         其实就是一个重命名的过程



### touch(创建文件)

- **touch [文件名]**



### 读取文件

> - cat  由第一行开始显示文件内容
> - tac  从最后一行开始显示，可以看出 tac 是 cat 的倒着写！
> - nl   显示的时候，顺道输出行号！
> - more 一页一页的显示文件内容
> - less 与 more 类似，但是比 more 更好的是，他可以往前翻页！
> - head 只看头几行
> - tail 只看尾巴几行

- **cat /sys/devices/system/cpu/online** 		读取 /sys/devices/system/cpu/路径下online文件内容。
- **cat test.txt**                                                        读取当前路径下test.txt文件内容。

- **tac /etc/issue**
- **nl [-bnw] 文件**
- **less /etc/man.config**
- **more /etc/man_db.config**
- **head [-n number] 文件**     取出文件前面几行，后面接数字，代表显示几行的意思
- **tail [-n number] 文件** 



## 链接

> Linux 链接分两种，一种被称为硬链接（Hard Link），另一种被称为符号链接（Symbolic Link）。默认情况下，**ln** 命令产生硬链接。
>
> -  1).删除符号连接f3,对f1,f2无影响；
> -  2).删除硬连接f2，对f1,f3也无影响；
> -  3).删除原文件f1，对硬连接f2没有影响，导致符号连接f3失效；
> -  4).同时删除原文件f1,硬连接f2，整个文件会真正的被删除。

```shell
[oracle@Linux]$ touch f1          #创建一个测试文件f1
[oracle@Linux]$ ln f1 f2          #创建f1的一个硬连接文件f2
[oracle@Linux]$ ln -s f1 f3       #创建f1的一个符号连接文件f3
[oracle@Linux]$ ls -li            # -i参数显示文件的inode节点信息
total 0
9797648 -rw-r--r--  2 oracle oinstall 0 Apr 21 08:11 f1
9797648 -rw-r--r--  2 oracle oinstall 0 Apr 21 08:11 f2
9797649 lrwxrwxrwx  1 oracle oinstall 2 Apr 21 08:11 f3 -> f1

[oracle@Linux]$ echo "I am f1 file" >>f1
[oracle@Linux]$ cat f1
I am f1 file
[oracle@Linux]$ cat f2
I am f1 file
[oracle@Linux]$ cat f3
I am f1 file
[oracle@Linux]$ rm -f f1
[oracle@Linux]$ cat f2
I am f1 file
[oracle@Linux]$ cat f3
cat: f3: No such file or directory
```



## 用户管理

### 添加用户

> useradd 选项 用户名
>
> - -c comment 指定一段注释性描述。
> - -d 目录 指定用户主目录，如果此目录不存在，则同时使用-m选项，可以创建主目录。
> - -g 用户组 指定用户所属的用户组。
> - -G 用户组，用户组 指定用户所属的附加组。
> - -s Shell文件 指定用户的登录Shell。
> - -u 用户号 指定用户的用户号，如果同时有-o选项，则可以重复使用其他用户的标识号。

```shell
# 此命令创建了一个用户sam，其中-d和-m选项用来为登录名sam产生一个主目录 /home/sam（/home为默认的用户主目录所在的父目录）
useradd –d  /home/sam -m sam

#此命令新建了一个用户gem，该用户的登录Shell是 /bin/sh，它属于group用户组，同时又属于adm和root用户组，其中group用户组是其主组。
#这里可能新建组： groupadd group及groupadd adm

useradd -s /bin/sh -g group –G adm,root gem

```

### 修改删除帐号

```shell
# 常用的选项是 -r，它的作用是把用户的主目录一起删除
userdel -r sam

# 常用的选项包括-c, -d, -m, -g, -G, -s, -u以及-o等，这些选项的意义与useradd命令中的选项一样，可以为用户指定新的资源值。另外，有些系统可以使用选项：-l 新用户名
usermod -s /bin/ksh -d /home/z –g developer sam
```



## 磁盘管理

> - df：列出文件系统的整体磁盘使用量
> - du：检查磁盘空间使用量
> - fdisk：用于磁盘分区

### df

df命令参数功能：检查文件系统的磁盘空间占用情况。可以利用该命令来获取硬盘被占用了多少空间，目前还剩下多少空间等信息。

语法：

```
df [-ahikHTm] [目录或文件名]
```

选项与参数：

- -a ：列出所有的文件系统，包括系统特有的 /proc 等文件系统；
- -k ：以 KBytes 的容量显示各文件系统；
- -m ：以 MBytes 的容量显示各文件系统；
- -h ：以人们较易阅读的 GBytes, MBytes, KBytes 等格式自行显示；
- -H ：以 M=1000K 取代 M=1024K 的进位方式；
- -T ：显示文件系统类型, 连同该 partition 的 filesystem 名称 (例如 ext3) 也列出；
- -i ：不用硬盘容量，而以 inode 的数量来显示

### du

Linux du命令也是查看使用空间的，但是与df命令不同的是Linux du命令是对文件和目录磁盘使用的空间的查看，还是和df命令有一些区别的，这里介绍Linux du命令。

语法：

```
du [-ahskm] 文件或目录名称
[-ahskm] 文件或目录名称
```

选项与参数：

- -a ：列出所有的文件与目录容量，因为默认仅统计目录底下的文件量而已。
- -h ：以人们较易读的容量格式 (G/M) 显示；
- -s ：列出总量而已，而不列出每个各别的目录占用容量；
- -S ：不包括子目录下的总计，与 -s 有点差别。
- -k ：以 KBytes 列出容量显示；
- -m ：以 MBytes 列出容量显示

### fdisk

fdisk 是 Linux 的磁盘分区表操作工具。

语法：

```
fdisk [-l] 装置名称
[-l] 装置名称
```

选项与参数：

- -l ：输出后面接的装置所有的分区内容。若仅有 fdisk -l 时， 则系统将会把整个系统内能够搜寻到的装置的分区均列出来。

## Vim & Shell

[vim](https://www.runoob.com/linux/linux-vim.html)

[shell](https://www.runoob.com/linux/linux-shell.html)

[linux手册](https://www.runoob.com/linux/linux-command-manual.html)













