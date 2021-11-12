#include <stdio.h>
#include <stdlib.h>

const int taille = 10;
int tab[10];

void remplire(void) {
    for (int i = 0;i < taille;i++) {
        tab[i] = rand();
    }
}

void affiche(void) {
    for (int i = 0;i < taille;i++) {
        printf("%d ",tab[i]);
    }
    printf("\n");
}

int minimum(void) {
    int min = tab[0];
    for (int i = 1; i < taille; i++) {
        if (tab[i] < min) {
            min = tab[i];
        }
    }
    return min;
}

int indice_minimum(void) {
    int min = tab[0];
    int index = 0;
    for (int i = 1; i < taille; i++) {
        if (tab[i] < min) {
            min = tab[i];
            index = i;
        }
    }
    return index;
}

void tri_insertion(void) {
    for (int j = 1;j < taille;j++) {
        int elt = tab[j];
        int i = j;
        while (i > 0 && tab[i - 1] > elt) {
            tab[i] = tab[i - 1];
            i -= 1;
        }
        tab[i] = elt;
    }
}


void main(void) {
    remplire();
    affiche();
    printf("%d",minimum());
    printf(" %d",indice_minimum());
    tri_insertion();
    printf("\n\n");
    affiche();
}