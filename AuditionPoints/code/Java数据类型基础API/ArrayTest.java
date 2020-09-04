package digit;

import java.util.Arrays;
import java.util.List;

public class ArrayTest {

    public static void main(String[] args) {
        int[] nums = {3, 2, 2, 5, 4};
        int length = nums.length;
        System.out.println(length);
        int[] clone = nums.clone();
        printArr(clone, "clone:");

        // 该方法返回的是Arrays内部静态类ArrayList，而不是我们平常使用的ArrayList
        List<Integer> integers = Arrays.asList(1, 2, 3);

        // 拷贝数组，其内部调用了 System.arrayCopy() 方法，从下标0开始，如果超过原数组长度，会用null进行填充
        int[] copyOf = Arrays.copyOf(nums, 5);
        // Arrays.copyOfRange(nums, 0 ,3);  // 指定范围
        printArr(copyOf, "copyOf:");

        // 排序，可以排序所有的数字类型类型（还可以指定开始和结束的位置）
        // Arrays.parallelSort(nums); 并行排序，当数目较大的时候可以更高效
        Arrays.sort(nums);
        printArr(nums, "sort:");

        // 二分查找（必须先排序），可以指定比较器
        int index = Arrays.binarySearch(nums, 4);
        System.out.println("4 index: " + index);

        // 比较，一般比较（比较哈希值），deep对应的是多维数组
        int[] nums2 = {1, 2, 3};
        boolean equals = Arrays.equals(nums, nums2);
        int hashCode = Arrays.hashCode(nums);
        // boolean deepEquals = Arrays.deepEquals(nums, nums2); // 比较的是多维数组
        // Arrays.deepHashCode(nums);

        // 比较
        int compare = Arrays.compare(nums, nums2);
        int compareUnsigned = Arrays.compareUnsigned(nums, nums2);

        // 填充
        Arrays.fill(nums, 1);
        printArr(nums, "fill:");
    }

    private static void printArr(int[] nums, String prefix) {
        System.out.print(prefix + " ");

        for (int num: nums) {
            System.out.println(num + " ");
        }

        System.out.println();
    }
}
