package normal;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class Pipe {
    public static void main(String[] args) {
        Send send = new Send();
        Receive receive = new Receive();

        // 管道连接
        try {
            send.getOut().connect(receive.getIn());
        } catch (IOException e) {
            e.printStackTrace();
        }

        new Thread(send).start();
        new Thread(receive).start();
    }
}

// 消息发送类
class Send implements Runnable{
    private PipedOutputStream out = null;

    public Send() {
        out = new PipedOutputStream();
    }

    public PipedOutputStream getOut() {
        return out;
    }

    @Override
    public void run() {
        String message = "send you a message";
        try {
            // 消息写入管道输出流
            out.write(message.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

// 消息接受类
class Receive implements Runnable{

    private PipedInputStream in = null;

    public Receive() {
        in = new PipedInputStream();
    }

    public PipedInputStream getIn() {
        return in;
    }

    @Override
    public void run() {
        byte[] b = new byte[2000];
        int len = 0;
        try {
            len = in.read(b);
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            in.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("接受的内容是： " + new String(b, 0, len));
    }
}