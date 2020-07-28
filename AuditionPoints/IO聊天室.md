# IO聊天室

## Socket和ServerSocket实战

首先创建一个最普通的Java项目。然后创建两个类，**Server和Client**。其代码和注释如下,仔细看下注释和代码，还是比较简单的

服务器**只能为一个客户端服**务，一旦监听到客户端的请求，就会一直被这个**客户端占用。**

```java
package io;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * 服务器只能为一个客户端服务，一旦监听到客户端的请求，就会一直被这个客户端占用。
 */
public class Server {
    public static void main(String[] args) {
        final int DEFAULT_PORT = 8088;
        try {
            ServerSocket serverSocket = new ServerSocket(DEFAULT_PORT);
            System.out.println("ServerSocket Start,The Port is:" + DEFAULT_PORT);
            // 不停监听窗口
            while (true){
                Socket socket = serverSocket.accept();
                System.out.println("Client[" + socket.getPort() + "]Online");
                // 分别接收消息和发送消息
                BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                // 与客户端进行交互
                String msg = null;
                while ((msg = reader.readLine()) != null){
                    System.out.println("Client[" + socket.getPort() + "]:" + msg);
                    writer.write("Server:" + msg + "\n");
                    writer.flush();

                    //如果客户端的消息是quit代表他退出了，并跳出循环，不用再接收他的消息了。如果客户端再次连接就会重新上线
                    if ("quit".equals(msg)){
                        System.out.println("Client[" + socket.getPort() + "]:Offline");
                        break;
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

```java
package io;

import java.io.*;
import java.net.Socket;

