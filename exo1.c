#include <stdio.h>
#include <stdlib.h>


void extrema(int t[],int taille, int* min, int* max) {
    *max = t[0];
    *min = t[0];
    for (int i = 1; i < taille; i++) {
        if (*min > t[i]) {
            *min = t[i];
        } else if (*max < t[i]) {
            *max = t[i];
        }
    }
}

void mystere(int* x, int* y) {
    *x = *x - *y;
    *y = *x + *y;
    *x = *y - *x;
}
int main(void) {
    /*
    int maxi;
    int min;
    int tab[3] = {1,2,3};
    extrema(tab,3,&min,&maxi);
    printf("%d\n",min);
    printf("%d\n",maxi);*/
    int x2 = 3;
    int y2 = 3;
    int x3 = 3;
    mystere(&x3,&x3);
    printf("x2 = %d\n",x3);
    return 0;
}