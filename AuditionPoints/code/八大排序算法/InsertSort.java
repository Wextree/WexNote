package sort;

/**
 * 插入排序
 *
 * 1. 从第一个元素开始，该元素可以认为已经被排序
 * 2. 取出下一个元素，在已经排序的元素序列中从后向前扫描
 * 3. 如果该元素（已排序）大于新元素，将该元素移到下一位置
 * 4. 重复步骤3，直到找到已排序的元素小于或者等于新元素的位置
 * 5. 将新元素插入到该位置后
 * 6. 重复步骤2~5
 */
public class InsertSort {
    public static void main(String[] args) {
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        insertSort2(arr);
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }

    // 移动多
    private static void insertSort(int[] arr) {

        for (int i = 1; i < arr.length; i++) {
            int temp = arr[i];
            for (int j = i; j >= 0 ; j--) {
                if (j > 0 && arr[j-1] > temp){
                    arr[j] = arr[j-1];
                }else {
                    arr[j] = temp;
                    break;
                }
            }
        }
    }

    // 交换多
    private static void insertSort2(int[] arr){
        for (int i = 1; i < arr.length; i++) {
            for (int j = i; j > 0 ; j--) {
                if (arr[j] < arr[j-1]){
                    int temp = arr[j];
                    arr[j] = arr[j-1];
                    arr[j-1] = temp;
                }else {
                    break;
                }
            }
        }
    }
}
