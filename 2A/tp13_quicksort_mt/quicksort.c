#define INSERTION_LIMIT 16
#include <stdlib.h>
#include <stdio.h>

void swap(int *arr, int i, int j){
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}


void insertion_sort(int *arr, int len){
    for (int i = 0; i < len;i++){
        int temp = i;
        while (temp > 0 && arr[temp] < arr[temp - 1]){
            swap(arr,temp, temp - 1);
            temp--;
        }
    }
}

int partition(int *arr, int len){
    int i = 1;
    int x = arr[0];
    for (int j = 1; j < len; j++){
        if (arr[j] <= x) {
            swap(arr, i, j);
            i++;
        }
    }
    swap(arr, 0, i - 1);
    return i - 1;
}

void full_sort(int *arr, int len){
    if (len < INSERTION_LIMIT) {
        insertion_sort(arr,len);
    }
    else {
        int indice_pivot = partition(arr,len);
        full_sort(arr,indice_pivot + 1);
        full_sort(arr + indice_pivot + 1,len - indice_pivot - 1);
    }
}