package digit;

public class StringTest {
    public static void main(String[] args) {
        String string = "HelloWorld!";

        // 字符串比较，大于就返回正数，小于是负数，等于是0(可以忽略大小写)
        int flag = string.compareTo("aaa");
        string.compareToIgnoreCase("helloworld!");
        System.out.println(flag);
        boolean equals = string.equals("HelloWorld!");
        string.equalsIgnoreCase("helloworld!");
        System.out.println(equals);

        // 转换大小写
        String upper = string.toUpperCase();
        System.out.println(upper);
        String lower = string.toLowerCase();
        System.out.println(lower);

        // 裁剪，包含前索引，不包含后索引
        String substring = string.substring(0, 3);
        System.out.println(substring);

        // 定位字符串字符
        char c = string.charAt(0);
        System.out.println(c);

        // 字符串长度
        int len = string.length();
        System.out.println(len);

        // 正则匹配
        boolean matches = string.matches("");
        System.out.println(matches);

        // 是否包含字符序列
        boolean contains = string.contains("Hello");
        System.out.println(contains);

        // 可以获取对应字符或者字符串的索引，还可以指定开始搜索的索引（可以从后开始搜索）
        int index1 = string.indexOf('l', 0);
        int index2 = string.lastIndexOf('l');
        System.out.println(index1 + " " + index2);

        // 去除首尾空格
        string = "  Hello World!  ";
        string = string.trim();
        System.out.println(string);

        // 以某个符号分割字符串
        String string1 = "3,4,5";
        String[] nums = string1.split(",");
        System.out.println(nums.length);

        // 代替，可以是字符或者字符串
        String replace1 = string.replace('e', 'a');
        String replace2 = string.replace("l", "ss");
        System.out.println(replace1 + " " + replace2);

        // 获取对应的字节数组
        byte[] bytes = string.getBytes();

        // 进行一个字符串的拼接
        String concat = string.concat(" wex");
        System.out.println(concat);

        // 获取字符数组
        char[] chars = string.toCharArray();
        System.out.println(chars.length);

        // 从常量池取出string，如果没有，就先加入常量池再取出
        String intern = string.intern();
        System.out.println(intern == string);

        // 以什么开始和结束
        boolean startsWith = string.startsWith("He");
        boolean endsWith = string.endsWith("!");

        // 进行一个重复
        String repeat = string.repeat(3);
        System.out.println(repeat);

        // 字符串格式化
        String format = String.format("%s: hello", "wex");
        System.out.println(format);

        // 获取其他类型的字符串表示
        String valueOf = String.valueOf(3);
        System.out.println(valueOf + " " + valueOf.getClass());

        // copyValueOf返回指定数组中表示该字符序列的String(包头不包尾)
        char[] cs = {'w', 'e', 'x'};
        String copyValueOf = String.copyValueOf(cs, 0, 1);
        System.out.println(copyValueOf);

        // charArrayAndString();
        // intToString();
    }

    /**
     * 字符串与字符数组之间的转换
     */
    public static void charArrayAndString(){
        String string = "abc";
        // 如果是字节数组基本转换也是一直的，不过调用的方法不同
        // byte[] bytes = string.getBytes();
        char[] chars = string.toCharArray();

        for (char c: chars){
            System.out.print(c + " ");
        }

        System.out.println();

        String str = new String(chars);
        System.out.println(str);
    }

    /**
     * 字符串与整型数组间的转换
     */
    public static void intToString(){
        String string = "1234567890";
        int[] nums = new int[string.length()];

        for (int i = 0; i < string.length(); i++) {
            nums[i] = Integer.parseInt(String.valueOf(string.charAt(i)));
            System.out.print(nums[i] +" ");
        }

        int[] nums2 = {1, 2, 3};
        String string2 = "";

        for (int num: nums2) {
            string2 += Integer.toString(num);
        }

        System.out.println();
        System.out.println(string2);
    }
}
