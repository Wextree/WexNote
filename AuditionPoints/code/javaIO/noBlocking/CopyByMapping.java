package noBlocking;

import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class CopyByMapping {
    public static void main(String[] args) throws IOException {
        FileChannel inChannel = FileChannel.open(Paths.get("E:/softprograme/java/in.txt"), StandardOpenOption.READ);
        FileChannel outChannel = FileChannel.open(Paths.get("E:/softprograme/java/out.txt"), StandardOpenOption.WRITE,StandardOpenOption.CREATE, StandardOpenOption.READ);

        // 内存映射文件
        MappedByteBuffer in = inChannel.map(FileChannel.MapMode.READ_ONLY, 0, inChannel.size());
        MappedByteBuffer out = outChannel.map(FileChannel.MapMode.READ_WRITE, 0, inChannel.size());

        byte[] bytes = new byte[in.limit()];
        in.get(bytes);
        out.put(bytes);

        inChannel.close();
        outChannel.close();
    }
}
