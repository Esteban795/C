#include <stdlib.h>
#include <stdio.h>

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}


int** random_int_matrix(int n){
    int** m = malloc(sizeof(int*) * n);
    for (int i = 0; i < n;i++){
        m[i] = malloc(sizeof(int) * n);
        for (int j = 0; j < n;j++){
            m[i][j] = rand_between(0,n);
        }
    }
    return m;
}

