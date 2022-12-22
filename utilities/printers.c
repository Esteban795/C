#include <stdlib.h>
#include <stdio.h>

void print_tab(int* tab,int n){
    for (int i = 0; i < n; i++){
        printf("%d | ",tab[i]);
    }
    printf("\n\n\n");
}

void print_matrix(int** m,int n){
    for (int i = 0; i < n;i++){
        print_tab(m[i],n);
        printf("\n");
    }
}