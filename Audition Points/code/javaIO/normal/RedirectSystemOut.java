package normal;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class RedirectSystemOut {
    public static void main(String[] args) throws FileNotFoundException {
        File file = new File("E:/softprograme/java/out.txt");

        System.out.println("这句话直接打印在控制台上！");

        System.setOut(new PrintStream(new FileOutputStream(file)));

        System.out.println("这句话打印在对应路径的文件中。");
    }
}
