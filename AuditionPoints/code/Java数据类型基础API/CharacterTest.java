package digit;

/**
 * 常用方法
 * 其中还包括了一个缓存类，用于提高性能CharacterCache（127以内使用缓存中的元素，Integer也有）
 */
public class CharacterTest {
    public static void main(String[] args) {
        // 常量
        int bytes = Character.BYTES;
        int size = Character.SIZE;
        char maxValue = Character.MAX_VALUE;
        char minValue = Character.MIN_VALUE;

        // （判断）转换大小写(可以传入ASCII码)
        boolean isUpperCase = Character.isUpperCase('a');
        boolean isLowerCase = Character.isLowerCase('a');
        char lowerCase = Character.toLowerCase('C');
        char upperCase = Character.toUpperCase('c');

        // 判断
        boolean digit = Character.isDigit('1');
        boolean letter = Character.isLetter('a');
        // 确定字符是否为Unicode字符
        boolean defined = Character.isDefined('a');
        // 是否为Unicode空白字符
        boolean spaceChar = Character.isSpaceChar(' ');

        // 字符比较
        int compare = Character.compare('a', 'c');

        // 转换为字符串
        String string = Character.toString('c');

        // 包装成Character
        Character character = Character.valueOf('a');

        // 确定表示指定字符所需的字符的值的数目(Unicode代码点)
        int count = Character.charCount('好');
        System.out.println(count);

    }
}
