# Docker概述

Docker 项目的目标是实现轻量级的操作系统虚拟化解决方案。 Docker 的基础是 Linux 容器（LXC）等技术。

在 LXC 的基础上 Docker 进行了进一步的封装，让用户不需要去关心容器的管理，使得操作更为简便。用户操作 Docker 的容器就像操作一个快速轻量级的虚拟机一样简单。

下面的图片比较了 Docker 和传统虚拟化方式的不同之处，可见容器是在操作系统层面上实现虚拟化，直接复用本地主机的操作系统，而传统方式则是在硬件层面实现。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/virtualization.png)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/docker.png)

- **Docker守护进程**可以直接与**主操作系统**进行通信，为各个**Docker容器**分配资源；它还可以将容器与**主操作系统**隔离，并将各个容器互相隔离。
- **虚拟机**启动需要数分钟，而**Docker容器**可以在数毫秒内启动
- 由于没有臃肿的**从操作系统**，Docker可以节省大量的磁盘空间以及其他系统资源
- **虚拟机**更擅长于彻底隔离整个运行环境。例如，云服务提供商通常采用虚拟机技术隔离不同的用户。
- 而**Docker**通常用于隔离不同的应用，例如**前端**，**后端**以及**数据库**。



# Image镜像

> 在 Docker 的术语里，一个只读层被称为镜像，一个镜像是永久不会变的。
>
> 由于 Docker 使用一个统一文件系统，Docker 进程认为整个文件系统是以读写方式挂载的。 但是所有的变更都发生顶层的可写层，而下层的原始的只读镜像文件并未变化。由于镜像不可写，所以镜像是无状态的。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/docker-filesystems-multilayer.png)



## 相关概念

- **父镜像：**每一个镜像都可能依赖于由一个或多个下层的组成的另一个镜像。我们有时说，下层那个镜像是上层镜像的**父镜像**。
- **基础镜像：**一个**没有任何父镜像**的镜像，谓之基础镜像。
- **镜像ID：**所有镜像都是通过一个 **64 位**十六进制字符串 （内部是一个 256 bit 的值）来标识的。 为简化使用，前 12 个字符可以组成一个**短ID**，可以在命令行中使用。短ID还是有一定的 碰撞机率，所以服务器总是返回长ID。



## 相关操作

- 查找镜像：

  ```shell
  docker@default:~$ docker search httpd
  NAME                                    DESCRIPTION                                     STARS           
  httpd                                   The Apache HTTP Server Project                  3039             
      OFFICIAL            AUTOMATED
  	[OK]
  	
  # 镜像删除使用 docker rmi 命令
  docker@default:~$ docker rmi hello-world
  ```

  

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

  

- 其他命令：

  ```shell
  # 当我们从 docker 镜像仓库中下载的镜像不能满足我们的需求时，我们可以通过以下两种方式对镜像进行更改。
  # 1、从已经创建的容器中更新镜像，并且提交这个镜像
  # 2、使用 Dockerfile 指令来创建一个新的镜像
  # 在运行的容器内使用 apt-get update 命令进行更新。
  
  # 我们可以通过命令 docker commit 来提交容器副本
  # -m: 提交的描述信息
  # -a: 指定镜像作者
  # e218edb10161：容器 ID
  # runoob/ubuntu:v2: 指定要创建的目标镜像名
  runoob@runoob:~$ docker commit -m="has update" -a="runoob" e218edb10161 runoob/ubuntu:v2
  sha256:70bf1840fd7c0d2d8ef0a42a817eb29f854c1af8f7c59fc03ac7bdee9545aff84
  
  ```



