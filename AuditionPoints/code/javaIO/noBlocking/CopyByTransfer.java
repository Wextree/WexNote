package noBlocking;

import java.io.IOException;
import java.nio.channels.FileChannel;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class CopyByTransfer {
    public static void main(String[] args) throws IOException {
        FileChannel inChannel = FileChannel.open(Paths.get("E:/softprograme/java/in.txt"), StandardOpenOption.READ);
        FileChannel outChannel = FileChannel.open(Paths.get("E:/softprograme/java/out.txt"), StandardOpenOption.WRITE,StandardOpenOption.CREATE, StandardOpenOption.READ);

        inChannel.transferTo(0, inChannel.size(), outChannel);
        inChannel.close();
        outChannel.close();
    }
}
