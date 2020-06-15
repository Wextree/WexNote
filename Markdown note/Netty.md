# Netty

##概述

官方定义为：”Netty 是一款**异步**的**事件驱动**的网络应用程序框架，支持快速地开发可维护的高性能的面向协议的服务器
和客户端”,按照惯例贴上一张High Level的架构图：

![](E:\git\WexNote\Markdown note\imgs\components.png)

Netty有很多重要的特性，主要**特性**如下：
- 优雅的设计
- 统一的API接口，支持多种传输类型，例如OIO,NIO
- 简单而强大的线程模型
- 丰富的文档
- 卓越的性能
- 拥有比原生Java API 更高的性能与更低的延迟
- 基于池化和复用技术，使资源消耗更低
- 安全性
- 完整的SSL/TLS以及StartTLS支持
- 可用于受限环境，如Applet以及OSGI



**原生NIO存在的问题：**
1、NIO的类库和API繁杂，使用麻烦：需要熟练掌握Selector 、ServerSocketChannel 、ByteBuffer等
2、需要具备其他的额外技能：要熟悉Java多线程编程，因为NIO涉及到Reactor模式，你必须对多线程和网络编程非常熟悉，才能编写出高质量的NIO程序
3、开发工作量和难度都非常大：例如客户端面临断连重连、网络闪断、半包读写、失败缓冲、网络拥塞和异常流的处理等等。
4、JDK NIO的bug：例如臭名昭著的Epoll Bug ,它会导致Selector空轮询，最终导致CPU100%.直到JDK1.7版本该问题仍旧存在，没有被根本解决。

## 架构设计

### 传统IO阻塞模型

![](E:\git\WexNote\Markdown note\imgs\1591968096.jpg)



**特点：**

- 采用阻塞IO获取输入的数据。
- 每个连接都需要独立的线程来获取数据的输入，业务处理，数据返回。



**问题：**

- 当并发数很大的时候，会占用大量的线程和大量的系统资源你。
- 当线程没有数据可读的时候，会阻塞在read操作中，造成线程资源的浪费。



### Reactor模式（反应器、分发者、通知者）

I/O复用结合线程池的基本设计思想。

1. 通过一个或多个输入同时传递给服务处理器的模式（基于事件驱动）。
2. 服务器端程序处理传入的多个请求，并将它们同步分派到相应的处理线程。
3. Reactor使用了IO复用监听事件。收到事件后进行分发给某个线程。（是网络处理高并发的关键）



- **单Reactor单线程**

![](E:\git\WexNote\Markdown note\imgs\timg (1).jpg)

服务器端用一个现车鞥通过多路复用搞定所有IO，包括所有的连接，读写等。

优点是编码简单，清晰明了，没有多线程，没有线程安全的问题。

但如果客户端较多，就无法支撑，目前的NIO就是属于这种类型。而且无法发挥多核CPU的优势。



- **单Reactor多线程**

![](E:\git\WexNote\Markdown note\imgs\timg.jpg)

1. Reactor对象通过selector监控客户端请求事件，收到事件后，通过dispatch进行分发。
2. 如果是建立连接请求，由Acceptor通过accept处理连接请求，然后创建一个handler处理完成连接后的各种事件。
3. 如果不是连接请求，则由Reactor分发调用连接对应的handler来处理。
4. handler只负责响应事件，不去做具体的业务处理。通过read方法读取数据后，会分发到后面的worker线程池的某个线程进行处理。
5. worker线程池会分配独立的线程来完成真正的业务，然后将结果返回给handler。



**优点：**

1. 可以充分利用多核CPU的处理能力

**缺点：**

1. 多线程数据共享和访问比较复杂，reactor处理所有的事件的监听和响应，在单线程运行，在高并发场景容易遇到性能瓶颈。



- **主从Reactor多线程**

![](E:\git\WexNote\Markdown note\imgs\u=2080208592,410273574&fm=11&gp=0.jpg)

1. Reactor主线程MainReactor对象通过select监听连接事件，收到事件后，通过Accept处理连接事件。
2. 当Acceptor处理连接事件后，MainReactor将连接分配给SubReactor。
3. SubReactor将连接加入到连接队列监听，并创建handler进行各种事件处理。
4. 当我新事件发生时，SubReactor就会调用对应的handler处理。
5. handler通过read读取数据，分发给后面的worker线程处理。
6. worker线程池会分配独立的线程来完成真正的业务，然后将结果返回给handler。
7. handler收到响应的结果后，再通过send将结果返回到对应的client客户端。
8. Reactor主线程可以对应多个子线程。



**优点：**

1. 父线程与子线程的数据交互简单，职责明确，父线程只需要接收新连接，子线程完成后续的业务处理。
2. 主线程只需要把新连接传给子线程，子线程无需返回数据。

**缺点：** 编程复杂度较高。



## Netty模型

### 工作原理

**通俗版：**

![](E:\git\WexNote\Markdown note\imgs\1592225829(1).jpg)



1. BossGroup线程维护Selector，只关注Accept
2. 当接受到Accept事件，获取对应的SocketChannel，封装成NIOSocketChannel并注册到Worker线程（事件循环），并进行维护
3. 当Worker线程监听到selector中通道发生自己感兴趣的事件后，就进行处理（就由handler完成），注意handler已经加入到通道了。



**详细版：**

![](E:\git\WexNote\Markdown note\imgs\V3Wt_mwBJleJMoPMMFZ1.jpg)

1. Netty抽象除两组线程池BossGroup专门负责接收客户端的连接，WorkerGroup专门负责网络的读写。
2. BossGroup和WorkerGroup类型都是NioEventLoopGroup。
3. NioEventLoopGroup相当于一个事件循环组，这个组中含有多个事件循环，每一个事件是NioEventLoop。
4. NioEventLoop表示一个不断循环的执行处理任务的线程，每个NioEventLoop都有一个selector，用于监听绑定在其上的socket网络通讯。
5. NioEventLoopGroup可以有多个线程，可以含有多个NioEventLoop。
6. 每个Boss NioEventLoop执行的步骤有三步：
   1. 轮询accept事件
   2. 处理accept事件，与client建立连接，生成NioSocketChannel，并将其注册到某个worker NioEventLoop上的selector。
   3. 处理任务队列的任务，即runAllTasks。
7. 每个worker NioEventLoop 循环执行的步骤。
   1. 轮询read，write事件
   2. 处理IO事件，即read、write事件，在对应的NioSocketChannel处理。
   3. 处理任务队列的任务，即runAllTasks。
8. 每个worker NioEventLoop在处理业务时，会使用pipeline（管道），pipeline中包含了channel，即通过pipeline可以获取对应通道，管道中维护了很多的处理器（用于数据的处理）。



## 快速入门



