package noBlocking;

import java.nio.ByteBuffer;

public class UseTraining {
    public static void main(String[] args) {
        // 创建一个缓冲区
        ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
        // 添加一些数据到缓冲区中
        String s = "Wextree";
        byteBuffer.put(s.getBytes());
        byteBuffer.flip();

        System.out.println("flip后-->limit--->"+byteBuffer.limit());
        System.out.println("flip后-->position--->"+byteBuffer.position());
        System.out.println("flip后-->capacity--->"+byteBuffer.capacity());
        System.out.println("flip后-->mark--->" + byteBuffer.mark());

        System.out.println("--------------------------------------");

        byte[] bytes = new byte[byteBuffer.limit()];
        byteBuffer.get(bytes);
        System.out.println(new String(bytes, 0, bytes.length));

        System.out.println("get完之后-->limit--->"+byteBuffer.limit());
        System.out.println("get完之后-->position--->"+byteBuffer.position());
        System.out.println("get完之后-->capacity--->"+byteBuffer.capacity());
        System.out.println("get完之后-->mark--->" + byteBuffer.mark());

        byteBuffer.clear();
    }
}
