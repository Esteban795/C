#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int etape(int number){
    int new = 0;
    while (number > 0){
        int temp = number % 10;
        new += temp * temp;
        number /= 10;
    }
    return new;
}


int main(void){
    int count = 0;
    bool* tab = malloc(sizeof(bool) * 600);
    for (int i = 0; i < 600;i++){ //init du tableau
        tab[i] = false;
    }
    for (int i = 1; i < 600;i++){ //precalcul pour les 600 premiers
        int j = i;
        while (j != 1 && j != 89){
            j = etape(j);
        }
        if (j == 89){
            count++;
            tab[i] = true;
        }
    }
    for (int i = 600; i < 10000000;i++){
        int number = etape(i);
        if (tab[number]) count++;
    }
    free(tab);
    printf("Nombre : %d\n",count);
    return 0;
}