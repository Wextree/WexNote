package normal;

import java.io.*;

/**
 * 这个类是利用输入输出流复制文件
 * 一遍读一遍写
 * 这个方法是对这个流一个一个字节的读，返回的结果就是这个字节的int表示方式；
 */
public class CopyFile {
    public static void main(String[] args) throws IOException {
        String inPath = "E:/softprograme/java/in.txt";
        String outPath = "E:/softprograme/java/out.txt";

        File file1 = new File(inPath);
        File file2 = new File(outPath);

        if (!file1.exists()){
            System.out.println("被复制的文件不存在！");
            System.exit(1);
        }

        InputStream in = new FileInputStream(file1);
        OutputStream out = new FileOutputStream(file2);

        if (in != null && out != null){
            int temp = 0;
            while ((temp = in.read()) != -1){
                out.write(temp);
            }
        }

        // 最后要记得关闭输入输出流
        in.close();
        out.close();
    }
}
