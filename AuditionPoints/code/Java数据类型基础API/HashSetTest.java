package digit;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;

public class HashSetTest {
    public static void main(String[] args) {
        // 初始化方法可以输入初始容量，也可以定义扩容因子，内部其实是一个hashmap
        // HashSet<Integer> set = new HashSet<>(16, 0.75);
        HashSet<Integer> set = new HashSet<>(Arrays.asList(1, 2, 3));

        // 添加元素（无序），输出只能靠迭代器
        set.add(4);
        set.add(10);
        set.add(8);
        printSet(set, "set: ");

        // 判空
        boolean isEmpty = set.isEmpty();
        System.out.println("isEmpty: " + isEmpty);

        // 克隆
        HashSet<Integer> clone = (HashSet<Integer>)set.clone();
        printSet(clone, "clone: ");

        set.contains(3);
        set.remove(3);
        // Spliterator是一个可分割迭代器(splitable iterator)，只要是提高并行能力
        // set.spliterator();

        // 清空set
        set.clear();
        int size = set.size();
        System.out.println("clear size = " + size);
    }

    /**
     * 遍历打印，set只能用迭代器进行遍历
     * @param set
     * @param prefix
     */
    private static void printSet(HashSet<Integer> set, String prefix) {
        System.out.print(prefix + " ");

        Iterator<Integer> iterator = set.iterator();

        while (iterator.hasNext()){
            Integer num = iterator.next();
            System.out.print(num +" ");
        }

        System.out.println();
    }
}
