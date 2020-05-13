package sort;

/**
 * 堆排序
 *
 * 1. 先将初始序列K[1..n]建成一个大顶堆, 那么此时第一个元素K1最大, 此堆为初始的无序区.
 * 2. 再将关键字最大的记录K1 (即堆顶, 第一个元素)和无序区的最后一个记录 Kn 交换, 由此得到新的无序区K[1..n−1]和有序区K[n], 且满足K[1..n−1].keys⩽K[n].key
 * 3. 交换K1 和 Kn 后, 堆顶可能违反堆性质, 因此需将K[1..n−1]调整为堆. 然后重复步骤②, 直到无序区只有一个元素时停止.
 *
 * 大顶堆：arr[i] >= arr[2i+1] && arr[i] >= arr[2i+2]
 * 小顶堆：arr[i] <= arr[2i+1] && arr[i] <= arr[2i+2]
 */
public class HeapSort {
    public static void main(String[] args) {
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        heapSort(arr);
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }

    private static void heapSort(int[] arr) {
        // 从第一个非叶子结点开始调整堆
        for (int i = arr.length/2-1; i >=0 ; i--) {
            max_heapify(arr, i, arr.length);
        }
        
        // 调整堆结构并且交换头尾元素
        for (int i = arr.length-1; i > 0; i--) {
            swap(arr, 0, i);
            max_heapify(arr, 0, i);
        }
    }

    private static void swap(int[] arr, int m, int n) {
        int temp = arr[m];
        arr[m] = arr[n];
        arr[n] = temp;
    }

    private static void max_heapify(int[] arr, int i, int len) {
        int temp = arr[i];

        // 从该结点的左结点开始
        for (int j = 2*i+1; j < len; j = j*2+1) {
            // 如果左结点小于右结点，那么指向右结点
            if (j+1 < len && arr[j+1] > arr[j])
                j++;

            if (arr[j] > temp){
                arr[i] = arr[j];
                i = j;
            }else {
                break;
            }
        }

        arr[i] = temp;
    }
}
