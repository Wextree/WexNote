package digit;

import java.util.*;

/**
 * 基本就是和TreeSet一致的
 * 一般结合红黑树的结构都是使得无序的集合变成相对有序
 * API简单理解
 */
public class TreeMapTest {
    public static void main(String[] args) {
        TreeMap<String, Integer> treeMap = new TreeMap<>();

        // 三个Collection
        Set<Map.Entry<String, Integer>> entrySet = treeMap.entrySet();
        Set<String> keySet = treeMap.keySet();
        Collection<Integer> values = treeMap.values();

        // 放入、删除、替换
        treeMap.put("wex", 1);
        treeMap.put("key", 2);
        treeMap.remove("wex");
        treeMap.remove("key", 2);
        treeMap.put("wex", 1);
        treeMap.put("key", 2);
        treeMap.replace("wex", 3);
        treeMap.replace("key", 2, 4);

        // ceiling，floor，higher，lower，first，last，poll，head，tail 可以参考TreeSet
        Map.Entry<String, Integer> ceilingEntry = treeMap.ceilingEntry("wex");
        String ceilingKey = treeMap.ceilingKey("wex");
        Map.Entry<String, Integer> floorEntry = treeMap.floorEntry("wex");
        String floorKey = treeMap.floorKey("wex");
        Map.Entry<String, Integer> higherEntry = treeMap.higherEntry("wex");
        String higherKey = treeMap.higherKey("wex");
        Map.Entry<String, Integer> lowerEntry = treeMap.lowerEntry("wex");
        String lowerKey = treeMap.lowerKey("wex");
        Map.Entry<String, Integer> firstEntry = treeMap.firstEntry();
        String firstKey = treeMap.firstKey();
        Map.Entry<String, Integer> lastEntry = treeMap.lastEntry();
        String lastKey = treeMap.lastKey();
        Map.Entry<String, Integer> pollFirstEntry = treeMap.pollFirstEntry();
        Map.Entry<String, Integer> pollLastEntry = treeMap.pollLastEntry();

        SortedMap<String, Integer> headMap = treeMap.headMap("wex");
        SortedMap<String, Integer> tailMap = treeMap.tailMap("wex");

        //是否包含
        boolean containsKey = treeMap.containsKey("wex");
        boolean containsValue = treeMap.containsValue(2);

        // 元素数量
        int size = treeMap.size();

        // 加入函数式接口执行
        // treeMap.forEach();

        // 获取比较器
        Comparator<? super String> comparator = treeMap.comparator();

        // 清楚元素
        treeMap.clear();
    }
}
