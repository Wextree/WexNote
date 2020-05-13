package sort;

public class QuickSort {
    public static void main(String[] args){
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        quickSort(arr, 0, arr.length-1);
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }

    private static void quickSort(int[] arr, int low, int high) {
        if (low >= high){
            return;
        }

        int i = low;
        int j = high;
        int temp = arr[low];

        while (i < j){
            while (j>i && arr[j]>=temp){
                j--;
            }

            while (i<j && arr[i]<=temp){
                i++;
            }

            if (i < j){
                int k = arr[i];
                arr[i] = arr[j];
                arr[j] = k;
            }
        }

        // 把基准的那个交换
        arr[low] = arr[i];
        arr[i] = temp;

        quickSort(arr, low, j-1);
        quickSort(arr, j+1, high);

    }
}
