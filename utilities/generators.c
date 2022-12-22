#include <stdlib.h>
#include <stdio.h>


int* generate_tab(int n, int init){
    int* tab = malloc(sizeof(int) * n);
    for (int i = 0; i < n;i++){
        tab[i] = init;
    }
    return tab;
}

int** generate_matrix(int n,int init){
    int** m = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++){
        m[i] = generate_tab(n,init);
    }
    return m;
}