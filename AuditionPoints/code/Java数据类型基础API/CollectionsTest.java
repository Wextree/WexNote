package digit;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;

public class CollectionsTest {
    public static void main(String[] args) {
        ArrayList<Integer> list = new ArrayList<>(Arrays.asList(1, 3, 2, 9, 8, 5, 4));

        // 随机打乱集合中的元素
        Collections.shuffle(list);
        printList(list, "shuffle:");

        // 排序
        Collections.sort(list, new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o2 - o1;
            }
        });
        printList(list, "sort:");

        // 翻转
        Collections.reverse(list);
        printList(list, "reverse:");

        // 找到对应元素的索引(二分查找)，只有继承了排序接口才可以，set不行
        // 主要看源码(需要顺序排列才能达到效果),可以自己定义比较器，默认自然比较
        int index = Collections.binarySearch(list, 3);
        System.out.println(index);

        // 用对象o替换集合list中的所有元素
        ArrayList<Integer> copy = new ArrayList<>(Arrays.asList(0, 0, 0, 0, 0, 0, 0));
        Collections.fill(copy, 2);
        printList(copy, "list2:");

        // 将集合n中的元素全部复制到m中,并且覆盖相应索引的元素
        Collections.copy(copy, list);
        printList(copy, "copy:");

        // 取最大最小值，可以使用默认自然比较，也可以自己添加比较器
        int max = Collections.max(list);
        int min = Collections.min(list, new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o1 - o2;
            }
        });
        System.out.println("max: " + max + " " + "min: " + min);

        // 找到集合中首次找到自己和的索引
        int subIndex = Collections.indexOfSubList(list, Arrays.asList(1, 2, 3));
        System.out.println("{1, 2, 3} index :" + subIndex);
        // int subIndex2 = Collections.lastIndexOfSubList(list, Arrays.asList(8, 9));

        // 集合中的元素向后移m个位置，在后面被遮盖的元素循环到前面来
        Collections.rotate(list, 2);
        printList(list, "rotate: ");

        // 交换集合中指定元素索引的位置
        Collections.swap(list, 3, 2);
        printList(list, "swap 3 and 2 : ");

        // 替换批定元素为某元素,若要替换的值存在刚返回true,反之返回false
        Collections.replaceAll(list, 9, 10);
        printList(list, "replace 9 to 10 :");
    }

    private static void printList(ArrayList<Integer> list, String prefix) {
        System.out.print(prefix + " ");

        for (Integer num: list) {
            System.out.print(num + " ");
        }

        System.out.println();
    }
}
