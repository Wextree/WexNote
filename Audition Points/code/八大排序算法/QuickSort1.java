package sort;

public class QuickSort1 {
    public static void main(String[] args){
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        quickSort(arr, 0, arr.length-1);
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }

    private static void quickSort(int[] arr, int low, int high) {
        if (low >= high)
            return;

        int index = partition(arr, low, high);
        quickSort(arr, low, index-1);
        quickSort(arr, index+1, high);
    }

    private static int partition(int[] arr, int low, int high) {
        int temp = arr[low];
        int i = low;
        int j = high;

        while (i < j){
            while (i < j && arr[j] >= temp){
            j--;
            }
            while (i < j && arr[i] <= temp){
                i++;
            }

            if (i < j){
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }

        arr[low] = arr[j];
        arr[j] = temp;
        return j;
    }

}
