#include <stdio.h>
#include <stdlib.h>

//buffer is also size 'len'.
void merge(int arr[],int mid,int len,int buffer[]){
    for (int i = 0; i <= mid;i++){
        buffer[i] = arr[i];
    }
    int i = 0;
    int j = mid;
    for (int k = 0; k < len; k++){
        if (i >= mid){
            arr[k] = arr[j];
            j++;
        } else if (j > len) {
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
    int mid = len/2;
    merge_sort_aux(arr,mid,buffer);
    merge_sort_aux(&arr[mid],len - mid,buffer);
    merge(arr,mid,len,buffer);
}

void merge_sort(int arr[],int len){
    int* buffer = malloc(sizeof(int) * len);
    merge_sort_aux(arr,len,buffer);
    free(buffer);
}