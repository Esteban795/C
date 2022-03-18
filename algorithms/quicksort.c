#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void swap(int* arr,int i,int j) { 
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(int* arr, int start, int end, int index_pivot){ //everything left to ipivot has a lower value than value_pivot, right for higher values.
    assert(arr != NULL);
    int value_pivot = arr[index_pivot];
    swap(arr,index_pivot,end - 1);
    int i = start;
    for (int j = start; i < end;j++){
        if (arr[j] <= value_pivot) {
            swap(arr,i,j);
            i++;
        }
    }
    return i - 1;
}

void quicksort_aux(int* arr,int start, int end){
    if (start < end) {
        int k = partition(arr,start,end,start);
        quicksort_aux(arr,start,k);
        quicksort_aux(arr,k + 1,end);
    } else return;
}

void quicksort(int* arr,int size){
    assert(arr != NULL);
    quicksort_aux(arr,0,size);
}