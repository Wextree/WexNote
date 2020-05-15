package normal;

import java.io.*;

public class ToLower {
    public static void main(String[] args) throws IOException {
        String str = "HELLO WORLD";
        InputStream in = new ByteArrayInputStream(str.getBytes());
        OutputStream out = new ByteArrayOutputStream();

        int temp = 0;
        while ((temp = in.read()) != -1){
            out.write(Character.toLowerCase((char)temp));
        }

        System.out.println(out.toString());
        in.close();
        out.close();
    }
}

