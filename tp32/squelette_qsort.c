#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void print_array(int *t, int n){
    for (int i = 0; i < n; i++){
        printf("%d ", t[i]);
    }
    printf("\n");
}

int *range(int a, int b){
    assert(b >= a);
    int *t = malloc((b - a) * sizeof(int));
    for (int i = 0; i < b - a; i++) {
        t[i] = a + i;
    }
    return t;
}

int *random_array(int len, int bound){
    int *arr = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++){
        arr[i] = rand() % bound;
    }
    return arr;
}

int *copy(int *arr, int len){
    int *arr_copy = malloc(len * sizeof(int));
    memcpy(arr_copy, arr, len * sizeof(int));
    return arr_copy;
}

bool is_sorted(int *t, int len){
    for (int i = 1; i < len; i++){
        if (t[i] < t[i - 1]) return false;
    }
    return true;
}

bool is_equal(int *arr1, int *arr2, int len){
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) return false;
    }
    return true;
}




void insertion_sort(int *arr, int len){
    for (int i = 1; i < len; i++){
        int j = i;
        int x = arr[i];
        while (j > 0 && x < arr[j - 1]) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = x;
    }
}

void swap(int *t, int i, int j){
    int x = t[i];
    t[i] = t[j];
    t[j] = x;
}

int partition(int* arr,int len){
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

void quicksort(int* arr,int len){
    if (len <= 1) return;
    int k = partition(arr,len);
    quicksort(&arr[k+ 1],len - k - 1);
    quicksort(arr,k);
}


void test_quicksort(int arr_len_max,int nb_essais,int sup_bound){
    for (int l = 1; l <= arr_len_max;l++){
        for (int j = 0; j < nb_essais; j++){
            int* arr = random_array(l,sup_bound);
            int* copy_arr = copy(arr,l);
            insertion_sort(copy_arr,l);
            quicksort(arr,l);
            assert(is_equal(arr,copy_arr,l));
            free(arr);
            free(copy_arr);
        }
    }
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

int quickselect(int* arr,int k, int len){
    int* copy_arr = copy(arr,len);
    int result = quickselect_aux(copy_arr,k,len);
    free(copy_arr);
    return result;
}

void siftdown(int* arr,int i, int len){
    int index_min = i;
    int left = 2 * i + 1;
    int right = 2*i + 2;
    int up = (i -1)/2;
    if (left < len && arr[left] > arr[i]) {index_min = left;}
    if (right < len && arr[right] > arr[i]) {index_min = right;}
    if (index_min != i){
        swap(arr,i,index_min);
        siftdown(arr,index_min,len);
    }
}

void extract_max(int* arr,int len){
    swap(arr,0,len - 1);
    siftdown(arr,0,len - 1);
}

int heapify(int* tab,int len){
    for (int i = (len - 1)/2; i >= 0; i--){
        siftdown(tab,i,len);
    }
}

void heapsort(int* arr,int len){
    heapify(arr,len);
    for (int i = 0; i < len;i++){
        extract_max(arr,len);
    }
}

int ilog(int n){
    int i = 0;
    while (n > 1){
        i++;
        n = n/2;
    }
    return i;
}

void introsort_aux(int* arr, int len,int max_height){
    if (len <= 1) return;
    if (max_height < 0){
        heapsort(arr,len);
    } else {
        int k = partition(arr,len);
        introsort_aux(&arr[k + 1],len - k - 1,max_height - 1);
        introsort_aux(arr,k,max_height - 1);
    }
}

void introsort(int* arr,int len){
    int max_height = 2 * ilog(len) + 1; 
    introsort_aux(arr,len,max_height);
}


int main(void){
    test_quicksort(10,3,1000);
    return 0;
}
