package sort;

public class MergeSort {
    public static void main(String[] args){
        int[] arr = {1,1,10,7,2,4,7,62,3,4,2,1,8,9,19};
        mergeSort(arr, 0, arr.length-1);
        for (int num:arr) {
            System.out.print(num + " ");
        }
    }

    private static void mergeSort(int[] arr, int low, int high) {
        if (low >= high) return;

        int mid = low+(high-low)/2;

        mergeSort(arr, low, mid);
        mergeSort(arr, mid+1, high);
        merge(arr, low, high, mid);
    }

    private static void merge(int[] arr, int low, int high, int mid) {
        int len = high - low + 1;
        int[] temp = new int[len];
        int i = 0;
        int a = low;
        int b = mid + 1;

        while(a <= mid && b <= high){
            if (arr[a] <= arr[b]){
                temp[i++] = arr[a++];
            }else {
                temp[i++] = arr[b++];
            }
        }

        while (a <= mid){
             temp[i++] = arr[a++];
        }

//        while (b <= high){
//            temp[i++] = arr[b++];
//        }
//
//        for (int j = 0; j < temp.length; j++) {
//            arr[low++] = temp[j];
//        }

        int cha = high-b+1;
        for (int j = 0; j < len-cha; j++) {
            arr[low++] = temp[j];
        }
    }
}