- 构建镜像：

  ```shell
  # 创建一个 Dockerfile 文件，其中包含一组指令来告诉 Docker 如何构建我们的镜像。
  runoob@runoob:~$ cat Dockerfile 
  FROM    centos:6.7
  MAINTAINER      Fisher "fisher@sudops.com"
  
  RUN     /bin/echo 'root:123456' |chpasswd
  RUN     useradd runoob
  RUN     /bin/echo 'runoob:123456' |chpasswd
  RUN     /bin/echo -e "LANG=\"en_US.UTF-8\"" >/etc/default/local
  EXPOSE  22
  EXPOSE  80
  CMD     /usr/sbin/sshd -D
  
  ```

  



# Docker容器

##简介

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
#NAMES: 自动分配的容器名称。
docker@default:~$ docker ps
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS   
8caf6dc88994        ubuntu:15.10        "/bin/sh -c 'while t…"   3 hours ago         Up 3 hours         
PORTS			   NAMES
				  sharp_mendel
				  
# 在使用 -d 参数时，容器启动后会进入后台。此时想要进入容器，可以通过以下指令进入：
docker@default:~$ docker attach
docker@default:~$ docker exec # 推荐大家使用 docker exec 命令，因为此退出容器终端，不会导致容器的停止
				  
				  
#　我们使用 docker stop 命令来停止容器，使用docker start启动一个已有容器
docker@default:~$ docker stop 8caf6dc88994b6b2f14cb8b15be
8caf6dc88994b6b2f14cb8b15be
docker@default:~$ docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
```

- **运行一个web应用：**

```shell
# 在docker容器中运行一个 Python Flask 应用来运行一个web应用
docker@default:~$ docker pull training/webapp
docker@default:~$ docker run -d -P training/webapp python app.py

# 我可以使用 docker port ID 或 docker port NAME 来查看容器端口的映射情况
docker@default:~$ docker port gifted_hypatia
5000/tcp -> 0.0.0.0:32768

# docker logs [ID或者名字] 可以查看容器内部的标准输出
docker@default:~$ docker logs 0d8439d12c0c
 * Running on http://0.0.0.0:5000/ (Press CTRL+C to quit)
192.168.99.1 - - [05/Jun/2020 07:17:49] "GET / HTTP/1.1" 200 -
192.168.99.1 - - [05/Jun/2020 07:17:50] "GET /favicon.ico HTTP/1.1" 404 -

# 使用 docker top 来查看容器内部运行的进程
docker@default:~$ docker top gifted_hypatia
UID                 PID                 PPID                C                   STIME               TTY 
root                2542                2521                0                   07:14               ?   
TIME                CMD
00:00:00            python app.py

