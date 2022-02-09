#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int datatype;

struct heap{
    int size;
    int capacity;
    datatype *arr;
};

typedef struct heap heap;

heap* heap_new(int capacity){
    heap* h = malloc(sizeof(heap));
    h->arr = malloc(capacity * sizeof(datatype));
    h->size = 0;
    return h;
}

void heap_delete(heap* h){
    free(h->arr);
    free(h);
}

int up(int i){
    return (i - 1)/2;
}

int left(int i){
    return 2 * i + 1;
}

int right(int i){
    return 2 * i + 2;
}

void swap(heap* h,int i, int j){
    datatype temp = h->arr[i];
    h->arr[i] = h->arr[j];
    h->arr[j] = temp;
}

void sift_up(heap* h,int i){
    while (i > 0 && h->arr[i] < h->arr[up(i)]){
        swap(h,i,up(i));
        i = up(i);
    }
}

void heap_insert(heap* h,datatype x){
    assert(h->size < h->capacity);
    h->arr[h->size] = x;
    sift_up(h,h->size);
    h->size++;
}

void sift_down(heap* h,int i){
    int index_mini = i;
    if (left(i) < h->size && h->arr[left(i)] < h->arr[index_mini]){
        index_mini = left(i);
    }
    if (right(i) < h->size && h->arr[right(i)] < h->arr[index_mini]) {
        index_mini = right(i);
    }
    if (index_mini != i){
        swap(h,index_mini,i);
        sift_down(h,index_mini);
    }
}

datatype extract_min(heap* h){
    assert(h->size > 0);
    datatype result = h->arr[0];
    h->size--;
    swap(h,0,h->size);
    sift_down(h,0);
    return result;
}

heap* heapify(int size,datatype arr[]){
    heap* h = malloc(sizeof(heap));
    h->arr = arr;
    h->size = size;
    h->capacity = size;
    for (int i = up(size); i >= 0; i--){
        sift_down(h,i);
    }
    return h;
}

void heapsort(datatype arr[],int size){
    heap* h = heapify(size,arr);
    for (int i = 0; i < size;i++){
        extract_min(h);
    }
    free(h);
}


int main(void){
    int arr[5] = {1,5,3,4,2};
    heapsort(arr,5);
    for (int i = 0; i < 5;i++){
        printf("%d\n",arr[i]);
    }
    return 0;
}