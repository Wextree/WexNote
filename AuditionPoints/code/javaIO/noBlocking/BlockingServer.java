package noBlocking;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class BlockingServer {
    public static void main(String[] args) throws IOException {
        ServerSocketChannel server = ServerSocketChannel.open();
        FileChannel fileChannel = FileChannel.open(Paths.get("E:/softprograme/java/out.txt"), StandardOpenOption.CREATE, StandardOpenOption.WRITE);

        // 绑定客户端
        server.bind(new InetSocketAddress(6666));

        // 获取客户端通道
        SocketChannel client = server.accept();

        ByteBuffer buf = ByteBuffer.allocate(1024);

        while (client.read(buf) != -1){
            buf.flip();
            fileChannel.write(buf);
            buf.clear();
        }

        // 向客户端发送确认请求
        buf.put("已经发送完毕！".getBytes());
        buf.flip();
        client.write(buf);
        buf.clear();

        client.close();
        fileChannel.close();
        server.close();
    }
}
