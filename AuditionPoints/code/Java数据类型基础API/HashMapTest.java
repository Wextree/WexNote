package digit;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

/**
 * map只将hashMap和treeMap，其他差不多
 * LRU算法需要用到LinkedHashMap，内部还维持着双向链表
 */
public class HashMapTest {
    public static void main(String[] args) {
        // 初始化方法，默认等于new HashMap<>(16, 0.75)
        HashMap<String, Integer> map = new HashMap<>();

        // 添加、删除、获取（如果要修改对应key上的值，应该先拿出来改完再覆盖上去）
        map.put("wex", 1);
        map.put("test", 2);
        // 删除会返回对应的value，还有一个重载的方法，传入key和value，返回是否成功的boolean
        Integer remove = map.remove("test");
        System.out.println("remove value: " + remove);
        Integer wexValue = map.get("wex");
        Integer testValue = map.getOrDefault("test", 2);
        System.out.println("wex value: " + wexValue + " " + "test value: " + testValue);
        map.put("test", testValue);
        printMap(map, "HashMap:");

        // 获取map的各种set，可以用来遍历
        Set<Map.Entry<String, Integer>> entrySet = map.entrySet();
        Set<String> keySet = map.keySet();
        Collection<Integer> values = map.values();

        // 替代，跟其他的差不多
        map.replace("wex", 3);
        map.replace("test", 2, 1);
        printMap(map, "replace:");

        // 克隆
        HashMap<String, Integer> clone = (HashMap<String, Integer>)map.clone();
        printMap(clone, "clone:");

        // 判断是否包含
        boolean containsKey = map.containsKey("wex");
        boolean containsValue = map.containsValue("0");
        System.out.println("containsKey: " + containsKey + " " + "containsValue: " + containsValue);

        // 清除所有内容
        map.clear();

        // jdk 1.8 之后提供的计算方法
        // testCompute();

        // 如果给定的key不存在（或者key对应的value为null），关联给定的key和给定的value，并返回null；
        // 如果存在，返回当前值（不会把value放进去）；
        map.put("wex", 1);
        map.putIfAbsent("wex", 2);
        map.putIfAbsent("sss", 10);
        printMap(map, "putIfAbsent:");
    }

    /**
     * 统计字符数量
     */
    private static void testCompute() {
        String str = "hello java, i am vary happy! nice to meet you";

        HashMap<Character, Integer> map = new HashMap<>();

        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            // computeIfAbsent和computeIfPresent对应key进行了筛选，只有对应的key不存在或者存在才会进行的函数操作
            map.compute(c, (k, v)->{
               if (v == null){
                   v = 1;
               }else {
                   v += 1;
               }

               return v;
            });
        }

        System.out.println("testCompute: |");

        Set<Character> keySet = map.keySet();

        for (Character key: keySet) {
            System.out.print(" " + key + " " + map.get(key) + " |");
        }

        System.out.println();

    }

    private static void printMap(HashMap<String, Integer> map, String prefix) {
        System.out.print(prefix + " |");

        Set<String> keySet = map.keySet();

        for (String key: keySet) {
            System.out.print(" " + key + " " + map.get(key) + " |");
        }

        System.out.println();
    }
}
