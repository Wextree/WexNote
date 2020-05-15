import java.lang.reflect.Array;
import java.util.Arrays;

/**
 *使用反射编写泛型数组拷贝
 */
public class CopyArray {
    public static void main(String[] args) {
        int[] array = {1,2,3,4,5};
        int[] newArray = (int[]) arrayCopyOf(array, array.length);
        System.out.println(Arrays.toString(newArray));

        String[] strArray = {"tom", "jack", "harry"};
        String[] newStrArray = (String[]) arrayCopyOf(strArray, strArray.length);
        System.out.println(Arrays.toString(newStrArray));
    }

    private static Object arrayCopyOf(Object array, int length) {
        Class clazz = array.getClass();
        // 判断是不是数组，不是就返回
        if (!clazz.isArray()){
            return null;
        }

        // 确定传入的数组类型
        Class componentType = clazz.getComponentType();
        // 获取传入数组的长度
        int len = Array.getLength(array);
        // 构造新的数组
        Object newArray = Array.newInstance(componentType, length);
        System.arraycopy(array, 0, newArray, 0, Math.min(len, length));
        return newArray;
    }
}
