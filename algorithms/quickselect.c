#include <stdio.h>
#include <stdlib.h>

int partition(int* arr,int len){ //quicksort-like partition
    assert(arr != NULL);
    int value_pivot = arr[0];
    int i = 1;
    for (int j = 1; j < len ;j++){
        if (arr[j] <= value_pivot) {
            swap(arr,i,j);
            i++;
        }
    }
    swap(arr,i - 1,0);
    return i - 1;
}

int quickselect_aux(int* arr,int k, int len){
    assert(k < len);
    int indice_pivot = partition(arr,len);
    if (indice_pivot == k){ return arr[indice_pivot];}
    if (indice_pivot < k){
        return quickselect_aux(&arr[indice_pivot + 1],k - indice_pivot - 1,len - indice_pivot - 1);
    } else {
        return quickselect_aux(arr,k,indice_pivot);
    }
}

int quickselect(int* arr,int k, int len){ //quickselect gives you the element that would be at index k if arr was sorted in O(n)
    int* copy_arr = copy(arr,len);
    int result = quickselect_aux(copy_arr,k,len);
    free(copy_arr);
    return result;
}