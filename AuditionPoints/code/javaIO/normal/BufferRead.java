package normal;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class BufferRead {
    public static void main(String[] args) {
        // 装饰器
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("请输入打印的字符串：");

        String str = null;

        try {
            str = reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println(str);
    }
}
