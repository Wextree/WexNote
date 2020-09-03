package digit;

import java.util.*;

public class TreeSetTest {
    public static void main(String[] args) {
        // 可以传入比较器，内部有序，默认自然排序
        TreeSet<Integer> treeSet = new TreeSet<>(new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o1 - o2;
            }
        });

        // 添加元素(与HashSet重复的API不测)
        treeSet.add(20);
        treeSet.add(18);
        treeSet.add(23);
        treeSet.add(22);
        treeSet.add(17);
        treeSet.add(24);
        treeSet.add(19);
        printSet(treeSet, "treeSet:");

        // 方法返回在这个集合中大于或者等于给定元素的最小元素，没有就null
        int moreThanNum = treeSet.ceiling(21);
        // 方法返回在这个集合中小于或者等于给定元素的最大元素，没有就null
        int lessThanNum = treeSet.floor(21);
        System.out.println(moreThanNum + " " + lessThanNum);

        // 获取自己的比较器
        Comparator<? super Integer> comparator = treeSet.comparator();

        // 它以相反的顺序获取此TreeSet元素
        NavigableSet<Integer> integers = treeSet.descendingSet();

        // 获取第一个和最后一个元素
        Integer first = treeSet.first();
        Integer last = treeSet.last();
        System.out.println("first: " + first + " " + "last: " + last);

        // 用于返回小于指定元素的元素组，第二个参数设置是否加入边界值。
        SortedSet<Integer> headSet = treeSet.headSet(22, true);
        System.out.print("headSet: ");
        for (Integer num: headSet){
            System.out.print(num + " ");
        }

        System.out.println();

        // 与上面差不多，只是它返回比它大的集合
        NavigableSet<Integer> tailSet = treeSet.tailSet(20, true);
        System.out.print("tailSet: ");
        for (Integer num: tailSet){
            System.out.print(num + " ");
        }

        System.out.println();

        // 跟ceiling和floor差不多，但是这个不包含对应元素
        Integer higher = treeSet.higher(20);
        Integer lower = treeSet.lower(20);
        System.out.println("higher: " + higher + " " + "lower: " + lower);

        // 和之前的last和first差不多，只是这个方法有把数取出set
        Integer pollFirst = treeSet.pollFirst();
        Integer pollLast = treeSet.pollLast();
        System.out.println("pollFirst: " + pollFirst + " " + "pollLast: " + pollLast);
    }

    /**
     * 遍历打印，set只能用迭代器进行遍历
     * @param set
     * @param prefix
     */
    private static void printSet(TreeSet<Integer> set, String prefix) {
        System.out.println(prefix + " ");

        // 降序迭代器
        Iterator<Integer> iterator = set.descendingIterator();

        while (iterator.hasNext()){
            Integer num = iterator.next();
            System.out.print(num +" ");
        }

        System.out.println();
    }
}
