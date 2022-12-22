#include <stdlib.h>
#include <stdio.h>


int* generate_arr(int n, int init){
    int* arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n;i++){
        arr[i] = init;
    }
    return arr;
}

int** generate_matrix(int n,int init){
    int** m = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++){
        m[i] = generate_arr(n,init);
    }
    return m;
}