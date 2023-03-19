#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "clock.h"
#include "counter.h"
#include "deque.h"
#include "quicksort.c"
#include "utils.c"


void *parallel_qsort_1(void *arg){
    slice_t slice = *(slice_t*)arg;
    if (slice.len <= 1) return NULL;
    int pivot = partition(slice.start, slice.len);
    slice_t lo = {.start = slice.start, .len = pivot};
    slice_t hi = {.start = slice.start + pivot + 1,
                  .len = slice.len - pivot - 1};
    pthread_t t_lo, t_hi;
    pthread_create(&t_lo, NULL, parallel_qsort_1, &lo);
    pthread_create(&t_hi, NULL, parallel_qsort_1, &hi);
    pthread_join(t_lo, NULL);
    pthread_join(t_hi, NULL);
    return NULL;
}

void *parallel_qsort_2(void *arg){
    slice_t slice = *(slice_t*)arg;
    if (slice.len <= 1) return NULL;
    int pivot = partition(slice.start,slice.len);
    slice_t lo = {.start = slice.start, .len = pivot};
    slice_t hi = {.start = slice.start + pivot + 1,
                  .len = slice.len - pivot - 1};
    pthread_t t_lo;
    pthread_create(&t_lo,NULL,parallel_qsort_2,&lo);
    parallel_qsort_2(&hi);
    pthread_join(t_lo,NULL);
    return NULL;

}
void *parallel_qsort_3(void *arg){
    slice_t slice = *(slice_t*)arg;
    if (slice.len <= 1000) {
        full_sort(slice.start, slice.len);
        return NULL;
    }
    int pivot = partition(slice.start, slice.len);
    slice_t lo = {.start = slice.start, .len = pivot};
    slice_t hi = {.start = slice.start + pivot + 1,
                .len = slice.len - pivot - 1};
    pthread_t thread;
    pthread_create(&thread, NULL, parallel_qsort_3, &lo);
    parallel_qsort_3(&hi);
    pthread_join(thread, NULL);
    return NULL;
}

/*
void parallel_sort_naive(int *arr, int len){
    slice_t slice = {.start = arr, .len = len};
    parallel_qsort_3(&slice);
}*/


int main(int argc,char* argv[]){
    if (argc < 1) return 1;
    int len = atoi(argv[1]);
    int* random = rand_array(len);
    slice_t s = {.start = random, .len =len};
    timestamp start = gettime();
    parallel_qsort_2(&s);
    timestamp end =  gettime();
    printf("Temps pris pour %d éléments : %f\n\n",len,delta_t(start,end));
    return 0;
}