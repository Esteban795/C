#include <stdio.h>
#include <stdlib.h>

double expo(double a,int n) {
    int b = a;
    int x = 1;
    int p = n;
    while (p > 0) {
        if (p%2 == 1) {
            x *= b;
        }
        b = b * b;
        p = p/2;
    }
    return x;
}

int main(int argc,char* argv[]) {
    if (argc != 3) {
        printf("Idiot tes arguments\n");
        return 1;
    }
    double x = atof(argv[1]);
    int n = atoi(argv[2]);
    printf("%f",expo(x,n));
    return 0;
}