#include <stdlib.h>
#include <stdio.h>


void free_matrix(int** matrix,int n){
    for (int i = 0; i < n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

