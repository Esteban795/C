#include <stdio.h>
#include <stdlib.h>

//buffer is also size 'len'.

//arr[0] <= arr[1] <=... arr[mid - 1] && arr[mid] <= arr[mid + 1] <= ... arr[len]


void merge(int arr[],int mid,int len,int buffer[]){
    for (int i = 0; i <= mid;i++){
        buffer[i] = arr[i];
    }
    int i = 0;
    int j = mid;
    for (int k = 0; k < len; k++){
        if (i >= mid){ //first half of the array is done, we just add the rest of the second part
            arr[k] = arr[j];
            j++;
        } else if (j > len) { //same but second half is done
            arr[k] = buffer[i];
            i++;
        }
        else if (arr[j] <= buffer[i]){
            arr[k] = buffer[i];
            i++;
        } else {
            arr[k] = arr[j];
            j++;
        }
    }
}

void merge_sort_aux(int arr[],int len, int buffer[]){
    if (len <= 1) return;
    int mid = len / 2;
    merge_sort_aux(arr,mid,buffer);
    merge_sort_aux(&arr[mid],len - mid,buffer);
    merge(arr,mid,len,buffer);
}

void merge_sort(int arr[],int len){
    int* buffer = malloc(sizeof(int) * len);
    merge_sort_aux(arr,len,buffer);
    free(buffer);
}