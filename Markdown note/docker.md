# Docker概述

Docker 项目的目标是实现轻量级的操作系统虚拟化解决方案。 Docker 的基础是 Linux 容器（LXC）等技术。

在 LXC 的基础上 Docker 进行了进一步的封装，让用户不需要去关心容器的管理，使得操作更为简便。用户操作 Docker 的容器就像操作一个快速轻量级的虚拟机一样简单。

下面的图片比较了 Docker 和传统虚拟化方式的不同之处，可见容器是在操作系统层面上实现虚拟化，直接复用本地主机的操作系统，而传统方式则是在硬件层面实现。

![](E:\git\WexNote\Markdown note\imgs\virtualization.png)

![](E:\git\WexNote\Markdown note\imgs\docker.png)

- **Docker守护进程**可以直接与**主操作系统**进行通信，为各个**Docker容器**分配资源；它还可以将容器与**主操作系统**隔离，并将各个容器互相隔离。
- **虚拟机**启动需要数分钟，而**Docker容器**可以在数毫秒内启动
- 由于没有臃肿的**从操作系统**，Docker可以节省大量的磁盘空间以及其他系统资源
- **虚拟机**更擅长于彻底隔离整个运行环境。例如，云服务提供商通常采用虚拟机技术隔离不同的用户。
- 而**Docker**通常用于隔离不同的应用，例如**前端**，**后端**以及**数据库**。



# Image镜像

> 在 Docker 的术语里，一个只读层被称为镜像，一个镜像是永久不会变的。
>
> 由于 Docker 使用一个统一文件系统，Docker 进程认为整个文件系统是以读写方式挂载的。 但是所有的变更都发生顶层的可写层，而下层的原始的只读镜像文件并未变化。由于镜像不 可写，所以镜像是无状态的。

![](E:\git\WexNote\Markdown note\imgs\docker-filesystems-multilayer.png)



## 相关概念

- **父镜像：**每一个镜像都可能依赖于由一个或多个下层的组成的另一个镜像。我们有时说，下层那个镜像是上层镜像的**父镜像**。
- **基础镜像：**一个**没有任何父镜像**的镜像，谓之基础镜像。
- **镜像ID：**所有镜像都是通过一个 **64 位**十六进制字符串 （内部是一个 256 bit 的值）来标识的。 为简化使用，前 12 个字符可以组成一个**短ID**，可以在命令行中使用。短ID还是有一定的 碰撞机率，所以服务器总是返回长ID。



## 相关操作

- 可以使用 `docker pull` 命令来从仓库获取所需要的镜像。

  ```shell
  # 版本省略默认为latest，仓库省略默认为官方仓库
  docker pull image-name:profile repository-address
  ```

- 使用 `docker images` 显示本地已有的镜像。

  ```shell
  # REPOSITORY：来自于哪个仓库
  # TAG：镜像的标记
  # IMAGE ID：它的 ID 号（唯一）
  # CREATED：创建时间
  # SIZE：镜像大小
  docker@default:~$ docker images
  REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
  hello-world         latest              bf756fb1ae65        4 months ago        13.3kB
  rabbitmq            3.7.7-management    2888deb59dfc        21 months ago       149MB
  ```

- 





# Docker容器

- 它会在所有的镜像层之上增加一个**可写层**。这个可写层有运行在CPU上的进程，而且有两个不同的状态：**运行态（Running）**和**退出态（Exited）**。

- 当我们使用docker run启动容器，Docker容器就进入运行态，当我们停止Docker容器时，它就进入退出态。

- 当我们有一个正在运行的Docker容器时，**从运行态到停止态**，我们对它所做的一切变更都会永久地写到**容器的文件系统**中。要切记，对容器的变更是写入到容器的文件系统的，而不是写入到Docker镜像中的。

- 我们可以用同一个镜像启动多个Docker容器，这些容器启动后都是活动的，彼此还是相互隔离的。我们对其中一个容器所做的变更只会**局限于那个容器本身**。

- 如果对容器的底层镜像进行修改，那么当前正在运行的容器是不受影响的，**不会发生自动更新现象。**



## 相关操作

- **启动一个容器**

```shell
# 启动一个容器
# ubuntu:15.10 指定要运行的镜像，如果不存在会下载
# /bin/bash "Hello world": 在启动的容器里执行的命令
# -t: 在新容器内指定一个伪终端或终端。 -i: 允许你对容器内的标准输入 (STDIN) 进行交互。
docker@default:~$ docker run -i -t ubuntu:15.10 /bin/bash
root@874f23f16592:/#

# 在容器中运行命令 cat /proc/version和ls分别查看当前系统的版本信息和当前目录下的文件列表
root@874f23f16592:/# cat /proc/version
Linux version 4.14.154-boot2docker (root@08b45408fb99) (gcc version 8.3.0 (Debian 8.3.0-6))
#1 SMP Thu Nov 14 19:19:08 UTC 2019
root@874f23f16592:/# ls
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var

# 我们可以通过运行 exit 命令或者使用 CTRL+D 来退出容器。
root@874f23f16592:/# exit
exit
docker@default:~$
```

- **启动容器（后台模式）**

```shell
# 并没有输出“hello world”，而是输出一串容器ID
docker@default:~$ docker run -d ubuntu:15.10 /bin/sh -c "while true; do echo hello world; sleep 1; done"
8caf6dc88994b6b2f14cb8b15be75916ded00100d520720ea01cb04adfaca1be

# 通过docker ps确定进程正在进行
# CONTAINER ID: 容器 ID。
# IMAGE: 使用的镜像。
# COMMAND: 启动容器时运行的命令。
# CREATED: 容器的创建时间。
# STATUS: 容器状态。created（已创建），restarting（重启中），running（运行中），removing（迁移中）
# 				  paused（暂停），exited（停止），dead（死亡）
# PORTS: 容器的端口信息和使用的连接类型（tcp\udp）。

NAMES: 自动分配的容器名称。
docker@default:~$ docker ps
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS   
8caf6dc88994        ubuntu:15.10        "/bin/sh -c 'while t…"   3 hours ago         Up 3 hours         
PORTS			   NAMES
				  sharp_mendel
				  
#　我们使用 docker stop 命令来停止容器
docker@default:~$ docker stop 8caf6dc88994b6b2f14cb8b15be
8caf6dc88994b6b2f14cb8b15be
docker@default:~$ docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
```

























































