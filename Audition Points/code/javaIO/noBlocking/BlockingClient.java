package noBlocking;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.SocketChannel;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

/**
 * 向服务端发送本地文件
 */
public class BlockingClient {
    public static void main(String[] args) throws IOException {
        // 1. 获取网络通道
        SocketChannel socketChannel = SocketChannel.open(new InetSocketAddress("127.0.0.1", 6666));

        // 2. 创建本地文件通道
        FileChannel fileChannel = FileChannel.open(Paths.get("E:/softprograme/java/in.txt"), StandardOpenOption.READ);

        // 3. 创建buffer
        ByteBuffer buf = ByteBuffer.allocate(1024);

        // 4. 写入网络通道
        while (fileChannel.read(buf) != -1){
            buf.flip();
            socketChannel.write(buf);
            buf.clear();
        }
        // 告诉服务器我已经发送完毕了，避免阻塞
        socketChannel.shutdownOutput();

        // 接受服务端发送的确认请求
        int len = 0;
        while ((len = socketChannel.read(buf)) != -1){
            buf.flip();
            System.out.println(new String(buf.array(), 0, len));
            buf.clear();
        }

        // 5. 关闭流
        socketChannel.close();
        fileChannel.close();
    }
}
