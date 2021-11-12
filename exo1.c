#include <stdio.h>

int s1(int n) {
    int s = 0;
    for (int k = 1; k < n + 1;k++) {
        s += 2*k + 1;
    }
    return s;
}

double s2(int n) {
    double s = 0.0;
    double k = 1.0;
    while (k < n + 1) {
        s += 1.0/(k*k);
        k++;
    }
    return s;
}

int s3(int n) {
    int s = 0;
    for (int i = 1; i < n + 1;i++) {
        for (int j = 1; j < n + 1;j++) {
            s += i + j;
        }
    }
    return s;
}

int s4(int n) {
    int s = 0;
    for (int i = 1; i < n + 1;i++) {
        for (int j = i; j < n + 1;j++) {
            s += i + j;
        }
    }
    return s;
}

int s5(int n) {
    int s = 0;
    for (int i = 1; i < n + 1;i++) {
        for (int j = 1; j < i;j++) {
            s += i + j;
        }
    }
    return s;
}

void main(void) {
    for (int i = 0;i <= 3;i++) {
        printf("%d\n",s1(i));
        printf("%f\n",s2(i));
        printf("%d\n",s3(i));
        printf("%d\n",s4(i));
        printf("%d\n",s5(i));
    }
}