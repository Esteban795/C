#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


float prod(int inf,int sup){
    float produit = 1;
    for (int i = inf; i <= sup;i++){
        produit*= i;
    }
    return produit;
}

int main(void){
    float esp_xi = 1 - (prod(41,50)/prod(61,70)); //
    printf("Esp : %f\n", 7 * esp_xi);
}