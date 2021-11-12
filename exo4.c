#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]) {
    int n;
    float x;
    if (argc != 3) {
        printf("Il faut donner un entier et un flottant\n");
        return 1;
    }
    n = atoi(argv[1]);
    x = atof(argv[2]);
    for (int i = 0; i < n;i++) {
        printf("%f\n",x);
    }
    return 0;
}