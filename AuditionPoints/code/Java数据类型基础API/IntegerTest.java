package digit;

/**
 * 只列举常用的，不常用的等以后如果有用到再回过头总结，不然也记不住
 */
public class IntegerTest {
    public static void main(String[] args) {
        // 常用构造方法(已被弃用)
        Integer integer = new Integer("123");


        // 几个常用的成员变量
        int size = Integer.SIZE;
        int bytes = Integer.BYTES;
        int maxValue = Integer.MAX_VALUE;
        int minValue = Integer.MIN_VALUE;
        System.out.println("size: " + size + " " + "bytes: " + bytes);
        System.out.println("maxValue: " + maxValue + " " + "minValue: " + minValue);

        // 字符串转换为整形
        int num1 = Integer.parseInt("123");
        // radix：以什么进制取翻译
        int num2 = Integer.parseInt("1110", 2);
        int num3 = Integer.parseInt("s1234s", 1, 4, 10);
        System.out.println(num1 + " " + num2 + " " + num3);

        // 数字转换为字符串
        String string = Integer.toString(123);  // 也可以任何类型加上 “ ” 也会变成字符串（推荐）
        System.out.println(string + " " + string.getClass());

        // 比较两个数大小
        int compare = Integer.compare(1, 2);
        int max = Integer.max(1, 2);
        int min = Integer.min(1, 2);
        System.out.println("compare: " + compare + " " + "max: " + max + " " + "min: " + min);

        // 翻转每一位01
        int reverse = Integer.reverse(123);
        System.out.println("reverse: " + reverse);

        // 取值，把其他类型的值转换为Integer类型，字符串不能包含字母或者其他非数字的字符
        Integer valueOf = Integer.valueOf("123");
        System.out.println("valueOf: " + valueOf);

        // 位为1计数
        int bitCount = Integer.bitCount(8);
        System.out.println("bit: " + bitCount);

        // 以无符号数进行比较 -1 > 1
        int compareUnsigned = Integer.compareUnsigned(-1, 1);
        System.out.println("-1 > 1 ? : " + compareUnsigned);

        // 十进制转成不同的进制，三个静态方法的返回值都是以字符串的形式返回
        int src = 1000;
        String Binary = Integer.toBinaryString(src);
        String OctalNumberSystem = Integer.toOctalString(src);
        String Hexadecimal = Integer.toHexString(src);
        System.out.printf("%d的二进制是：%s,八进制是:%s,十六进制是：%s\n",src,Binary,OctalNumberSystem,Hexadecimal);

    }
}
