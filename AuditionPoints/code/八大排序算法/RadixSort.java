package sort;

/**
 * 基数排序（LSD 从低位开始）
 *
 * 基数排序适用于：
 *  (1)数据范围较小，建议在小于1000
 *  (2)每个数值都要大于等于0
 *
 * ①. 取得数组中的最大数，并取得位数；
 * ②. arr为原始数组，从最低位开始取每个位组成radix数组；
 * ③. 对radix进行计数排序（利用计数排序适用于小范围数的特点）；
 */
public class RadixSort {
    public static void main(String[] args) {
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        radixSort(arr,100);
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }

    private static void radixSort(int[] arr, int d) {
        int len = arr.length;
        int n = 1;
        int[][] bucket = new int[10][len];
        int[] order = new int[10];

        while (n < d){
            for (int num:arr) {
                int digit = (num/n) % 10;
                bucket[digit][order[digit]] = num;
                order[digit]++;
            }

            int k = 0;
            for (int i = 0; i < 10; i++) {
                if (order[i] != 0){
                    for (int j = 0; j < order[i]; j++) {
                        arr[k++] = bucket[i][j];
                    }
                }
                order[i] = 0;
            }
            n *= 10;
        }
    }
}
