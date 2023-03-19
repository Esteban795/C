#include <stdio.h>
#include <math.h>

double exo2while(int n) {
    double k = 0.0;
    double result = 1.0;
    while  (result < n) {
        k += 1;
        result *= 2.0;
    }
    return k;
}

void main(void) {
    int n;
    scanf("%d",&n);
    printf("%f\n",exo2while(n));
}