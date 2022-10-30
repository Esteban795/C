#include <stdio.h>

void incremente(int* p) {
    *p = *p + 1;
}


void f(int* x1, int* x2) {
    if (*x2 < *x1) {
        incremente(x2);
    } else {
        incremente(x1);
    }
}

int main(void) {
    int N = 3;
    int M = 3;
    f(&N,&M);
    printf("N : %d, M : %d",N,M);
}

