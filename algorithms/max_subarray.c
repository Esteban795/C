#include <stdio.h>
#include <stdlib.h>


int max(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    }
    return n2;
}

int max_sum_subarray(int array[],int array_length, int window_size) {
    int max_sum = 0;
    for (int i = 0; i < window_size;i++) {
        max_sum += array[i];
    }
    int sum = max_sum;
    for (int i = 1;i < array_length - 1;i++) {
        sum = sum - array[i - 1] + array[i];
        if (sum > max_sum) {
            max_sum = sum;
        }
    }
    return max_sum;
}


int max_subarray_sum(int* array[],int length) {
    int max_so_far = 0; 
    int max_ending_here = 0;
    for (int i = 0; i < length; i++) {
        max_ending_here = max(max_ending_here,max_ending_here + array[i]);
        max_so_far = max(max_so_far,max_ending_here);
    }
    return max_so_far;
}