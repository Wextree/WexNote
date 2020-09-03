package digit;

import java.util.*;
import java.util.function.Predicate;

// 测试ArrayList
public class ArrayListTest {
    public static void main(String[] args) {
        // 初始化
        // ArrayList<Integer> list = new ArrayList<>();
        // ArrayList<Integer> list = new ArrayList<>(10);
        ArrayList<Integer> list = new ArrayList<>(Arrays.asList(1, 2, 3));

        // 函数式接口输出列表元素
        list.forEach(System.out::println);

        // 列表内元素大小
        int size = list.size();

        // 在末尾添加一个元素，或者指定在索引位置添加元素
        list.add(4);
        list.add(1, 5);
        printList(list);
        // 添加一个集合中的所有元素(可以指定索引)
        // list.addAll(Arrays.asList(7, 8, 9));

        // 判断是否包含
        System.out.println("list has 3: " + list.contains(3) + " " + "list has 10: " + list.contains(10));
        // 元素不考虑顺序
        System.out.println("list has {1, 3, 4}: " + list.containsAll(Arrays.asList(1, 3, 4)));

        // 移除指定元素和指定索引上的元素
        list.remove(1);
        list.remove((Integer)1);
        printList(list);
        // list.removeAll(Arrays.asList(1, 2));  // 移除多个元素
        // 传入判定型函数接口，对函数进行筛选移除
        /* list.removeIf(new Predicate<Integer>() {
            @Override
            public boolean test(Integer integer) {
                return false;
            }
        }); */

        int index = list.indexOf(1);

        // 排序
        list.sort(new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o2 - o1;
            }
        });
        printList(list);

        // 克隆，集合的深拷贝，集合内元素的浅拷贝
        ArrayList<Integer> cloneList = (ArrayList<Integer>) list.clone();
        System.out.println(cloneList.getClass());
        System.out.println(cloneList);

        // 判空
        boolean isEmpty = list.isEmpty();

        // 截断，包含前面的索引，不包含后面的索引
        List<Integer> subList = list.subList(0, 1);
        System.out.println("subList: ");
        printList(subList);

        // 变成数组
        Object[] arrays1 = list.toArray(); // 不可以强制转换的
        Integer[] t = new Integer[list.size()];
        Integer[] arrays2 = list.toArray(t);

        testIterator();

        // 清空集合内元素
        list.clear();
    }

    /**
     * 测试两种迭代器，两种都是可以指定从哪个索引前面开始的，测试就没搞出来，可以自己去尝试
     */
    private static void testIterator() {
        ArrayList<Integer> list = new ArrayList<>(Arrays.asList(1, 2, 3, 4, 5));

        Iterator<Integer> iterator = list.iterator();

        while (iterator.hasNext()){
            Integer num = iterator.next();
            // iterator.remove(); // 删除刚刚遍历过的元素，下面的迭代器也是一样的，利用内部的modCount实现安全
            System.out.println(num);
        }

        // 双向迭代器，可以往前往后
        ListIterator<Integer> integerListIterator = list.listIterator();

        while (integerListIterator.hasNext()){
            Integer num = integerListIterator.next();
            System.out.println(num);
        }

        while (integerListIterator.hasPrevious()){
            Integer num = integerListIterator.previous();
            System.out.println(num);
        }
    }

    public static void printList(List<Integer> list){
        for (int num: list) {
            System.out.print(num + " ");
        }
        System.out.println();
    }
}
