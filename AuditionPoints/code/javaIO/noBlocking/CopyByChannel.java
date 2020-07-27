package noBlocking;


import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

// 使用FileChannel配合缓冲区实现文件复制的功能：
public class CopyByChannel {
    public static void main(String[] args) throws IOException {

        FileInputStream in = null;
        FileOutputStream out = null;

        FileChannel fin = null;
        FileChannel fout = null;

        try {
            in = new FileInputStream(new File("E:/softprograme/java/in.txt"));
            out = new FileOutputStream(new File("E:/softprograme/java/out.txt"));

            fin = in.getChannel();
            fout = out.getChannel();

            ByteBuffer buf = ByteBuffer.allocate(1024);

            while (fin.read(buf) != -1){
                buf.flip();
                fout.write(buf);
                buf.clear();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            fin.close();
            fout.close();
            in.close();
            out.close();
        }

    }
}
