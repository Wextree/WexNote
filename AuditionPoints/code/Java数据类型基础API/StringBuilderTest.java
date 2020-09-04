package digit;

/**
 * 字符串拼接内部也是使用StringBuilder去执行的
 */
public class StringBuilderTest {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder("wex");

        // 追加
        sb.append(" is nice!");
        // sb.appendCodePoint(65);
        System.out.println(sb.toString());

        // 在某个索引处插入字符或者数字
        sb.insert(3, 's');
        System.out.println(sb.toString());

        // 查找对应字符串的位置（可以指定开始查找的索引位置）
        int indexOfNice = sb.indexOf("nice", 0);
        // sb.lastIndexOf();   // 从后往前搜索
        System.out.println("indexOfNice: " + indexOfNice);

        // 获取长度
        int length = sb.length();
        System.out.println("length: " + length);

        // 删除指定索引区间的字符串
        sb.delete(0, 3);
        System.out.println("newSb: " + sb.toString());

        // 两个sb在比较哈
        int compareTo = sb.compareTo(new StringBuilder("wex"));
        System.out.println("compareTo: " + compareTo);

        // 删除指定位置的字符
        sb.deleteCharAt(3);

        // 反转
        sb.reverse();
        System.out.println("reverse: " + sb.toString());
    }
}