public class Client {
    public static void main(String[] args) {
        final String DEFAULT_HOST = "127.0.0.1";
        final int DEFAULT_PORT = 8088;
        try {
            Socket socket = new Socket(DEFAULT_HOST, DEFAULT_PORT);
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            BufferedReader userReader = new BufferedReader(new InputStreamReader(System.in));

            String msg = null;
            while (true){
                String input = userReader.readLine();
                writer.write(input + "\n");
                writer.flush();
                msg = reader.readLine();
                System.out.println(msg);

                if(input.equals("quit")){
                    break;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}

```

**使用：**

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591879985(1).jpg)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591879964(1).jpg)





## BIO编程模型实现群聊

所谓BIO，就是Block IO，**阻塞式的IO**。这个阻塞主要发生在：ServerSocket接收请求时**（accept()方法）**、InputStream、OutputStream（输入输出流的读和写）都是阻塞的。这个可以在下面代码的调试中发现，比如在客户端接收服务器消息的输入流处打上断点，除非服务器发来消息，不然断点是一直停在这个地方的。也就是说这个线程在这时间是被阻塞的。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1383122-20200411190359629-713089288.png)

**服务端：**

ChatServer:这个类的作用就像图中的Acceptor。它有两个比较关键的**全局变量**，一个就是**存储在线用户信息的Map**，一个就是**线程池**。这个类会监听端口，接收客户端的请求，然后为客户端分配工作线程。还会提供一些常用的工具方法给每个工作线程调用，比如：发送消息、添加在线用户等。



**客户端：**

相较于服务器，客户端的改动较小，主要是把等待用户输入信息这个功能分到其他线程做，不然这个功能会一直阻塞主线程，导致无法接收其他客户端的消息。



```java
package bio;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ChatServer {
    final int DEFAULT_PORT = 8088;
    /**
     * 创建一个Map存储在线用户的信息。这个map可以统计在线用户、针对这些用户可以转发其他用户发送的消息
     * 因为会有多个线程操作这个map，所以为了安全起见用ConcurrentHashMap
     * 在这里key就是客户端的端口号，但在实际中肯定不会用端口号区分用户，如果是web的话一般用session。
     * value是IO的Writer，用以存储客户端发送的消息
     */
    private Map<Integer, Writer> map = new ConcurrentHashMap<>();

    /**
     * 创建线程池，线程上限为10个，如果第11个客户端请求进来，服务器会接收但是不会去分配线程处理它。
     * 前10个客户端的聊天记录，它看不见。当有一个客户端下线时，这第11个客户端就会被分配线程，服务器显示在线
     * 大家可以把10再设置小一点，测试看看
     * */
    private ExecutorService executorService = Executors.newFixedThreadPool(10);

    public static void main(String[] args) {
        ChatServer server = new ChatServer();
        server.start();
    }

    private void start() {
        try {
            ServerSocket server = new ServerSocket(DEFAULT_PORT);
            System.out.println("Server Start,The Port is:"+DEFAULT_PORT);
            while (true){
                Socket socket = server.accept();
                executorService.execute(new ChatHandler(this, socket));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void addClient(Socket socket){
        if (socket != null){
            try {
                BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                map.put(socket.getPort(), writer);
                System.out.println("Client["+socket.getPort()+"]:Online");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void sendMessage(Socket socket, String msg) {
        for (Integer port:map.keySet()) {
            if (port != socket.getPort()){
                Writer writer = map.get(port);
                try {
                    writer.write(msg);
                    writer.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void removeClient(Socket socket) throws IOException {
        if (socket != null){
            if (map.containsKey(socket.getPort())){
                map.get(socket.getPort()).close();
                map.remove(socket.getPort());
            }
            System.out.println("Client[" + socket.getPort() + "]Offline");
        }
    }
}

```

```java
package bio;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class ChatHandler implements Runnable{
    private ChatServer server;
    private Socket socket;

    public ChatHandler(ChatServer server, Socket socket) {
        this.server = server;
        this.socket = socket;
    }

    @Override
    public void run() {
        // 往map添加这个客户端
        server.addClient(socket);

        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String msg = null;
            while ((msg = reader.readLine()) != null) {
                //这样拼接是为了让其他客户端也能看清是谁发送的消息
                String sendmsg = "Client[" + socket.getPort() + "]:" + msg;
                //服务器打印这个消息
                System.out.println(sendmsg);
                //将收到的消息转发给其他在线客户端
                server.sendMessage(socket, sendmsg + "\n");
                if (msg.equals("quit")) {
                    break;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            //如果用户退出或者发生异常，就在map中移除该客户端
            try {
                server.removeClient(socket);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}

```

```java
package bio;

import java.io.*;
import java.net.Socket;

public class ChatClient {
    private BufferedReader reader;
    private BufferedWriter writer;
    private Socket socket;

    public static void main(String[] args) {
        new ChatClient().start();
    }

    private void start() {
        try {
            socket = new Socket("127.0.0.1", 8088);
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new BufferedWriter((new OutputStreamWriter(socket.getOutputStream())));

            // 新建一个线程去监听用户的输入
            new Thread(new UserInputHandler(this));
            /**
             * 不停的读取服务器转发的其他客户端的信息
             * 记录一下之前踩过的小坑：
             * 这里一定要创建一个msg接收信息，如果直接用receive()方法判断和输出receive()的话会造成有的消息不会显示
             * 因为receive()获取时，在返回之前是阻塞的，一旦接收到消息才会返回，也就是while这里是阻塞的，一旦有消息就会进入到while里面
             * 这时候如果输出的是receive(),那么上次获取的信息就会丢失，然后阻塞在System.out.println
             * */
            String msg = null;
            // \n非常重要，不然在server端的readline就无法继续
            while ((msg = recieve()) != null){
                System.out.println(msg+"\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String recieve() throws IOException {
        String msg = null;
        if (!socket.isInputShutdown()){
            // 没有关闭的话就可以通过reader读取服务器发送来的消息。
            // 注意：如果没有读取到消息线程会阻塞在这里
            msg = reader.readLine();
        }
        return msg;
    }

    public void sendToServer(String input) throws IOException {
        if (!socket.isOutputShutdown()){
            writer.write(input);
            writer.flush();
        }
    }
}

```

```java
package bio;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class UserInputHandler implements Runnable{
    private ChatClient chatClient;

    public UserInputHandler(ChatClient chatClient) {
        this.chatClient = chatClient;
    }

    @Override
    public void run() {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        while (true){
            try {
                String input = reader.readLine();
                chatClient.sendToServer(input);
                if ("quit".equals(input)){
                    break;
                }
            } catch (IOException e) {
                e.printStackTrace();
            }

        }
    }
}

```

为了设置多个客户端，可以这样做：

复制ChatClient文件，然后改名，直接这样会报错，因为我们使用了一个线程，它需要的参数类型是ChatClient，所以我们要让我们这个复制的文件里的类**继承ChatClient**即可：

```java
public class ChatClientCopy extends ChatClient{
........
```

**效果：**

ChatClient:

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591926450(1).jpg)

ChatClientCopy:

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591926483(1).jpg)

ChatServer:

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591926511(1).jpg)



## NIO编程模型，升级改造聊天室

用**BIO编程模型**，简单的实现了一个聊天室。但是其最大的问题在解释BIO时就已经说了：ServerSocket接收请求时（accept()方法）、InputStream、OutputStream（输入输出流的读和写）都是**阻塞**的。还有一个问题就是**线程池**，线程多了，服务器性能耗不起。线程少了，在聊天室这种场景下，让用户等待连接肯定不可取。今天要说到的NIO编程模型就很好的解决了这几个问题。有两个主要的替换地方：

1. 用**Channel**代替Stream。

2. 使用**Selector**监控多条Channel，起到类似线程池的作用，但是它**只需一条线程**。

既然要用NIO编程模型，那就要说说它的三个主要核心：Selector、Channel、Buffer。它们的关系是：一个Selector管理多个Channel，一个Channel可以往Buffer中写入和读取数据。Buffer名叫缓冲区，底层其实是一个数组，会提供一些方法往数组写入读取数据。

```java
package nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Set;

public class NioServer {
    // 申请两个缓冲区待会做读写用
    private final int BUFFER_SIZE = 1024;
    //编码方式设置为utf-8，下面字符和字符串互转时用得到
    private Charset charset = Charset.forName("UTF-8");
    private ByteBuffer read_buffer = ByteBuffer.allocate(BUFFER_SIZE);
    private ByteBuffer write_buffer = ByteBuffer.allocate(BUFFER_SIZE);
    // 不把端口写死
    private int port;

    public NioServer(int port) {
        this.port = port;
    }

    public static void main(String[] args) {
        new NioServer(8088).start();
    }

    private void start() {
        try {
            // 打开一个服务器通道，然后设置为非阻塞，绑定端口
            ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.configureBlocking(false);
            serverSocketChannel.socket().bind(new InetSocketAddress(port));
            // 新建一个选择器，然后把通道注册上去，监听ACCEPT事件
            Selector selector = Selector.open();
            serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
            System.out.println("启动服务器，监听端口:" + port);

            // 循环监听
            while (true){
                // 进行监听，会阻塞
                selector.select();
                // 监听到事件就获取SelectKey，它包含了事件的所有信息
                Set<SelectionKey> selectionKeySet = selector.selectedKeys();
                // 循环处理这些事件
                for (SelectionKey selectionKey:selectionKeySet) {
                    if (selectionKey.isAcceptable()){
                        // 只有服务端才有这种接收，所以我们先获取服务器通道，然后获取客户端通道，注册进服务器的选择器中
                        ServerSocketChannel server = (ServerSocketChannel) selectionKey.channel();
                        SocketChannel client = server.accept();
                        client.configureBlocking(false);
                        client.register(selector, SelectionKey.OP_READ);
                        System.out.println("客户端[" + client.socket().getPort() + "]上线啦！");
                    }

                    // 若是可读事件
                    if (selectionKey.isReadable()){
                        // 获取客户端通道
                        SocketChannel client = (SocketChannel)selectionKey.channel();
                        // 清楚写缓存的残留
                        read_buffer.clear();
                        while (client.read(read_buffer) > 0);
                        // 设置为可读模式
                        read_buffer.flip();
                        String msg = String.valueOf(charset.decode(read_buffer));
                        System.out.println("客户端[" + client.socket().getPort() + "]:" + msg);
                        //把消息转发给其他客户端
                        sendMessage(client, msg, selector);
                        // 判断用户是否退出
                        if ("quit".equals(msg)){
                            //解除该事件的监听
                            selectionKey.cancel();
                            //更新Selector
                            selector.wakeup();
                            System.out.println("客户端[" + client.socket().getPort() + "]下线了！");
                        }
                    }
                }

                // 处理完事件要清空
                selectionKeySet.clear();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private void sendMessage(SocketChannel client, String msg, Selector selector) throws IOException {
        msg = "客户端[" + client.socket().getPort() + "]:" + msg;
        //获取所有客户端,keys()与前面的selectedKeys不同，这个是获取所有已经注册的信息，而selectedKeys获取的是触发了的事件的信息
        for (SelectionKey key : selector.keys()) {
            //排除服务器和本客户端并且保证key是有效的，isValid()会判断Selector监听是否正常、对应的通道是保持连接的状态等
            if (!(key.channel() instanceof ServerSocketChannel) && !client.equals(key.channel()) && key.isValid()) {
                SocketChannel otherClient = (SocketChannel) key.channel();
                write_buffer.clear();
                write_buffer.put(charset.encode(msg));
                write_buffer.flip();
                //把消息写入到缓冲区后，再把缓冲区的内容写到客户端对应的通道中
                while (write_buffer.hasRemaining()) {
                    otherClient.write(write_buffer);
                }
            }
        }
    }
}

```

```java
package nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Set;

public class NioClient {
    private final int BUFFER_SIZE = 1024;
    private ByteBuffer read_buffer = ByteBuffer.allocate(BUFFER_SIZE);
    private ByteBuffer write_buffer = ByteBuffer.allocate(BUFFER_SIZE);
    private SocketChannel client;
    private Selector selector;
    private Charset charset = Charset.forName("UTF-8");

    public static void main(String[] args) {
        new NioClient().start();
    }

    private void start() {
        try {
            client = SocketChannel.open();
            selector = Selector.open();
            client.configureBlocking(false);
            client.connect(new InetSocketAddress("127.0.0.1", 8088));
            client.register(selector, SelectionKey.OP_CONNECT);

            while (true){
                selector.select();
                if (!selector.isOpen()) break;
                Set<SelectionKey> selectionKeys = selector.selectedKeys();
                for (SelectionKey selectionKey:selectionKeys) {
                    if (selectionKey.isConnectable()){
                        SocketChannel channel = (SocketChannel)selectionKey.channel();
                        //finishConnect()返回true，说明和服务器已经建立连接。如果是false，说明还在连接中，还没完全连接完成
                        if (channel.finishConnect()){
                            //新建一个新线程去等待用户输入
                            new Thread(new UserInputHandler(this)).start();
                        }
                        //连接建立完成后，注册read事件，开始监听服务器转发的消息
                        client.register(selector,SelectionKey.OP_READ);
                    }

                    if (selectionKey.isReadable()){
                        SocketChannel channel = (SocketChannel)selectionKey.channel();
                        read_buffer.clear();
                        while (channel.read(read_buffer) > 0);
                        read_buffer.flip();
                        String msg = String.valueOf(charset.decode(read_buffer));
                        System.out.println(msg);
                        if ("quit".equals(msg)){
                            selectionKey.cancel();
                            selector.wakeup();
                        }
                    }
                }
                selectionKeys.clear();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void sendToServer(String msg) throws IOException {
        if(!msg.isEmpty()){
            write_buffer.clear();
            write_buffer.put(charset.encode(msg));
            write_buffer.flip();
            while (write_buffer.hasRemaining()){
                client.write(write_buffer);
            }
        if(msg.equals("quit")){
            selector.close();
        }
        }
    }
}

```

```java
package nio;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class UserInputHandler implements Runnable{
    private NioClient chatClient;

    public UserInputHandler(NioClient chatClient) {
        this.chatClient = chatClient;
    }

    @Override
    public void run() {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        while (true){
            try {
                String input = reader.readLine();
                chatClient.sendToServer(input);
                if ("quit".equals(input)){
                    break;
                }
            } catch (IOException e) {
                e.printStackTrace();
            }

        }
    }
}

```

**效果：**

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591934584(1).jpg)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591934634(1).jpg)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1591934662(1).jpg)



## AIO模型改造，最后升级

**阻塞和非阻塞，描述的是结果的请求**。**阻塞**：在得到结果之前就一直呆在那，啥也不干，此时线程挂起，就如其名，线程被阻塞了。**非阻塞**：如果没得到结果就返回，等一会再去请求，直到得到结果为止。**异步和同步，描述的是结果的发出**，当调用方的请求进来。**同步**：在没获取到结果前就不返回给调用方，如果调用方是阻塞的，那么调用方就会一直等着。如果调用方是非阻塞的，调用方就会先回去，等一会再来问问得到结果没。**异步**：调用方一来，如果是非阻塞的叫它先回去，待会有结果了再告诉你。如果是阻塞的，虽然异步会通知他，但他还是要等着，实属铁憨憨。

### AIO中的异步操作

**CompletionHandler**

在AIO编程模型中，常用的API，如connect、accept、read、write都是支持异步操作的。当调用这些方法时，可以携带一个**CompletionHandler**参数，它会提供一些回调函数。这些回调函数包括：1.当这些操作成功时你需要怎么做；2.如果这些操作失败了你要这么做。关于这个CompletionHandler参数，你只需要写一个类实现CompletionHandler口，并实现里面两个方法就行了。

先说说Socket和ServerSocket，在NIO中，它们变成了通道，配合缓冲区，从而实现了非阻塞。而在AIO中它们变成了异步通道。也就是AsynchronousServerSocketChannel和AsynchronousSocketChannel,下面例子中对象名分别是serverSocket和socket.

- **accept：**serverSocket.accept(attachment,handler)。handler就是实现了CompletionHandler接口并实现两个回调函数的类，它具体怎么写可以看下面的实战代码。attachment为handler里面可能需要用到的辅助数据，如果没有就填null。
- **read：**socket.read(buffer,attachment,handler)。buffer是缓冲区，用以存放读取到的信息。后面两个参数和accept一样。
- **write：**socket.write(buffer,attachment,handler)。和read参数一样。
- **connect：**socket.connect(address,attachment,handler)。address为服务器的IP和端口，后面两个参数与前几个一样。