# 使用 docker inspect 来查看 Docker 的底层信息。它会返回一个 JSON 文件记录着 Docker 容器的配置和状态信息。
docker@default:~$ docker inspect gifted_hypatia
[
    {
        "Id": "0d8439d12c0cf2cb2e22bd5c4d9a54ee3bfb490904072d7f4694408e7ef2acef",
        "Created": "2020-06-05T07:14:55.982500525Z",
        "Path": "python",
        "Args": [
            "app.py"
        ],
        "State": {
            "Status": "running",
            "Running": true,
            "Paused": false,
            "Restarting": false,
            "OOMKilled": false,
            "Dead": false,
            "Pid": 2542,
            "ExitCode": 0,
            "Error": "",
            "StartedAt": "2020-06-05T07:14:58.726873875Z",
            "FinishedAt": "0001-01-01T00:00:00Z"
        },
 .........
 
 # 停止WEB应用容器
 docker@default:~$ docker stop gifted_hypatia
 
 # 重启WEB应用程序
 docker@default:~$ docker start gifted_hypatia
 docker@default:~$ docker restart gifted_hypatia
 
 # docker ps -l 查询最后一次创建的容器：
  docker@default:~$ docker ps -l
  CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS                 
0d8439d12c0c        training/webapp     "python app.py"     15 minutes ago      Exited (137) 2 minutes 
PORTS               NAMES
ago                 gifted_hypatia
```

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591341485(1).png)



# Docker 容器连接

```shell
# 容器中可以运行一些网络应用，要让外部也可以访问这些应用，可以通过 -P 或 -p 参数来指定端口映射。
# -P :是容器内部端口随机映射到主机的高端口。
# -p : 是容器内部端口绑定到指定的主机端口。
runoob@runoob:~$ docker run -d -P training/webapp python app.py
runoob@runoob:~$ docker run -d -p 5000:5000 training/webapp python app.py

# 可以指定容器绑定的网络地址，比如绑定 127.0.0.1
runoob@runoob:~$ docker run -d -p 127.0.0.1:5001:5000 training/webapp python app.py

# 默认都是绑定 tcp 端口，如果要绑定 UDP 端口，可以在端口后面加上 /udp。
runoob@runoob:~$ docker run -d -p 127.0.0.1:5000:5000/udp training/webapp python app.py

# docker port 命令可以让我们快捷地查看端口的绑定情况
runoob@runoob:~$ docker port adoring_stonebraker 5000
127.0.0.1:5001
```



## 容器互联

端口映射并**不是唯一**把 docker 连接到另一个容器的方法。

docker 有一个连接系统允许将**多个容器连接**在一起，**共享**连接信息。

docker 连接会创建一个**父子关系**，其中父容器可以看到子容器的信息。

```shell
# 创建一个容器的时候，docker 会自动对它进行命名。另外，我们也可以使用 --name 标识来命名容器
runoob@runoob:~$  docker run -d -P --name runoob training/webapp python app.py
```



- **网络：**

  ```shell
  # 列出当前的网络
  docker@default:~$ docker network ls
  NETWORK ID          NAME                DRIVER              SCOPE
  89d895ee77d4        bridge              bridge              local
  dbe2e0c72bd5        host                host                local
  824d14abad6c        none                null                local
  
  # 创建一个新的 Docker 网络
  # -d：参数指定 Docker 网络类型，有 bridge、overlay。
  docker@default:~$ docker network create -d bridge test-net
  16da5425ac40d32c2dc8f4fd03b7731557b49e15923ad4766e02cb4fe7cb496e
  
  # 运行两个容器并连接到新建的 test-net 网络
  docker@default:~$ docker run -itd --name test1 --network test-net ubuntu /bin/bash
  docker@default:~$ docker run -itd --name test2 --network test-net ubuntu /bin/bash
  
  # 进入容器test1然后pingtest2
  # 如果该容器没有ping命令，先执行以下两个命令
  root@c825bcb75788:/# apt-get update
  root@c825bcb75788:/# apt install iputils-ping
  root@c825bcb75788:/# ping test2
  PING test2 (172.18.0.3) 56(84) bytes of data.
  64 bytes from test2.test-net (172.18.0.3): icmp_seq=1 ttl=64 time=0.049 ms
  64 bytes from test2.test-net (172.18.0.3): icmp_seq=2 ttl=64 time=0.041 ms
  64 bytes from test2.test-net (172.18.0.3): icmp_seq=3 ttl=64 time=0.208 ms
  64 bytes from test2.test-net (172.18.0.3): icmp_seq=4 ttl=64 time=0.062 ms
  # 这样，test1 容器和 test2 容器建立了互联关系。
  ```



# Docker仓库管理

```shell
# 登录需要输入用户名和密码，登录成功后，我们就可以从 docker hub 上拉取自己账号下的全部镜像
docker@default:~$ docker login

# 退出 docker hub 可以使用以下命令：
docker@default:~$ docker logout

# 你可以通过 docker search 命令来查找官方仓库中的镜像，并利用 docker pull 命令来将它下载到本地。
docker@default:~$ docker search ubuntu
docker@default:~$ docker pull ubuntu 

# 用户登录后，可以通过 docker push 命令将自己的镜像推送到 Docker Hub。
docker@default:~$ docker push username/ubuntu:18.04
```



# Docker Dockerfile











































